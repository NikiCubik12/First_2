#include <clocale>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <cctype>
#include <fstream>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

#include "MutableArraySequence.hpp"
#include "ReadOnlyStream.hpp"
#include "PatternCounter.hpp"
#include "LazySequence.hpp"
#include "Generator.hpp"

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

std::string escapeHtml(const std::string& s)
{
    std::string result;
    for (char c : s)
    {
        if (c == '<') result += "&lt;";
        else if (c == '>') result += "&gt;";
        else if (c == '&') result += "&amp;";
        else if (c == '"') result += "&quot;";
        else result += c;
    }
    return result;
}

std::string urlDecode(const std::string& value)
{
    std::string decoded;
    for (size_t i = 0; i < value.length(); ++i)
    {
        if (value[i] == '+') decoded += ' ';
        else if (value[i] == '%' && i + 2 < value.length())
        {
            char hex[3] = {value[i+1], value[i+2], 0};
            decoded += static_cast<char>(strtol(hex, NULL, 16));
            i += 2;
        }
        else decoded += value[i];
    }
    return decoded;
}

// ==================== ГЛОБАЛЬНОЕ СОСТОЯНИЕ ====================

struct AppState
{
    MutableArraySequence<std::string> patterns;
    std::string sourceText;
    std::string filePath;
    bool allowOverlap;
    size_t lazyLength;
    std::string lazyAlphabet;
    size_t processedCount;
    MutableArraySequence<std::pair<std::string, size_t>> lastResults;
    std::string lastErrorMessage;
    bool isProcessing;
    
    AppState() : allowOverlap(true), lazyLength(1000000), 
                 lazyAlphabet("ab"), processedCount(0), isProcessing(false) {}
};

static AppState g_state;

#ifdef _WIN32
static CRITICAL_SECTION g_stateMutex;
#else
static pthread_mutex_t g_stateMutex = PTHREAD_MUTEX_INITIALIZER;
#endif

class StateLock
{
public:
    StateLock()
    {
#ifdef _WIN32
        EnterCriticalSection(&g_stateMutex);
#else
        pthread_mutex_lock(&g_stateMutex);
#endif
    }

    ~StateLock()
    {
#ifdef _WIN32
        LeaveCriticalSection(&g_stateMutex);
#else
        pthread_mutex_unlock(&g_stateMutex);
#endif
    }
};

// ==================== ГЕНЕРАЦИЯ HTML ====================

std::string renderMainPage()
{
    std::stringstream ss;
    ss << "<!DOCTYPE html>\n";
    ss << "<html lang=\"ru\">\n";
    ss << "<head>\n";
    ss << "<meta charset=\"UTF-8\">\n";
    ss << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    ss << "<title>Лабораторная работа №4 - Поиск подстрок</title>\n";
    ss << "<style>\n";
    ss << "* { margin: 0; padding: 0; box-sizing: border-box; }\n";
    ss << "body { font-family: 'Segoe UI', system-ui, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; padding: 20px; }\n";
    ss << ".container { max-width: 1200px; margin: 0 auto; background: white; border-radius: 24px; box-shadow: 0 25px 50px -12px rgba(0,0,0,0.25); overflow: hidden; }\n";
    ss << "h1 { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 24px 32px; text-align: center; font-size: 28px; }\n";
    ss << ".content { padding: 32px; }\n";
    ss << ".section { margin-bottom: 32px; border: 1px solid #e9ecef; border-radius: 16px; padding: 20px; background: #f8f9fa; }\n";
    ss << ".section h2 { color: #667eea; margin-bottom: 16px; font-size: 20px; }\n";
    ss << ".section h3 { color: #764ba2; margin: 16px 0 12px 0; font-size: 18px; }\n";
    ss << "label { display: block; font-weight: 600; color: #555; margin-bottom: 8px; }\n";
    ss << "input[type=\"text\"], input[type=\"number\"], textarea, select { width: 100%; padding: 10px; border: 2px solid #dee2e6; border-radius: 8px; font-size: 14px; margin-bottom: 12px; }\n";
    ss << "input[type=\"file\"] { width: 100%; padding: 8px; border: 2px solid #dee2e6; border-radius: 8px; font-size: 14px; margin-bottom: 12px; background: white; }\n";
    ss << "textarea { font-family: monospace; resize: vertical; min-height: 100px; }\n";
    ss << "button { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; padding: 10px 20px; border-radius: 8px; cursor: pointer; font-weight: 600; margin-right: 10px; margin-bottom: 10px; }\n";
    ss << "button:hover { transform: translateY(-1px); }\n";
    ss << "button:disabled { opacity: 0.5; cursor: not-allowed; }\n";
    ss << ".pattern-list { background: white; border-radius: 12px; padding: 16px; margin-top: 12px; max-height: 200px; overflow-y: auto; }\n";
    ss << ".pattern-item { display: flex; align-items: center; gap: 10px; margin-bottom: 8px; padding: 4px 8px; border-radius: 4px; }\n";
    ss << ".pattern-item:hover { background: #f1f3f5; }\n";
    ss << ".pattern-item span { font-family: monospace; background: #e9ecef; padding: 4px 8px; border-radius: 4px; flex: 1; }\n";
    ss << ".pattern-item button { background: #dc3545; padding: 4px 12px; font-size: 12px; margin: 0; border-radius: 4px; }\n";
    ss << ".pattern-item button:hover { background: #c82333; }\n";
    ss << ".add-pattern { display: flex; gap: 10px; margin-top: 12px; flex-wrap: wrap; }\n";
    ss << ".add-pattern input { flex: 1; min-width: 150px; margin: 0; }\n";
    ss << ".add-pattern button { margin: 0; }\n";
    ss << ".results-table { width: 100%; border-collapse: collapse; margin-top: 16px; }\n";
    ss << ".results-table th, .results-table td { padding: 12px; text-align: left; border-bottom: 1px solid #dee2e6; }\n";
    ss << ".results-table th { background: #667eea; color: white; }\n";
    ss << ".results-table tr:hover { background: #f1f3f5; }\n";
    ss << ".error { background: #fee; color: #dc3545; padding: 12px; border-radius: 8px; margin-bottom: 16px; border-left: 4px solid #dc3545; }\n";
    ss << ".info { background: #e7f3ff; padding: 12px; border-radius: 8px; margin-bottom: 16px; border-left: 4px solid #667eea; }\n";
    ss << ".row { display: flex; gap: 20px; flex-wrap: wrap; }\n";
    ss << ".col { flex: 1; min-width: 250px; }\n";
    ss << ".file-status { margin-top: 8px; font-size: 12px; color: #666; }\n";
    ss << "</style>\n";
    
    // ==================== JAVASCRIPT ====================
    ss << "<script>\n";
    ss << "let isProcessing = false;\n\n";
    
    ss << "async function apiCall(endpoint, data) {\n";
    ss << "  if (isProcessing) {\n";
    ss << "    alert('Подождите, выполняется предыдущий запрос...');\n";
    ss << "    return;\n";
    ss << "  }\n";
    ss << "  isProcessing = true;\n";
    ss << "  document.querySelectorAll('button').forEach(b => b.disabled = true);\n";
    ss << "  try {\n";
    ss << "    const formData = new URLSearchParams();\n";
    ss << "    for (const [key, value] of Object.entries(data)) {\n";
    ss << "      formData.append(key, value);\n";
    ss << "    }\n";
    ss << "    const response = await fetch(endpoint, { method: 'POST', body: formData });\n";
    ss << "    if (!response.ok) throw new Error('HTTP error: ' + response.status);\n";
    ss << "    const html = await response.text();\n";
    ss << "    const parser = new DOMParser();\n";
    ss << "    const doc = parser.parseFromString(html, 'text/html');\n";
    ss << "    const newContent = doc.querySelector('.content');\n";
    ss << "    const oldContent = document.querySelector('.content');\n";
    ss << "    if (newContent && oldContent) {\n";
    ss << "      oldContent.innerHTML = newContent.innerHTML;\n";
    ss << "    } else {\n";
    ss << "      document.open(); document.write(html); document.close();\n";
    ss << "    }\n";
    ss << "  } catch (error) {\n";
    ss << "    console.error('API Error:', error);\n";
    ss << "    alert('Ошибка: ' + error.message);\n";
    ss << "  } finally {\n";
    ss << "    isProcessing = false;\n";
    ss << "    document.querySelectorAll('button').forEach(b => b.disabled = false);\n";
    ss << "  }\n";
    ss << "}\n\n";
    
    ss << "function addPattern() {\n";
    ss << "  const input = document.getElementById('new-pattern');\n";
    ss << "  const pattern = input ? input.value.trim() : '';\n";
    ss << "  if (pattern) { apiCall('/api/add-pattern', { pattern: pattern }); input.value = ''; }\n";
    ss << "  else alert('Введите подстроку');\n";
    ss << "}\n\n";
    
    ss << "function removePattern(index) {\n";
    ss << "  apiCall('/api/remove-pattern', { index: index });\n";
    ss << "}\n\n";
    
    ss << "function clearPatterns() {\n";
    ss << "  if (confirm('Удалить все подстроки?')) apiCall('/api/clear-patterns', {});\n";
    ss << "}\n\n";
    
    ss << "function setExample() {\n";
    ss << "  apiCall('/api/example', {});\n";
    ss << "}\n\n";
    
    ss << "function runSearch(source) {\n";
    ss << "  const overlap = document.getElementById('allow-overlap');\n";
    ss << "  const data = { source: source, overlap: overlap ? (overlap.checked ? '1' : '0') : '0' };\n";
    ss << "  if (source === 'keyboard') {\n";
    ss << "    const text = document.getElementById('keyboard-text');\n";
    ss << "    if (text) data.text = text.value;\n";
    ss << "    apiCall('/api/search', data);\n";
    ss << "  } else if (source === 'file') {\n";
    ss << "    const fileInput = document.getElementById('file-input');\n";
    ss << "    if (!fileInput || !fileInput.files || fileInput.files.length === 0) {\n";
    ss << "      alert('Выберите файл');\n";
    ss << "      return;\n";
    ss << "    }\n";
    ss << "    const status = document.getElementById('file-status');\n";
    ss << "    if (status) status.textContent = 'Загрузка...';\n";
    ss << "    const reader = new FileReader();\n";
    ss << "    reader.onload = function(e) {\n";
    ss << "      data.text = e.target.result;\n";
    ss << "      apiCall('/api/search', data);\n";
    ss << "      if (status) status.textContent = 'Файл загружен';\n";
    ss << "    };\n";
    ss << "    reader.onerror = function() { if (status) status.textContent = 'Ошибка чтения файла'; };\n";
    ss << "    reader.readAsText(fileInput.files[0]);\n";
    ss << "  } else if (source === 'lazy') {\n";
    ss << "    const length = document.getElementById('lazy-length');\n";
    ss << "    const alphabet = document.getElementById('lazy-alphabet');\n";
    ss << "    if (length) data.length = length.value;\n";
    ss << "    if (alphabet) data.alphabet = alphabet.value;\n";
    ss << "    apiCall('/api/search', data);\n";
    ss << "  }\n";
    ss << "}\n";
    ss << "</script>\n";
    
    ss << "</head>\n";
    ss << "<body>\n";
    ss << "<div class=\"container\">\n";
    ss << "<h1>🔍 Поиск подстрок в потоке данных</h1>\n";
    ss << "<div class=\"content\">\n";
    
    if (!g_state.lastErrorMessage.empty())
        ss << "<div class=\"error\">⚠️ " << escapeHtml(g_state.lastErrorMessage) << "</div>\n";
    
    // Подстроки
    ss << "<div class=\"section\">\n";
    ss << "<h2>📝 Подстроки для поиска</h2>\n";
    ss << "<div class=\"pattern-list\">\n";
    if (g_state.patterns.GetLength() == 0)
    {
        ss << "<div style=\"color: #999; text-align: center; padding: 20px;\">Нет добавленных подстрок</div>\n";
    }
    else
    {
        for (size_t i = 0; i < g_state.patterns.GetLength(); ++i)
        {
            ss << "<div class=\"pattern-item\">\n";
            ss << "<span>\"" << escapeHtml(g_state.patterns.Get(i)) << "\"</span>\n";
            ss << "<button onclick=\"removePattern(" << i << ")\">✕</button>\n";
            ss << "</div>\n";
        }
    }
    ss << "<div class=\"add-pattern\">\n";
    ss << "<input type=\"text\" id=\"new-pattern\" placeholder=\"Новая подстрока\" onkeypress=\"if(event.key==='Enter') addPattern()\">\n";
    ss << "<button onclick=\"addPattern()\">➕ Добавить</button>\n";
    ss << "<button onclick=\"clearPatterns()\" style=\"background:#6c757d\">🗑️ Очистить все</button>\n";
    ss << "<button onclick=\"setExample()\" style=\"background:#28a745\">📋 Пример</button>\n";
    ss << "</div>\n";
    ss << "</div>\n";
    ss << "</div>\n";
    
    // Источник данных
    ss << "<div class=\"section\">\n";
    ss << "<h2>📂 Источник данных</h2>\n";
    ss << "<div class=\"row\">\n";
    
    ss << "<div class=\"col\">\n";
    ss << "<h3>⌨️ Ручной ввод</h3>\n";
    ss << "<textarea id=\"keyboard-text\" rows=\"4\" placeholder=\"Введите текст...\">" << escapeHtml(g_state.sourceText) << "</textarea>\n";
    ss << "<button onclick=\"runSearch('keyboard')\">🔍 Поискать</button>\n";
    ss << "</div>\n";
    
    ss << "<div class=\"col\">\n";
    ss << "<h3>📄 Файл</h3>\n";
    ss << "<input type=\"file\" id=\"file-input\" accept=\".txt,.log,.csv\">\n";
    ss << "<button onclick=\"runSearch('file')\">📤 Загрузить и поискать</button>\n";
    ss << "<div class=\"file-status\" id=\"file-status\"></div>\n";
    ss << "</div>\n";
    
    ss << "<div class=\"col\">\n";
    ss << "<h3>⚡ Генерация</h3>\n";
    ss << "<label>Длина:</label>\n";
    ss << "<input type=\"number\" id=\"lazy-length\" value=\"" << g_state.lazyLength << "\" min=\"1\" max=\"10000000\">\n";
    ss << "<label>Алфавит:</label>\n";
    ss << "<input type=\"text\" id=\"lazy-alphabet\" value=\"" << escapeHtml(g_state.lazyAlphabet) << "\" maxlength=\"26\">\n";
    ss << "<button onclick=\"runSearch('lazy')\">🎲 Сгенерировать и поискать</button>\n";
    ss << "</div>\n";
    
    ss << "</div>\n";
    
    ss << "<div style=\"margin-top: 16px;\">\n";
    ss << "<label style=\"display: inline-block; margin-right: 20px;\">\n";
    ss << "<input type=\"checkbox\" id=\"allow-overlap\"" << (g_state.allowOverlap ? " checked" : "") << ">\n";
    ss << " 🔄 Разрешить перекрывающиеся вхождения\n";
    ss << "</label>\n";
    ss << "</div>\n";
    ss << "</div>\n";
    
    if (g_state.lastResults.GetLength() > 0)
    {
        ss << "<div class=\"section\">\n";
        ss << "<h2>📊 Результаты</h2>\n";
        ss << "<div class=\"info\">📌 Обработано символов: " << g_state.processedCount << "</div>\n";
        ss << "<table class=\"results-table\">\n";
        ss << "<thead><tr><th>Подстрока</th><th>Количество</th></tr></thead>\n";
        ss << "<tbody>\n";
        for (size_t i = 0; i < g_state.lastResults.GetLength(); ++i)
        {
            std::pair<std::string, size_t> p = g_state.lastResults.Get(i);
            ss << "<tr><td>\"" << escapeHtml(p.first) << "\"</td><td>" << p.second << "</td></tr>\n";
        }
        ss << "</tbody></table></div>\n";
    }
    
    ss << "</div></div></body></html>\n";
    return ss.str();
}

// ==================== HTTP ОТВЕТЫ ====================

std::string httpResponse(int code, const std::string& contentType, const std::string& body)
{
    std::stringstream ss;
    ss << "HTTP/1.1 " << code;
    if (code == 200) ss << " OK";
    else if (code == 404) ss << " Not Found";
    else if (code == 400) ss << " Bad Request";
    ss << "\r\n";
    ss << "Content-Type: " << contentType << "\r\n";
    ss << "Content-Length: " << body.length() << "\r\n";
    ss << "Connection: close\r\n";
    ss << "\r\n";
    ss << body;
    return ss.str();
}

std::string htmlResponse(int code, const std::string& html)
{
    return httpResponse(code, "text/html; charset=utf-8", html);
}

// ==================== ОБРАБОТКА ЗАПРОСОВ ====================

void handleRequest(const std::string& request, std::string& response)
{
    StateLock lock;

    std::stringstream ss(request);
    std::string method, path, version;
    ss >> method >> path >> version;
    
    if (method == "GET" && (path == "/" || path == "/index.html"))
    {
        response = htmlResponse(200, renderMainPage());
        return;
    }
    
    if (method == "GET" && path == "/favicon.ico")
    {
        response = httpResponse(404, "text/plain", "");
        return;
    }
    
    if (method == "POST")
    {
        size_t bodyPos = request.find("\r\n\r\n");
        std::string body = (bodyPos != std::string::npos) ? request.substr(bodyPos + 4) : "";
        
        std::map<std::string, std::string> params;
        std::stringstream ssBody(body);
        std::string pair;
        while (std::getline(ssBody, pair, '&'))
        {
            size_t eqPos = pair.find('=');
            if (eqPos != std::string::npos)
            {
                std::string key = pair.substr(0, eqPos);
                std::string value = pair.substr(eqPos + 1);
                params[key] = urlDecode(value);
            }
        }
        
        if (path == "/api/add-pattern")
        {
            auto it = params.find("pattern");
            if (it != params.end() && !it->second.empty())
                g_state.patterns.Append(it->second);
            g_state.lastErrorMessage.clear();
        }
        else if (path == "/api/remove-pattern")
        {
            auto it = params.find("index");
            if (it != params.end())
            {
                int idx = std::stoi(it->second);
                if (idx >= 0 && static_cast<size_t>(idx) < g_state.patterns.GetLength())
                {
                    MutableArraySequence<std::string> newPatterns;
                    for (size_t i = 0; i < g_state.patterns.GetLength(); ++i)
                        if (static_cast<int>(i) != idx)
                            newPatterns.Append(g_state.patterns.Get(i));
                    g_state.patterns = newPatterns;
                }
            }
            g_state.lastErrorMessage.clear();
        }
        else if (path == "/api/clear-patterns")
        {
            g_state.patterns = MutableArraySequence<std::string>();
            g_state.lastResults = MutableArraySequence<std::pair<std::string, size_t>>();
            g_state.lastErrorMessage.clear();
        }
        else if (path == "/api/example")
        {
            g_state.patterns = MutableArraySequence<std::string>();
            g_state.patterns.Append("he");
            g_state.patterns.Append("she");
            g_state.patterns.Append("his");
            g_state.patterns.Append("hers");
            g_state.sourceText = "ushers";
            g_state.lastResults = MutableArraySequence<std::pair<std::string, size_t>>();
            g_state.lastErrorMessage.clear();
        }
        else if (path == "/api/search")
        {
            if (g_state.isProcessing)
            {
                g_state.lastErrorMessage = "Поиск уже выполняется";
                response = htmlResponse(200, renderMainPage());
                return;
            }
            
            g_state.isProcessing = true;
            
            try
            {
                std::string source = params["source"];
                bool overlap = (params["overlap"] == "1");
                g_state.allowOverlap = overlap;
                
                if (g_state.patterns.GetLength() == 0)
                {
                    g_state.lastErrorMessage = "Добавьте хотя бы одну подстроку";
                    g_state.isProcessing = false;
                    response = htmlResponse(200, renderMainPage());
                    return;
                }
                
                PatternCounter pc(g_state.patterns, overlap);
                
                if (source == "keyboard")
                {
                    std::string text = params["text"];
                    g_state.sourceText = text;
                    for (size_t i = 0; i < text.length(); ++i)
                        pc.ConsumeChar(text[i]);
                    g_state.processedCount = text.length();
                }
                else if (source == "file")
                {
                    std::string text = params["text"];
                    g_state.sourceText = text;
                    for (size_t i = 0; i < text.length(); ++i)
                        pc.ConsumeChar(text[i]);
                    g_state.processedCount = text.length();
                }
                else if (source == "lazy")
                {
                    size_t length = std::stoul(params["length"]);
                    std::string alphabet = params["alphabet"];
                    if (alphabet.empty()) alphabet = "ab";
                    
                    g_state.lazyLength = length;
                    g_state.lazyAlphabet = alphabet;
                    
                    std::string alphabetCopy = alphabet;
                    auto rule = [alphabetCopy](const BoundedQueue<char>&) -> char {
                        static std::mt19937 rng(42);
                        static std::uniform_int_distribution<size_t> dist(0, alphabetCopy.size() - 1);
                        return alphabetCopy[dist(rng)];
                    };
                    
                    MutableArraySequence<char> init;
                    init.Append(alphabet[0]);
                    Generator<char> gen(rule, init, 1, length);
                    LazySequence<char> lazy(gen);
                    
                    for (size_t i = 0; i < length; ++i)
                        pc.ConsumeChar(lazy.Get(i));
                    g_state.processedCount = length;
                }
                
                Sequence<std::pair<std::string, size_t>>* results = pc.GetCounts();
                g_state.lastResults = MutableArraySequence<std::pair<std::string, size_t>>();
                for (size_t i = 0; i < results->GetLength(); ++i)
                    g_state.lastResults.Append(results->Get(i));
                delete results;
                g_state.lastErrorMessage.clear();
            }
            catch (const std::exception& e)
            {
                g_state.lastErrorMessage = e.what();
            }
            
            g_state.isProcessing = false;
        }
        
        response = htmlResponse(200, renderMainPage());
        return;
    }
    
    response = httpResponse(404, "text/html", "<h1>404 Not Found</h1>");
}

// ==================== СЕРВЕР ====================

#ifdef _WIN32
DWORD WINAPI clientHandler(LPVOID arg)
#else
void* clientHandler(void* arg)
#endif
{
    SOCKET clientSocket = *(SOCKET*)arg;
    delete (SOCKET*)arg;
    
    char buffer[65536];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead > 0)
    {
        buffer[bytesRead] = '\0';
        std::string request(buffer);
        std::string response;
        
        try
        {
            handleRequest(request, response);
            send(clientSocket, response.c_str(), response.length(), 0);
        }
        catch (const std::exception& e)
        {
            std::string error = htmlResponse(500, "<h1>500 Internal Server Error</h1><p>" + std::string(e.what()) + "</p>");
            send(clientSocket, error.c_str(), error.length(), 0);
        }
    }
    
    closesocket(clientSocket);
    return 0;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    
#ifdef _WIN32
    InitializeCriticalSection(&g_stateMutex);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }
#endif
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket" << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
    
    int opt = 1;
#ifdef _WIN32
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8081);
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind to port 8081" << std::endl;
        closesocket(serverSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
    
    if (listen(serverSocket, 10) == SOCKET_ERROR)
    {
        std::cerr << "Failed to listen" << std::endl;
        closesocket(serverSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
    
    std::cout << "========================================" << std::endl;
    std::cout << "  Сервер запущен!" << std::endl;
    std::cout << "  Откройте браузер: http://localhost:8081" << std::endl;
    std::cout << "========================================" << std::endl;
    
    while (true)
    {
        struct sockaddr_in clientAddr;
#ifdef _WIN32
        int clientLen = sizeof(clientAddr);
#else
        socklen_t clientLen = sizeof(clientAddr);
#endif
        SOCKET* clientSocket = new SOCKET;
        *clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (*clientSocket != INVALID_SOCKET)
        {
#ifdef _WIN32
            CreateThread(NULL, 0, clientHandler, clientSocket, 0, NULL);
#else
            pthread_t thread;
            pthread_create(&thread, NULL, clientHandler, clientSocket);
            pthread_detach(thread);
#endif
        }
        else
        {
            delete clientSocket;
        }
    }
    
    closesocket(serverSocket);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}