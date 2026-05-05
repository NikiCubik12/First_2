// server.cpp - полностью исправленная версия
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include <fstream>
#include <vector>
#include <climits>
#include <cctype>

// Windows сокеты
#include <winsock2.h>
#include <ws2tcpip.h>

// Подключаем ваши заголовочные файлы
#include "../DynamicArray.hpp"
#include "../LinkedList.hpp"
#include "../Sequence.hpp"
#include "../ArraySequence.hpp"
#include "../MutableArraySequence.hpp"
#include "../ImmutableArraySequence.hpp"
#include "../ListSequence.hpp"
#include "../BitSequence.hpp"

using namespace std;

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

bool safeStoi(const string& str, int& result) {
    try {
        if (str.length() > 11) return false;
        
        size_t start = 0;
        if (str[0] == '-') start = 1;
        for (size_t i = start; i < str.length(); i++) {
            if (!isdigit(str[i])) return false;
        }
        
        size_t pos = 0;
        long long val = stoll(str, &pos);
        
        if (pos != str.length() || val < INT_MIN || val > INT_MAX) {
            return false;
        }
        
        result = static_cast<int>(val);
        return true;
    } catch (...) {
        return false;
    }
}

bool safeIndex(const string& str, int& result, int maxSize) {
    if (!safeStoi(str, result)) return false;
    if (result < 0 || result > maxSize) return false;
    return true;
}

// ==================== ГЛОБАЛЬНЫЕ ДАННЫЕ ====================

struct AppData {
    DynamicArray<int> dynamicArray;
    bool dynamicArrayCreated = false;
    
    LinkedList<int> linkedList;
    bool linkedListCreated = false;
    
    Sequence<int>* intSequence = nullptr;
    BitSequence* bitSequence = nullptr;
    bool sequenceCreated = false;
    string sequenceType = "ArraySequence";
    string arrayImpl = "Mutable";
    
    AppData() {
        intSequence = new MutableArraySequence<int>();
    }
    
    ~AppData() {
        if (intSequence) delete intSequence;
        if (bitSequence) delete bitSequence;
    }
};

AppData g_appData;

// ==================== РАБОТА С ФАЙЛАМИ ====================

string readFile(const string& path) {
    vector<string> paths = {
        "../frontend/" + path,
        "./frontend/" + path,
        path
    };
    
    for (const auto& p : paths) {
        ifstream file(p);
        if (file.is_open()) {
            string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
            file.close();
            return content;
        }
    }
    return "";
}

// ==================== HTTP ОТВЕТЫ ====================

string response(int code, const string& contentType, const string& body) {
    stringstream ss;
    ss << "HTTP/1.1 " << code;
    if (code == 200) ss << " OK";
    else if (code == 404) ss << " Not Found";
    ss << "\r\n";
    ss << "Content-Type: " << contentType << "\r\n";
    ss << "Access-Control-Allow-Origin: *\r\n";
    ss << "Content-Length: " << body.length() << "\r\n";
    ss << "Connection: close\r\n";
    ss << "\r\n";
    ss << body;
    return ss.str();
}

string responseJson(int code, const string& jsonBody) {
    return response(code, "application/json", jsonBody);
}

string successJson(const string& message, const string& additional = "") {
    string json = "{\"success\":true,\"message\":\"" + message + "\"";
    if (!additional.empty()) {
        json += "," + additional;
    }
    json += "}";
    return json;
}

string errorJson(const string& message) {
    return "{\"success\":false,\"message\":\"" + message + "\"}";
}

// ==================== GET STATE ====================

string getStateJson() {
    stringstream ss;
    ss << "{";
    ss << "\"dynamicArrayCreated\":" << (g_appData.dynamicArrayCreated ? "true" : "false") << ",";
    ss << "\"dynamicArraySize\":" << (g_appData.dynamicArrayCreated ? g_appData.dynamicArray.GetSize() : 0) << ",";
    
    ss << "\"dynamicArrayValues\":[";
    if (g_appData.dynamicArrayCreated && g_appData.dynamicArray.GetSize() > 0) {
        for (int i = 0; i < g_appData.dynamicArray.GetSize(); i++) {
            if (i > 0) ss << ",";
            ss << g_appData.dynamicArray.Get(i);
        }
    }
    ss << "],";
    
    ss << "\"linkedListCreated\":" << (g_appData.linkedListCreated ? "true" : "false") << ",";
    ss << "\"linkedListSize\":" << (g_appData.linkedListCreated ? g_appData.linkedList.GetLength() : 0) << ",";
    
    ss << "\"linkedListValues\":[";
    if (g_appData.linkedListCreated && g_appData.linkedList.GetLength() > 0) {
        for (int i = 0; i < g_appData.linkedList.GetLength(); i++) {
            if (i > 0) ss << ",";
            ss << g_appData.linkedList.Get(i);
        }
    }
    ss << "],";
    
    ss << "\"sequenceCreated\":" << (g_appData.sequenceCreated ? "true" : "false") << ",";
    ss << "\"sequenceType\":\"" << g_appData.sequenceType << "\",";
    ss << "\"sequenceImpl\":\"" << g_appData.arrayImpl << "\",";
    
    int seqSize = 0;
    if (g_appData.sequenceCreated) {
        if (g_appData.sequenceType == "BitSequence") {
            seqSize = g_appData.bitSequence->GetLength();
        } else {
            seqSize = g_appData.intSequence->GetLength();
        }
    }
    ss << "\"sequenceSize\":" << seqSize << ",";
    
    ss << "\"sequenceValues\":[";
    if (g_appData.sequenceCreated && seqSize > 0) {
        if (g_appData.sequenceType == "BitSequence") {
            for (int i = 0; i < seqSize; i++) {
                if (i > 0) ss << ",";
                ss << g_appData.bitSequence->Get(i);
            }
        } else {
            for (int i = 0; i < seqSize; i++) {
                if (i > 0) ss << ",";
                ss << g_appData.intSequence->Get(i);
            }
        }
    }
    ss << "]";
    ss << "}";
    return ss.str();
}

// ==================== ОБРАБОТЧИКИ API ====================

void handleDynamicArray(const map<string, string>& params, string& jsonResponse) {
    try {
        auto it = params.find("action");
        if (it == params.end()) {
            jsonResponse = errorJson("Missing 'action' parameter");
            return;
        }
        string action = it->second;
        
        if (action == "create") {
            g_appData.dynamicArray.Resize(0);
            g_appData.dynamicArrayCreated = true;
            jsonResponse = successJson("Dynamic array created");
        }
        else if (action == "append") {
            if (!g_appData.dynamicArrayCreated) throw runtime_error("Create array first");
            auto valIt = params.find("value");
            if (valIt == params.end()) throw runtime_error("Missing value");
            int value;
            if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
            g_appData.dynamicArray.Append(value);
            jsonResponse = successJson("Value appended");
        }
        else if (action == "prepend") {
            if (!g_appData.dynamicArrayCreated) throw runtime_error("Create array first");
            auto valIt = params.find("value");
            if (valIt == params.end()) throw runtime_error("Missing value");
            int value;
            if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
            g_appData.dynamicArray.Prepend(value);
            jsonResponse = successJson("Value prepended");
        }
        else if (action == "insert") {
            if (!g_appData.dynamicArrayCreated) throw runtime_error("Create array first");
            auto idxIt = params.find("index");
            auto valIt = params.find("value");
            if (idxIt == params.end() || valIt == params.end()) throw runtime_error("Missing index or value");
            int index, value;
            if (!safeIndex(idxIt->second, index, g_appData.dynamicArray.GetSize())) throw runtime_error("Invalid index");
            if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
            g_appData.dynamicArray.InsertAt(value, index);
            jsonResponse = successJson("Value inserted");
        }
        else if (action == "set") {
            if (!g_appData.dynamicArrayCreated) throw runtime_error("Create array first");
            auto idxIt = params.find("index");
            auto valIt = params.find("value");
            if (idxIt == params.end() || valIt == params.end()) throw runtime_error("Missing index or value");
            int index, value;
            if (!safeIndex(idxIt->second, index, g_appData.dynamicArray.GetSize() - 1)) throw runtime_error("Invalid index");
            if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
            g_appData.dynamicArray.Set(index, value);
            jsonResponse = successJson("Value set");
        }
        else if (action == "get") {
            if (!g_appData.dynamicArrayCreated) throw runtime_error("Create array first");
            auto idxIt = params.find("index");
            if (idxIt == params.end()) throw runtime_error("Missing index");
            int index;
            if (!safeIndex(idxIt->second, index, g_appData.dynamicArray.GetSize() - 1)) throw runtime_error("Invalid index");
            int value = g_appData.dynamicArray.Get(index);
            jsonResponse = successJson("Value: " + to_string(value), "\"value\":" + to_string(value));
        }
        else if (action == "clear") {
            if (g_appData.dynamicArrayCreated) {
                g_appData.dynamicArray.Resize(0);
            }
            jsonResponse = successJson("Array cleared");
        }
        else {
            jsonResponse = errorJson("Unknown action: " + action);
        }
    }
    catch (const exception& e) {
        jsonResponse = errorJson(e.what());
    }
}

void handleLinkedList(const map<string, string>& params, string& jsonResponse) {
    try {
        auto it = params.find("action");
        if (it == params.end()) {
            jsonResponse = errorJson("Missing 'action' parameter");
            return;
        }
        string action = it->second;
        
        if (action == "create") {
            g_appData.linkedList.ClearList();
            g_appData.linkedListCreated = true;
            jsonResponse = successJson("Linked list created");
        }
        else if (action == "append") {
            if (!g_appData.linkedListCreated) throw runtime_error("Create list first");
            auto valIt = params.find("value");
            if (valIt == params.end()) throw runtime_error("Missing value");
            int value;
            if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
            g_appData.linkedList.Append(value);
            jsonResponse = successJson("Value appended");
        }
        else if (action == "prepend") {
            if (!g_appData.linkedListCreated) throw runtime_error("Create list first");
            int value;
            auto valIt = params.find("value");
            if (valIt == params.end()) throw runtime_error("Missing value");
            if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
            g_appData.linkedList.Prepend(value);
            jsonResponse = successJson("Value prepended");
        }
        else if (action == "insert") {
            if (!g_appData.linkedListCreated) throw runtime_error("Create list first");
            auto idxIt = params.find("index");
            auto valIt = params.find("value");
            if (idxIt == params.end() || valIt == params.end()) throw runtime_error("Missing index or value");
            int index, value;
            if (!safeIndex(idxIt->second, index, g_appData.linkedList.GetLength())) throw runtime_error("Invalid index");
            if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
            g_appData.linkedList.InsertAt(value, index);
            jsonResponse = successJson("Value inserted");
        }
        else if (action == "get") {
            if (!g_appData.linkedListCreated) throw runtime_error("Create list first");
            auto idxIt = params.find("index");
            if (idxIt == params.end()) throw runtime_error("Missing index");
            int index;
            if (!safeIndex(idxIt->second, index, g_appData.linkedList.GetLength() - 1)) throw runtime_error("Invalid index");
            int value = g_appData.linkedList.Get(index);
            jsonResponse = successJson("Value: " + to_string(value), "\"value\":" + to_string(value));
        }
        else if (action == "getFirst") {
            if (!g_appData.linkedListCreated) throw runtime_error("Create list first");
            int value = g_appData.linkedList.GetFirst();
            jsonResponse = successJson("First value: " + to_string(value), "\"value\":" + to_string(value));
        }
        else if (action == "getLast") {
            if (!g_appData.linkedListCreated) throw runtime_error("Create list first");
            int value = g_appData.linkedList.GetLast();
            jsonResponse = successJson("Last value: " + to_string(value), "\"value\":" + to_string(value));
        }
        else if (action == "clear") {
            if (g_appData.linkedListCreated) {
                g_appData.linkedList.ClearList();
            }
            jsonResponse = successJson("List cleared");
        }
        else {
            jsonResponse = errorJson("Unknown action: " + action);
        }
    }
    catch (const exception& e) {
        jsonResponse = errorJson(e.what());
    }
}

void handleSequence(const map<string, string>& params, string& jsonResponse) {
    try {
        auto it = params.find("action");
        if (it == params.end()) {
            jsonResponse = errorJson("Missing 'action' parameter");
            return;
        }
        string action = it->second;
        
        if (action == "create") {
            if (g_appData.intSequence) delete g_appData.intSequence;
            if (g_appData.bitSequence) delete g_appData.bitSequence;
            
            auto typeIt = params.find("type");
            if (typeIt == params.end()) throw runtime_error("Missing type");
            g_appData.sequenceType = typeIt->second;
            
            if (g_appData.sequenceType == "ArraySequence") {
                auto implIt = params.find("impl");
                if (implIt == params.end()) throw runtime_error("Missing impl for ArraySequence");
                g_appData.arrayImpl = implIt->second;
                if (g_appData.arrayImpl == "Mutable") {
                    g_appData.intSequence = new MutableArraySequence<int>();
                } else {
                    g_appData.intSequence = new ImmutableArraySequence<int>();
                }
                g_appData.sequenceCreated = true;
            }
            else if (g_appData.sequenceType == "ListSequence") {
                g_appData.intSequence = new ListSequence<int>();
                g_appData.sequenceCreated = true;
            }
            else if (g_appData.sequenceType == "BitSequence") {
                g_appData.bitSequence = new BitSequence();
                g_appData.sequenceCreated = true;
            }
            jsonResponse = successJson("Sequence created");
        }
        else if (action == "append") {
            if (!g_appData.sequenceCreated) throw runtime_error("Create sequence first");
            auto valIt = params.find("value");
            if (valIt == params.end()) throw runtime_error("Missing value");
            
            if (g_appData.sequenceType == "BitSequence") {
                int value;
                if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
                if (value != 0 && value != 1) throw runtime_error("BitSequence accepts only 0 or 1");
                g_appData.bitSequence->AppendImpl(value);
            } else {
                int value;
                if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
                g_appData.intSequence->Append(value);
            }
            jsonResponse = successJson("Value appended");
        }
        else if (action == "prepend") {
            if (!g_appData.sequenceCreated) throw runtime_error("Create sequence first");
            auto valIt = params.find("value");
            if (valIt == params.end()) throw runtime_error("Missing value");
            
            if (g_appData.sequenceType == "BitSequence") {
                int value;
                if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
                if (value != 0 && value != 1) throw runtime_error("BitSequence accepts only 0 or 1");
                g_appData.bitSequence->PrependImpl(value);
            } else {
                int value;
                if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
                g_appData.intSequence->Prepend(value);
            }
            jsonResponse = successJson("Value prepended");
        }
        else if (action == "insert") {
            if (!g_appData.sequenceCreated) throw runtime_error("Create sequence first");
            auto idxIt = params.find("index");
            auto valIt = params.find("value");
            if (idxIt == params.end() || valIt == params.end()) throw runtime_error("Missing index or value");
            
            int currentSize = (g_appData.sequenceType == "BitSequence") ? 
                              g_appData.bitSequence->GetLength() : 
                              g_appData.intSequence->GetLength();
            int index;
            if (!safeIndex(idxIt->second, index, currentSize)) throw runtime_error("Invalid index");
            
            if (g_appData.sequenceType == "BitSequence") {
                int value;
                if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
                if (value != 0 && value != 1) throw runtime_error("BitSequence accepts only 0 or 1");
                g_appData.bitSequence->InsertAtImpl(value, index);
            } else {
                int value;
                if (!safeStoi(valIt->second, value)) throw runtime_error("Invalid number");
                g_appData.intSequence->InsertAt(value, index);
            }
            jsonResponse = successJson("Value inserted");
        }
        else if (action == "get") {
            if (!g_appData.sequenceCreated) throw runtime_error("Create sequence first");
            auto idxIt = params.find("index");
            if (idxIt == params.end()) throw runtime_error("Missing index");
            
            int currentSize = (g_appData.sequenceType == "BitSequence") ? 
                              g_appData.bitSequence->GetLength() : 
                              g_appData.intSequence->GetLength();
            int index;
            if (!safeIndex(idxIt->second, index, currentSize - 1)) throw runtime_error("Invalid index");
            
            int value;
            if (g_appData.sequenceType == "BitSequence") {
                value = g_appData.bitSequence->Get(index);
            } else {
                value = g_appData.intSequence->Get(index);
            }
            jsonResponse = successJson("Value: " + to_string(value), "\"value\":" + to_string(value));
        }
        else if (action == "getFirst") {
            if (!g_appData.sequenceCreated) throw runtime_error("Create sequence first");
            int value;
            if (g_appData.sequenceType == "BitSequence") {
                value = g_appData.bitSequence->GetFirst();
            } else {
                value = g_appData.intSequence->GetFirst();
            }
            jsonResponse = successJson("First value: " + to_string(value), "\"value\":" + to_string(value));
        }
        else if (action == "getLast") {
            if (!g_appData.sequenceCreated) throw runtime_error("Create sequence first");
            int value;
            if (g_appData.sequenceType == "BitSequence") {
                value = g_appData.bitSequence->GetLast();
            } else {
                value = g_appData.intSequence->GetLast();
            }
            jsonResponse = successJson("Last value: " + to_string(value), "\"value\":" + to_string(value));
        }
        else {
            jsonResponse = errorJson("Unknown action: " + action);
        }
    }
    catch (const exception& e) {
        jsonResponse = errorJson(e.what());
    }
}

// ==================== ПАРСИНГ POST BODY ====================

map<string, string> parsePostBody(const string& body) {
    map<string, string> params;
    stringstream ss(body);
    string pair;
    while (getline(ss, pair, '&')) {
        size_t eqPos = pair.find('=');
        if (eqPos != string::npos) {
            string key = pair.substr(0, eqPos);
            string value = pair.substr(eqPos + 1);
            // Декодируем URL (просто заменяем + на пробел)
            for (char& c : value) {
                if (c == '+') c = ' ';
            }
            params[key] = value;
        }
    }
    return params;
}

// ==================== ОБРАБОТКА ЗАПРОСОВ ====================

void handleRequest(const string& request, string& responseStr) {
    stringstream ss(request);
    string method, path, version;
    ss >> method >> path >> version;
    
    if (method == "OPTIONS") {
        responseStr = response(200, "text/plain", "");
        return;
    }
    
    if (method == "GET" && path == "/state") {
        responseStr = responseJson(200, getStateJson());
        return;
    }
    
    if (method == "GET" && (path == "/" || path == "/index.html")) {
        string content = readFile("index.html");
        if (content.empty()) {
            responseStr = response(404, "text/html", "<h1>404 - index.html not found</h1>");
        } else {
            responseStr = response(200, "text/html", content);
        }
        return;
    }
    
    if (method == "GET" && (path.find(".css") != string::npos || path.find(".js") != string::npos)) {
        string filename = path.substr(1);
        string content = readFile(filename);
        if (content.empty()) {
            responseStr = response(404, "text/plain", "File not found: " + filename);
        } else {
            string contentType = (path.find(".css") != string::npos) ? "text/css" : "application/javascript";
            responseStr = response(200, contentType, content);
        }
        return;
    }
    
    if (method == "POST") {
        // Находим body запроса
        size_t bodyPos = request.find("\r\n\r\n");
        string body;
        if (bodyPos != string::npos) {
            body = request.substr(bodyPos + 4);
        }
        
        map<string, string> params = parsePostBody(body);
        
        if (path == "/api/dynamicarray") {
            string jsonResponse;
            handleDynamicArray(params, jsonResponse);
            responseStr = responseJson(200, jsonResponse);
            return;
        }
        
        if (path == "/api/linkedlist") {
            string jsonResponse;
            handleLinkedList(params, jsonResponse);
            responseStr = responseJson(200, jsonResponse);
            return;
        }
        
        if (path == "/api/sequence") {
            string jsonResponse;
            handleSequence(params, jsonResponse);
            responseStr = responseJson(200, jsonResponse);
            return;
        }
        
        responseStr = responseJson(404, errorJson("API endpoint not found"));
        return;
    }
    
    responseStr = response(404, "text/plain", "Not found: " + path);
}

// ==================== ОБРАБОТЧИК КЛИЕНТОВ ====================

DWORD WINAPI clientHandler(LPVOID arg) {
    SOCKET clientSocket = *(SOCKET*)arg;
    delete (SOCKET*)arg;
    
    char buffer[32768];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        string request(buffer);
        string responseStr;
        handleRequest(request, responseStr);
        send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
    }
    
    closesocket(clientSocket);
    return 0;
}

// ==================== MAIN ====================

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Failed to create socket" << endl;
        WSACleanup();
        return 1;
    }
    
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Failed to bind to port 8080" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    if (listen(serverSocket, 10) == SOCKET_ERROR) {
        cerr << "Failed to listen" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "========================================" << endl;
    cout << "  Web Server Started!" << endl;
    cout << "  Open browser at: http://localhost:8080" << endl;
    cout << "  Press Ctrl+C to stop" << endl;
    cout << "========================================" << endl;
    
    while (true) {
        struct sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        SOCKET* clientSocket = new SOCKET;
        *clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (*clientSocket == INVALID_SOCKET) {
            delete clientSocket;
            continue;
        }
        
        CreateThread(NULL, 0, clientHandler, clientSocket, 0, NULL);
    }
    
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}