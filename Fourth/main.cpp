#include <chrono>
#include <clocale>
#include <fstream>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <cctype>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
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

std::string trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

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
    MutableArraySequence<std::pair<std::string, size_t> > lastResults;
    std::string lastErrorMessage;
    
    AppState() : allowOverlap(true), lazyLength(1000000), lazyAlphabet("ab"), processedCount(0) {}
};

static AppState g_state;

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
    ss << "textarea { font-family: monospace; resize: vertical; }\n";
    ss << "button { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; padding: 10px 20px; border-radius: 8px; cursor: pointer; font-weight: 600; margin-right: 10px; margin-bottom: 10px; }\n";
    ss << "button:hover { transform: translateY(-1px); }\n";
    ss << ".pattern-list { background: white; border-radius: 12px; padding: 16px; margin-top: 12px; }\n";
    ss << ".pattern-item { display: flex; align-items: center; gap: 10px; margin-bottom: 8px; }\n";
    ss << ".pattern-item span { font-family: monospace; background: #e9ecef; padding: 4px 8px; border-radius: 4px; }\n";
    ss << ".pattern-item button { background: #dc3545; padding: 4px 12px; font-size: 12px; margin: 0; }\n";
    ss << ".add-pattern { display: flex; gap: 10px; margin-top: 12px; }\n";
    ss << ".add-pattern input { flex: 1; margin: 0; }\n";
    ss << ".add-pattern button { margin: 0; }\n";
    ss << ".results-table { width: 100%; border-collapse: collapse; margin-top: 16px; }\n";
    ss << ".results-table th, .results-table td { padding: 12px; text-align: left; border-bottom: 1px solid #dee2e6; }\n";
    ss << ".results-table th { background: #667eea; color: white; }\n";
    ss << ".results-table tr:hover { background: #f1f3f5; }\n";
    ss << ".error { background: #fee; color: #dc3545; padding: 12px; border-radius: 8px; margin-bottom: 16px; border-left: 4px solid #dc3545; }\n";
    ss << ".success { background: #e8f4e8; color: #28a745; padding: 12px; border-radius: 8px; margin-bottom: 16px; border-left: 4px solid #28a745; }\n";
    ss << ".info { background: #e7f3ff; padding: 12px; border-radius: 8px; margin-bottom: 16px; border-left: 4px solid #667eea; }\n";
    ss << ".row { display: flex; gap: 20px; flex-wrap: wrap; }\n";
    ss << ".col { flex: 1; min-width: 250px; }\n";
    ss << "hr { margin: 20px 0; border: none; border-top: 1px solid #dee2e6; }\n";
    ss << "</style>\n";
    ss << "<script>\n";
    ss << "async function apiCall(endpoint, data) {\n";
    ss << "  const formData = new URLSearchParams();\n";
    ss << "  for (const [key, value] of Object.entries(data)) formData.append(key, value);\n";
    ss << "  const response = await fetch(endpoint, { method: 'POST', body: formData });\n";
    ss << "  const html = await response.text();\n";
    ss << "  document.open();\n";
    ss << "  document.write(html);\n";
    ss << "  document.close();\n";
    ss << "}\n";
    ss << "function addPattern() {\n";
    ss << "  const input = document.getElementById('new-pattern');\n";
    ss << "  const pattern = input.value.trim();\n";
    ss << "  if (pattern) apiCall('/api/add-pattern', { pattern: pattern });\n";
    ss << "  else alert('Введите подстроку');\n";
    ss << "}\n";
    ss << "function removePattern(index) {\n";
    ss << "  apiCall('/api/remove-pattern', { index: index });\n";
    ss << "}\n";
    ss << "function clearPatterns() {\n";
    ss << "  if (confirm('Удалить все подстроки?')) apiCall('/api/clear-patterns', {});\n";
    ss << "}\n";
    ss << "function runSearch(source) {\n";
    ss << "  let data = { source: source };\n";
    ss << "  if (source === 'keyboard') data.text = document.getElementById('keyboard-text').value;\n";
    ss << "  if (source === 'file') data.path = document.getElementById('file-path').value;\n";
    ss << "  if (source === 'lazy') {\n";
    ss << "    data.length = document.getElementById('lazy-length').value;\n";
    ss << "    data.alphabet = document.getElementById('lazy-alphabet').value;\n";
    ss << "  }\n";
    ss << "  data.overlap = document.getElementById('allow-overlap').checked ? '1' : '0';\n";
    ss << "  apiCall('/api/search', data);\n";
    ss << "}\n";
    ss << "function setExample() {\n";
    ss << "  document.getElementById('keyboard-text').value = 'ushers';\n";
    ss << "  apiCall('/api/add-pattern', { pattern: 'he' });\n";
    ss << "  setTimeout(() => apiCall('/api/add-pattern', { pattern: 'she' }), 100);\n";
    ss << "  setTimeout(() => apiCall('/api/add-pattern', { pattern: 'his' }), 200);\n";
    ss << "  setTimeout(() => apiCall('/api/add-pattern', { pattern: 'hers' }), 300);\n";
    ss << "}\n";
    ss << "</script>\n";
    ss << "</head>\n";
    ss << "<body>\n";
    ss << "<div class=\"container\">\n";
    ss << "<h1>?? Поиск подстрок в потоке данных</h1>\n";
    ss << "<div class=\"content\">\n";
    
    // Сообщения
    if (!g_state.lastErrorMessage.empty())
        ss << "<div class=\"error\">? " << escapeHtml(g_state.lastErrorMessage) << "</div>\n";
    
    // Подстроки
    ss << "<div class=\"section\">\n";
    ss << "<h2>?? Подстроки для поиска</h2>\n";
    ss << "<div class=\"pattern-list\">\n";
    for (size_t i = 0; i < g_state.patterns.GetLength(); ++i)
    {
        ss << "<div class=\"pattern-item\">\n";
        ss << "<span>\"" << escapeHtml(g_state.patterns.Get(i)) << "\"</span>\n";
        ss << "<button onclick=\"removePattern(" << i << ")\">? Удалить</button>\n";
        ss << "</div>\n";
    }
    ss << "<div class=\"add-pattern\">\n";
    ss << "<input type=\"text\" id=\"new-pattern\" placeholder=\"Новая подстрока\">\n";
    ss << "<button onclick=\"addPattern()\">? Добавить</button>\n";
    ss << "<button onclick=\"clearPatterns()\" style=\"background:#6c757d\">?? Очистить все</button>\n";
    ss << "<button onclick=\"setExample()\" style=\"background:#28a745\">?? Пример (ushers)</button>\n";
    ss << "</div>\n";
    ss << "</div>\n";
    ss << "</div>\n";
    
    // Источник данных
    ss << "<div class=\"section\">\n";
    ss << "<h2>?? Источник данных</h2>\n";
    ss << "<div class=\"row\">\n";
    
    // Клавиатура
    ss << "<div class=\"col\">\n";
    ss << "<h3>?? Ручной ввод</h3>\n";
    ss << "<textarea id=\"keyboard-text\" rows=\"4\" placeholder=\"Введите текст для поиска...\">" << escapeHtml(g_state.sourceText) << "</textarea>\n";
    ss << "<button onclick=\"runSearch('keyboard')\">?? Поискать</button>\n";
    ss << "</div>\n";
    
    // Файл
    ss << "<div class=\"col\">\n";
    ss << "<h3>?? Файл</h3>\n";
    ss << "<input type=\"text\" id=\"file-path\" placeholder=\"Путь к файлу\" value=\"" << escapeHtml(g_state.filePath) << "\">\n";
    ss << "<button onclick=\"runSearch('file')\">?? Открыть и поискать</button>\n";
    ss << "</div>\n";
    
    // Генерация
    ss << "<div class=\"col\">\n";
    ss << "<h3>?? Генерация (ленивая последовательность)</h3>\n";
    ss << "<label>Длина:</label>\n";
    ss << "<input type=\"number\" id=\"lazy-length\" value=\"" << g_state.lazyLength << "\" min=\"1\" max=\"10000000\">\n";
    ss << "<label>Алфавит:</label>\n";
    ss << "<input type=\"text\" id=\"lazy-alphabet\" value=\"" << escapeHtml(g_state.lazyAlphabet) << "\" maxlength=\"26\">\n";
    ss << "<button onclick=\"runSearch('lazy')\">?? Сгенерировать и поискать</button>\n";
    ss << "</div>\n";
    
    ss << "</div>\n";
    
    // Настройки
    ss << "<div style=\"margin-top: 16px;\">\n";
    ss << "<label style=\"display: inline-block; margin-right: 20px;\">\n";
    ss << "<input type=\"checkbox\" id=\"allow-overlap\"" << (g_state.allowOverlap ? " checked" : "") << ">\n";
    ss << " Разрешить перекрывающиеся вхождения\n";
    ss << "</label>\n";
    ss << "</div>\n";
    ss << "</div>\n";
    
    // Результаты
    if (g_state.lastResults.GetLength() > 0)
    {
        ss << "<div class=\"section\">\n";
        ss << "<h2>?? Результаты поиска</h2>\n";
        ss << "<div class=\"info\">?? Обработано символов: " << g_state.processedCount << "</div>\n";
        ss << "<table class=\"results-table\">\n";
        ss << "<thead>\n<tr><th>Подстрока</th><th>Количество вхождений</th></tr>\n</thead>\n";
        ss << "<tbody>\n";
        for (size_t i = 0; i < g_state.lastResults.GetLength(); ++i)
        {
            std::pair<std::string, size_t> p = g_state.lastResults.Get(i);
            ss << "<tr><td>\"" << escapeHtml(p.first) << "\"</td><td>" << p.second << "</td></tr>\n";
        }
        ss << "</tbody>\n";
        ss << "</table>\n";
        ss << "</div>\n";
    }
    
    ss << "</div>\n";
    ss << "</div>\n";
    ss << "</body>\n";
    ss << "</html>\n";
    
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
    std::stringstream ss(request);
    std::string method, path, version;
    ss >> method >> path >> version;
    
    // GET запросы
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
    
    // POST запросы
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
                // URL decode
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
                params[key] = decoded;
            }
        }
        
        if (path == "/api/add-pattern")
        {
            std::map<std::string, std::string>::iterator it = params.find("pattern");
            if (it != params.end() && !it->second.empty())
                g_state.patterns.Append(it->second);
            g_state.lastErrorMessage.clear();
        }
        else if (path == "/api/remove-pattern")
        {
            std::map<std::string, std::string>::iterator it = params.find("index");
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
            g_state.lastErrorMessage.clear();
        }
        else if (path == "/api/search")
        {
            std::string source = params["source"];
            bool overlap = (params["overlap"] == "1");
            g_state.allowOverlap = overlap;
            
            if (g_state.patterns.GetLength() == 0)
            {
                g_state.lastErrorMessage = "Добавьте хотя бы одну подстроку для поиска";
                response = htmlResponse(200, renderMainPage());
                return;
            }
            
            try
            {
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
                    std::string pathStr = params["path"];
                    g_state.filePath = pathStr;
                    
                    std::ifstream file(pathStr.c_str(), std::ios::binary);
                    if (!file.is_open())
                    {
                        g_state.lastErrorMessage = "Не удалось открыть файл: " + pathStr;
                        response = htmlResponse(200, renderMainPage());
                        return;
                    }
                    
                    char c;
                    while (file.get(c))
                        pc.ConsumeChar(c);
                    g_state.processedCount = pc.GetProcessedCount();
                }
                else if (source == "lazy")
                {
                    size_t length = std::stoul(params["length"]);
                    std::string alphabet = params["alphabet"];
                    if (alphabet.empty()) alphabet = "ab";
                    
                    g_state.lazyLength = length;
                    g_state.lazyAlphabet = alphabet;
                    
                    // Простой генератор без захвата алфавита по ссылке
                    std::string alphabetCopy = alphabet;
                    auto rule = [alphabetCopy](const BoundedQueue<char>&) -> char {
                        static std::mt19937 rng(42);
                        std::uniform_int_distribution<size_t> dist(0, alphabetCopy.size() - 1);
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
                
                Sequence<std::pair<std::string, size_t> >* results = pc.GetCounts();
                
                // Копируем результаты вручную
                g_state.lastResults = MutableArraySequence<std::pair<std::string, size_t> >();
                for (size_t i = 0; i < results->GetLength(); ++i)
                    g_state.lastResults.Append(results->Get(i));
                
                delete results;
                g_state.lastErrorMessage.clear();
            }
            catch (const std::exception& e)
            {
                g_state.lastErrorMessage = e.what();
            }
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
    serverAddr.sin_port = htons(8080);
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Failed to bind to port 8080" << std::endl;
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
    std::cout << "  Откройте браузер: http://localhost:8080" << std::endl;
    std::cout << "========================================" << std::endl;
    
    while (true)
    {
        struct sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
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