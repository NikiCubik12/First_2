#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include <fstream>
#include <climits>
#include <cctype>
#include <memory>
#include <vector>
#include <regex>
#include <algorithm>
#include <utility>

// Windows сокеты
#include <winsock2.h>
#include <ws2tcpip.h>

// Подключаем заголовочные файлы
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "ArraySequence.hpp"
#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ListSequence.hpp"
#include "BitSequence.hpp"
#include "Functional.hpp"
#include "RectangularMatrix.hpp"
#include "Complex.hpp"

using namespace std;

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

bool safeStoi(const string& str, int& result) 
{
    if (str.empty()) return false;
    try 
    {
        size_t pos = 0;
        long long val = stoll(str, &pos);
        if (pos != str.length() || val < INT_MIN || val > INT_MAX) return false;
        result = static_cast<int>(val);
        return true;
    } 
    catch (...) 
    {
        return false;
    }
}

bool safeSizeT(const string& str, size_t& result, size_t maxSize) 
{
    int temp;
    if (!safeStoi(str, temp)) return false;
    if (temp < 0 || static_cast<size_t>(temp) > maxSize) return false;
    result = static_cast<size_t>(temp);
    return true;
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

bool hasSpecialChars(const string& str) {
    static const string specialChars = "!@#$%^&*()_+={}[]|\\:;\"'<>?,./`~";
    for (char c : str) {
        if (specialChars.find(c) != string::npos) {
            return true;
        }
    }
    return false;
}

bool hasLetters(const string& str) {
    for (char c : str) {
        if (isalpha(static_cast<unsigned char>(c))) {
            return true;
        }
    }
    return false;
}

string validateValue(const string& value, bool isBitSequence = false) {
    string trimmed = trim(value);
    if (trimmed.empty()) {
        return "❌ Ошибка: значение не может быть пустым";
    }
    if (hasSpecialChars(trimmed)) {
        return "❌ Ошибка: значение содержит недопустимые спецсимволы";
    }
    if (hasLetters(trimmed)) {
        return "❌ Ошибка: значение содержит буквы";
    }
    int val;
    if (isBitSequence) {
        if (!safeStoi(trimmed, val) || (val != 0 && val != 1)) {
            return "❌ Ошибка: для BitSequence допустимы только 0 или 1";
        }
    } else {
        if (!safeStoi(trimmed, val)) {
            return "❌ Ошибка: значение должно быть целым числом";
        }
    }
    return "";
}

string validateIndex(const string& idx, size_t currentSize, bool allowEqual = false) {
    string trimmed = trim(idx);
    if (trimmed.empty()) {
        return "❌ Ошибка: индекс не может быть пустым";
    }
    if (hasSpecialChars(trimmed)) {
        return "❌ Ошибка: индекс содержит недопустимые спецсимволы";
    }
    if (hasLetters(trimmed)) {
        return "❌ Ошибка: индекс содержит буквы";
    }
    size_t index;
    if (!safeSizeT(trimmed, index, currentSize)) {
        if (allowEqual) {
            return "❌ Ошибка: индекс должен быть от 0 до " + to_string(currentSize);
        } else {
            if (currentSize == 0) {
                return "❌ Ошибка: последовательность пуста, нет доступных индексов";
            }
            return "❌ Ошибка: индекс должен быть от 0 до " + to_string(currentSize - 1);
        }
    }
    return "";
}

string escapeHtml(const string& s) 
{
    string result;
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

// ==================== МЕНЕДЖЕРЫ ДАННЫХ ====================

template<typename T>
class DynamicArrayManager 
{
private:
    DynamicArray<T>* data;
    bool created;
public:
    DynamicArrayManager() : data(nullptr), created(false) {}
    ~DynamicArrayManager() { if (data) delete data; }
    void create() { if (data) delete data; data = new DynamicArray<T>(); created = true; }
    void clear() { if (created && data) data->Resize(0); }
    bool isCreated() const { return created; }
    void append(const T& value) { if (created && data) data->Append(value); }
    void prepend(const T& value) { if (created && data) data->Prepend(value); }
    void insertAt(const T& value, size_t index) { if (created && data) data->InsertAt(value, index); }
    void set(size_t index, const T& value) { if (created && data) data->Set(index, value); }
    T get(size_t index) const { if (created && data) return data->Get(index); throw runtime_error("Not created"); }
    size_t getSize() const { return created && data ? data->GetSize() : 0; }
    string getValuesString() const {
        stringstream ss;
        if (created && data) {
            ss << "[";
            for (size_t i = 0; i < data->GetSize(); i++) {
                if (i > 0) ss << ", ";
                ss << data->Get(i);
            }
            ss << "]";
        }
        return ss.str();
    }
};

template<typename T>
class LinkedListManager 
{
private:
    LinkedList<T>* data;
    bool created;
public:
    LinkedListManager() : data(nullptr), created(false) {}
    ~LinkedListManager() { if (data) delete data; }
    void create() { if (data) delete data; data = new LinkedList<T>(); created = true; }
    void clear() { if (created && data) data->ClearList(); }
    bool isCreated() const { return created; }
    void append(const T& value) { if (created && data) data->Append(value); }
    void prepend(const T& value) { if (created && data) data->Prepend(value); }
    void insertAt(const T& value, size_t index) { if (created && data) data->InsertAt(value, index); }
    T get(size_t index) const { if (created && data) return data->Get(index); throw runtime_error("Not created"); }
    T getFirst() const { if (created && data) return data->GetFirst(); throw runtime_error("Not created"); }
    T getLast() const { if (created && data) return data->GetLast(); throw runtime_error("Not created"); }
    size_t getSize() const { return created && data ? data->GetLength() : 0; }
    string getValuesString() const {
        stringstream ss;
        if (created && data) {
            ss << "[";
            for (size_t i = 0; i < data->GetLength(); i++) {
                if (i > 0) ss << " → ";
                ss << data->Get(i);
            }
            ss << "]";
        }
        return ss.str();
    }
};

// Класс для управления последовательностями
class SequenceManager 
{
private:
    enum SeqType { ARRAY_MUTABLE, ARRAY_IMMUTABLE, LIST, BIT };
    
    struct SequenceData {
        Sequence<int>* intSeq;
        BitSequence* bitSeq;
        SeqType type;
        bool isBit;
        vector<Sequence<int>*> tempIntSeqs;
        vector<BitSequence*> tempBitSeqs;
        
        SequenceData() : intSeq(nullptr), bitSeq(nullptr), type(ARRAY_MUTABLE), isBit(false) {}
        ~SequenceData() { clear(); for (auto seq : tempIntSeqs) delete seq; for (auto seq : tempBitSeqs) delete seq; }
        void clear() { if (intSeq) delete intSeq; if (bitSeq) delete bitSeq; intSeq = nullptr; bitSeq = nullptr; isBit = false; }
        size_t getSize() const { if (isBit) return bitSeq ? bitSeq->GetLength() : 0; return intSeq ? intSeq->GetLength() : 0; }
        string getValuesString() const {
            stringstream ss; ss << "[";
            size_t size = getSize();
            for (size_t i = 0; i < size; i++) {
                if (i > 0) ss << ", ";
                if (isBit) ss << bitSeq->Get(i);
                else ss << intSeq->Get(i);
            }
            ss << "]";
            return ss.str();
        }
        string getTypeString() const {
            if (!intSeq && !bitSeq) return "Не создана";
            switch (type) {
                case ARRAY_MUTABLE: return "ArraySequence (Mutable)";
                case ARRAY_IMMUTABLE: return "ArraySequence (Immutable)";
                case LIST: return "ListSequence";
                case BIT: return "BitSequence";
                default: return "Unknown";
            }
        }
        string append(int value) {
            if (isBit) { if (value != 0 && value != 1) return "❌ Ошибка: для BitSequence допустимы только 0 или 1"; if (bitSeq) bitSeq->Append(value); }
            else { if (intSeq) intSeq->Append(value); }
            return "";
        }
        string prepend(int value) {
            if (isBit) { if (value != 0 && value != 1) return "❌ Ошибка: для BitSequence допустимы только 0 или 1"; if (bitSeq) bitSeq->Prepend(value); }
            else { if (intSeq) intSeq->Prepend(value); }
            return "";
        }
        string insertAt(int value, size_t index) {
            if (isBit) { if (value != 0 && value != 1) return "❌ Ошибка: для BitSequence допустимы только 0 или 1"; if (bitSeq) bitSeq->InsertAt(value, index); }
            else { if (intSeq) intSeq->InsertAt(value, index); }
            return "";
        }
        int get(size_t index) const { if (isBit) return bitSeq ? bitSeq->Get(index) : 0; return intSeq ? intSeq->Get(index) : 0; }
        int getFirst() const { if (isBit) return bitSeq ? bitSeq->GetFirst() : 0; return intSeq ? intSeq->GetFirst() : 0; }
        int getLast() const { if (isBit) return bitSeq ? bitSeq->GetLast() : 0; return intSeq ? intSeq->GetLast() : 0; }
        string getSubsequence(size_t start, size_t end) {
            if (!intSeq && !bitSeq) return "❌ Ошибка: последовательность не создана";
            if (start > end) return "❌ Ошибка: начальный индекс не может быть больше конечного";
            if (end >= getSize()) return "❌ Ошибка: конечный индекс выходит за границы (максимум " + to_string(getSize() - 1) + ")";
            stringstream ss; ss << "[";
            if (isBit) {
                BitSequence* sub = bitSeq->GetSubsequence(start, end);
                tempBitSeqs.push_back(sub);
                for (size_t i = 0; i < sub->GetLength(); i++) { if (i > 0) ss << ", "; ss << sub->Get(i); }
            } else {
                Sequence<int>* sub = intSeq->GetSubsequence(start, end);
                tempIntSeqs.push_back(sub);
                for (size_t i = 0; i < sub->GetLength(); i++) { if (i > 0) ss << ", "; ss << sub->Get(i); }
            }
            ss << "]";
            return ss.str();
        }
        void clearTempSubsequences() { for (auto seq : tempIntSeqs) delete seq; for (auto seq : tempBitSeqs) delete seq; tempIntSeqs.clear(); tempBitSeqs.clear(); }
    };
    
    SequenceData seq1;
    SequenceData seq2;
    SequenceData concatResult;

    string getBaseType(const string& type) const {
        string base = type;
        size_t pos = base.find(" (");
        if (pos != string::npos) base = base.substr(0, pos);
        return base;
    }
    
public:
    SequenceManager() {}
    ~SequenceManager() { seq1.clear(); seq2.clear(); concatResult.clear(); }
    
    void createSequence(int id, const string& type, const string& impl) {
        SequenceData* target = (id == 1) ? &seq1 : &seq2;
        target->clear();
        target->clearTempSubsequences();
        if (type == "ArraySequence") {
            if (impl == "Mutable") { target->intSeq = new MutableArraySequence<int>(); target->type = ARRAY_MUTABLE; }
            else { target->intSeq = new ImmutableArraySequence<int>(); target->type = ARRAY_IMMUTABLE; }
            target->isBit = false;
        }
        else if (type == "ListSequence") { target->intSeq = new ListSequence<int>(); target->type = LIST; target->isBit = false; }
        else if (type == "BitSequence") { target->bitSeq = new BitSequence(); target->type = BIT; target->isBit = true; }
    }
    
    bool isCreated(int id) const { const SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->intSeq != nullptr || target->bitSeq != nullptr; }
    string append(int id, int value) { SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->append(value); }
    string prepend(int id, int value) { SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->prepend(value); }
    string insertAt(int id, int value, size_t index) { SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->insertAt(value, index); }
    int get(int id, size_t index) const { const SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->get(index); }
    int getFirst(int id) const { const SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->getFirst(); }
    int getLast(int id) const { const SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->getLast(); }
    size_t getSize(int id) const { const SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->getSize(); }
    string getValuesString(int id) const { const SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->getValuesString(); }
    string getTypeString(int id) const { const SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->getTypeString(); }
    string getSubsequence(int id, size_t start, size_t end) { SequenceData* target = (id == 1) ? &seq1 : &seq2; return target->getSubsequence(start, end); }
    void clear(int id) { SequenceData* target = (id == 1) ? &seq1 : &seq2; target->clear(); target->clearTempSubsequences(); }
    
    string concat() {
        if (!seq1.intSeq && !seq1.bitSeq) return "❌ Ошибка: Первая последовательность не создана";
        if (!seq2.intSeq && !seq2.bitSeq) return "❌ Ошибка: Вторая последовательность не создана";
        if (seq1.isBit != seq2.isBit) return "❌ Ошибка: BitSequence можно объединять только с BitSequence";
        
        string base1 = getBaseType(seq1.getTypeString());
        string base2 = getBaseType(seq2.getTypeString());
        if (!seq1.isBit && base1 != base2) return "❌ Ошибка: Нельзя объединить " + base1 + " с " + base2;
        
        concatResult.clear();
        concatResult.clearTempSubsequences();
        string resultStr;
        
        if (seq1.isBit) {
            BitSequence* result = new BitSequence();
            for (size_t i = 0; i < seq1.bitSeq->GetLength(); i++) result->Append(seq1.bitSeq->Get(i));
            for (size_t i = 0; i < seq2.bitSeq->GetLength(); i++) result->Append(seq2.bitSeq->Get(i));
            concatResult.bitSeq = result;
            concatResult.isBit = true;
            concatResult.type = BIT;
            return "✅ Результат объединения: " + concatResult.getValuesString();
        }
        
        if (base1 == "ListSequence") {
            Sequence<int>* result = seq1.intSeq->Concat(seq2.intSeq);
            concatResult.intSeq = result;
            concatResult.type = LIST;
            return "✅ Результат объединения: " + concatResult.getValuesString();
        }
        
        bool isFirstMutable = (seq1.getTypeString().find("Mutable") != string::npos);
        bool isSecondMutable = (seq2.getTypeString().find("Mutable") != string::npos);
        bool resultIsMutable = (isFirstMutable && isSecondMutable);
        
        Sequence<int>* result = nullptr;
        if (resultIsMutable) {
            result = seq1.intSeq->Concat(seq2.intSeq);
            concatResult.type = ARRAY_MUTABLE;
        } else {
            ImmutableArraySequence<int>* immutableResult = new ImmutableArraySequence<int>();
            for (size_t i = 0; i < seq1.intSeq->GetLength(); i++) immutableResult->Append(seq1.intSeq->Get(i));
            for (size_t i = 0; i < seq2.intSeq->GetLength(); i++) immutableResult->Append(seq2.intSeq->Get(i));
            result = immutableResult;
            concatResult.type = ARRAY_IMMUTABLE;
        }
        concatResult.intSeq = result;
        concatResult.isBit = false;
        return "✅ Результат объединения: " + concatResult.getValuesString();
    }
    
    string getConcatResult() const { if (!concatResult.intSeq && !concatResult.bitSeq) return "Нет результата"; return concatResult.getValuesString(); }
    void clearConcatResult() { concatResult.clear(); }
};

// ==================== ГЛОБАЛЬНЫЕ ДАННЫЕ ====================

struct AppData 
{
    DynamicArrayManager<int> dynamicArray;
    LinkedListManager<int> linkedList;
    SequenceManager sequence;
};

AppData g_appData;

// ==================== HTML ГЕНЕРАЦИЯ ====================

string renderMatrixPage()
{
    stringstream ss;
    ss << "<!DOCTYPE html>";
    ss << "<html lang=\"ru\">";
    ss << "<head>";
    ss << "<meta charset=\"UTF-8\">";
    ss << "<meta http-equiv=\"Cache-Control\" content=\"no-cache, no-store, must-revalidate\">";
    ss << "<title>Матричный калькулятор</title>";
    ss << "<style>";
    ss << "* { margin: 0; padding: 0; box-sizing: border-box; }";
    ss << "body { font-family: 'Segoe UI', system-ui, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; padding: 20px; }";
    ss << ".container { max-width: 1400px; margin: 0 auto; background: white; border-radius: 24px; box-shadow: 0 25px 50px -12px rgba(0,0,0,0.25); overflow: hidden; }";
    ss << "h1 { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 24px 32px; text-align: center; font-size: 28px; }";
    ss << ".matrix-section { padding: 24px; border-bottom: 1px solid #eee; }";
    ss << ".matrix-section h2 { color: #667eea; margin-bottom: 16px; }";
    ss << ".matrix-grid { display: inline-block; background: #f8f9fa; padding: 20px; border-radius: 16px; margin: 10px 0; }";
    ss << ".matrix-row { display: flex; }";
    ss << ".matrix-cell { width: 70px; margin: 4px; }";
    ss << ".matrix-cell input { width: 100%; padding: 8px; text-align: center; border: 2px solid #dee2e6; border-radius: 8px; font-size: 14px; }";
    ss << ".dimension-panel { display: flex; gap: 16px; align-items: flex-end; margin-bottom: 20px; flex-wrap: wrap; }";
    ss << ".dimension-panel label { font-weight: 600; color: #555; }";
    ss << ".dimension-panel input { padding: 8px 12px; border: 2px solid #dee2e6; border-radius: 8px; width: 80px; }";
    ss << ".dimension-panel button { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; padding: 8px 20px; border-radius: 8px; cursor: pointer; }";
    ss << ".operations { display: flex; flex-wrap: wrap; gap: 10px; margin: 20px 0; padding: 20px; background: #f8f9fa; border-radius: 16px; }";
    ss << ".operations button { background: white; border: 2px solid #667eea; color: #667eea; padding: 8px 16px; border-radius: 8px; cursor: pointer; font-weight: 600; }";
    ss << ".operations button:hover { background: #667eea; color: white; }";
    ss << ".result { background: #f0f7ff; padding: 20px; border-radius: 16px; margin: 20px; font-family: monospace; white-space: pre-wrap; }";
    ss << ".error-result { background: #fee; border-left: 4px solid #dc3545; }";
    ss << ".back-btn { display: inline-block; margin: 20px; color: #667eea; text-decoration: none; }";
    ss << "</style>";
    ss << "<script>";
    
    ss << "function createMatrixGrid1() {";
    ss << "  let rows = parseInt(document.getElementById('rows1').value) || 3;";
    ss << "  let cols = parseInt(document.getElementById('cols1').value) || 3;";
    ss << "  let container = document.getElementById('matrix-grid1');";
    ss << "  if (!container) return;";
    ss << "  container.innerHTML = '';";
    ss << "  for (let i = 0; i < rows; i++) {";
    ss << "    let row = document.createElement('div');";
    ss << "    row.className = 'matrix-row';";
    ss << "    for (let j = 0; j < cols; j++) {";
    ss << "      let cell = document.createElement('div');";
    ss << "      cell.className = 'matrix-cell';";
    ss << "      let input = document.createElement('input');";
    ss << "      input.type = 'text';";
    ss << "      input.value = '0';";
    ss << "      input.id = 'cell1_' + i + '_' + j;";
    ss << "      cell.appendChild(input);";
    ss << "      row.appendChild(cell);";
    ss << "    }";
    ss << "    container.appendChild(row);";
    ss << "  }";
    ss << "}";
    
    ss << "function createMatrixGrid2() {";
    ss << "  let rows = parseInt(document.getElementById('rows2').value) || 3;";
    ss << "  let cols = parseInt(document.getElementById('cols2').value) || 3;";
    ss << "  let container = document.getElementById('matrix-grid2');";
    ss << "  if (!container) return;";
    ss << "  container.innerHTML = '';";
    ss << "  for (let i = 0; i < rows; i++) {";
    ss << "    let row = document.createElement('div');";
    ss << "    row.className = 'matrix-row';";
    ss << "    for (let j = 0; j < cols; j++) {";
    ss << "      let cell = document.createElement('div');";
    ss << "      cell.className = 'matrix-cell';";
    ss << "      let input = document.createElement('input');";
    ss << "      input.type = 'text';";
    ss << "      input.value = '0';";
    ss << "      input.id = 'cell2_' + i + '_' + j;";
    ss << "      cell.appendChild(input);";
    ss << "      row.appendChild(cell);";
    ss << "    }";
    ss << "    container.appendChild(row);";
    ss << "  }";
    ss << "}";
    
    ss << "function getMatrixData(id) {";
    ss << "  let rows = parseInt(document.getElementById('rows'+id).value);";
    ss << "  let cols = parseInt(document.getElementById('cols'+id).value);";
    ss << "  let data = [];";
    ss << "  for (let i = 0; i < rows; i++) {";
    ss << "    let row = [];";
    ss << "    for (let j = 0; j < cols; j++) {";
    ss << "      let input = document.getElementById('cell'+id+'_'+i+'_'+j);";
    ss << "      let val = input ? input.value : '0';";
    ss << "      row.push(val === '' ? '0' : val);";
    ss << "    }";
    ss << "    data.push(row);";
    ss << "  }";
    ss << "  return data;";
    ss << "}";
    
    ss << "function showResult(text, isError) {";
    ss << "  let resultDiv = document.getElementById('result');";
    ss << "  resultDiv.innerHTML = '<strong>Результат:</strong><br><pre>' + text + '</pre>';";
    ss << "  if (isError) resultDiv.classList.add('error-result');";
    ss << "  else resultDiv.classList.remove('error-result');";
    ss << "}";
    
    ss << "async function sendRequest(operation, getParamsCallback) {";
    ss << "  showResult('Выполняется...', false);";
    ss << "  let data1 = getMatrixData(1);";
    ss << "  let formData = new URLSearchParams();";
    ss << "  formData.append('operation', operation);";
    ss << "  formData.append('rows1', document.getElementById('rows1').value);";
    ss << "  formData.append('cols1', document.getElementById('cols1').value);";
    ss << "  formData.append('data1', JSON.stringify(data1));";
    ss << "  ";
    ss << "  if (operation === 'add') {";
    ss << "    let data2 = getMatrixData(2);";
    ss << "    formData.append('rows2', document.getElementById('rows2').value);";
    ss << "    formData.append('cols2', document.getElementById('cols2').value);";
    ss << "    formData.append('data2', JSON.stringify(data2));";
    ss << "  }";
    ss << "  ";
    ss << "  if (getParamsCallback) {";
    ss << "    let params = getParamsCallback();";
    ss << "    if (params === null) { showResult('Операция отменена', true); return; }";
    ss << "    for (let key in params) formData.append(key, params[key]);";
    ss << "  }";
    ss << "  ";
    ss << "  try {";
    ss << "    let response = await fetch('/api/matrix', { method: 'POST', body: formData });";
    ss << "    let result = await response.text();";
    ss << "    showResult(result, result.includes('Ошибка') || result.includes('❌'));";
    ss << "  } catch(e) { showResult('Ошибка: ' + e.message, true); }";
    ss << "}";
    
    ss << "function getTwoRows() {";
    ss << "  let max = parseInt(document.getElementById('rows1').value)-1;";
    ss << "  let r1 = prompt('Строка 1 (0-'+max+'):'); if(r1===null) return null;";
    ss << "  let r2 = prompt('Строка 2 (0-'+max+'):'); if(r2===null) return null;";
    ss << "  return { param1: r1, param2: r2 };";
    ss << "}";
    ss << "function getRowAndScalar() {";
    ss << "  let max = parseInt(document.getElementById('rows1').value)-1;";
    ss << "  let r = prompt('Строка (0-'+max+'):'); if(r===null) return null;";
    ss << "  let s = prompt('Скаляр:'); if(s===null) return null;";
    ss << "  return { param1: r, param2: s };";
    ss << "}";
    ss << "function getTwoRowsWithScalar() {";
    ss << "  let max = parseInt(document.getElementById('rows1').value)-1;";
    ss << "  let from = prompt('От какой строки (0-'+max+'):'); if(from===null) return null;";
    ss << "  let to = prompt('К какой строке (0-'+max+'):'); if(to===null) return null;";
    ss << "  let s = prompt('Коэффициент:', '1'); if(s===null) return null;";
    ss << "  return { param1: from, param2: to, param3: s };";
    ss << "}";
    ss << "function getTwoCols() {";
    ss << "  let max = parseInt(document.getElementById('cols1').value)-1;";
    ss << "  let c1 = prompt('Столбец 1 (0-'+max+'):'); if(c1===null) return null;";
    ss << "  let c2 = prompt('Столбец 2 (0-'+max+'):'); if(c2===null) return null;";
    ss << "  return { param1: c1, param2: c2 };";
    ss << "}";
    ss << "function getColAndScalar() {";
    ss << "  let max = parseInt(document.getElementById('cols1').value)-1;";
    ss << "  let c = prompt('Столбец (0-'+max+'):'); if(c===null) return null;";
    ss << "  let s = prompt('Скаляр:'); if(s===null) return null;";
    ss << "  return { param1: c, param2: s };";
    ss << "}";
    ss << "function getTwoColsWithScalar() {";
    ss << "  let max = parseInt(document.getElementById('cols1').value)-1;";
    ss << "  let from = prompt('От какого столбца (0-'+max+'):'); if(from===null) return null;";
    ss << "  let to = prompt('К какому столбцу (0-'+max+'):'); if(to===null) return null;";
    ss << "  let s = prompt('Коэффициент:', '1'); if(s===null) return null;";
    ss << "  return { param1: from, param2: to, param3: s };";
    ss << "}";
    ss << "function getScalar() {";
    ss << "  let s = prompt('Скаляр:'); if(s===null) return null;";
    ss << "  return { param1: s };";
    ss << "}";
    
    ss << "window.onload = function() { createMatrixGrid1(); createMatrixGrid2(); };";
    ss << "</script>";
    ss << "</head>";
    ss << "<body>";
    ss << "<div class=\"container\">";
    ss << "<h1>Прямоугольная матрица</h1>";
    
    ss << "<div class=\"matrix-section\">";
    ss << "<h2>Матрица №1</h2>";
    ss << "<div class=\"dimension-panel\">";
    ss << "<div><label>Строки:</label><input type=\"number\" id=\"rows1\" value=\"3\" min=\"1\" max=\"5\" onchange=\"createMatrixGrid1()\"></div>";
    ss << "<div><label>Столбцы:</label><input type=\"number\" id=\"cols1\" value=\"3\" min=\"1\" max=\"5\" onchange=\"createMatrixGrid1()\"></div>";
    ss << "<button onclick=\"createMatrixGrid1()\">Создать сетку</button>";
    ss << "</div>";
    ss << "<div id=\"matrix-grid1\" class=\"matrix-grid\"></div>";
    ss << "</div>";
    
    ss << "<div class=\"matrix-section\">";
    ss << "<h2>Матрица №2 (для сложения)</h2>";
    ss << "<div class=\"dimension-panel\">";
    ss << "<div><label>Строки:</label><input type=\"number\" id=\"rows2\" value=\"3\" min=\"1\" max=\"5\" onchange=\"createMatrixGrid2()\"></div>";
    ss << "<div><label>Столбцы:</label><input type=\"number\" id=\"cols2\" value=\"3\" min=\"1\" max=\"5\" onchange=\"createMatrixGrid2()\"></div>";
    ss << "<button onclick=\"createMatrixGrid2()\">Создать сетку</button>";
    ss << "</div>";
    ss << "<div id=\"matrix-grid2\" class=\"matrix-grid\"></div>";
    ss << "</div>";
    
    ss << "<div class=\"operations\">";
    ss << "<h3 style=\"width:100%;\">Основные операции</h3>";
    ss << "<button onclick=\"sendRequest('norm', null)\">Норма</button>";
    ss << "<button onclick=\"sendRequest('multiply_scalar', getScalar)\">Умножить на скаляр</button>";
    ss << "<button onclick=\"sendRequest('add', null)\">Сложить</button>";
    ss << "</div>";
    
    ss << "<div class=\"operations\">";
    ss << "<h3 style=\"width:100%;\">Преобразования строк</h3>";
    ss << "<button onclick=\"sendRequest('swap_rows', getTwoRows)\">Переставить строки</button>";
    ss << "<button onclick=\"sendRequest('multiply_row', getRowAndScalar)\">Умножить строку</button>";
    ss << "<button onclick=\"sendRequest('add_row', getTwoRowsWithScalar)\">Прибавить строку</button>";
    ss << "</div>";
    
    ss << "<div class=\"operations\">";
    ss << "<h3 style=\"width:100%;\">Преобразования столбцов</h3>";
    ss << "<button onclick=\"sendRequest('swap_cols', getTwoCols)\">Переставить столбцы</button>";
    ss << "<button onclick=\"sendRequest('multiply_col', getColAndScalar)\">Умножить столбец</button>";
    ss << "<button onclick=\"sendRequest('add_col', getTwoColsWithScalar)\">Прибавить столбец</button>";
    ss << "</div>";
    
    ss << "<div id=\"result\" class=\"result\"><strong>Результат:</strong><br>Здесь появится результат...</div>";
    ss << "<a href=\"/\" class=\"back-btn\">← Вернуться на главную</a>";
    ss << "</div>";
    ss << "</body>";
    ss << "</html>";
    
    return ss.str();
}

string renderFunctionalTestPage()
{
    stringstream ss;
    ss << "<!DOCTYPE html>";
    ss << "<html lang=\"ru\">";
    ss << "<head>";
    ss << "<meta charset=\"UTF-8\">";
    ss << "<title>Функциональные операции</title>";
    ss << "<style>";
    ss << "* { margin: 0; padding: 0; box-sizing: border-box; }";
    ss << "body { font-family: 'Segoe UI', sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; padding: 20px; }";
    ss << ".container { max-width: 900px; margin: 0 auto; background: white; border-radius: 24px; padding: 32px; }";
    ss << "h1 { color: #667eea; margin-bottom: 20px; }";
    ss << ".input-group { margin-bottom: 20px; }";
    ss << "label { display: block; font-weight: 600; margin-bottom: 5px; color: #555; }";
    ss << "input[type=text] { width: 100%; padding: 10px; border: 2px solid #dee2e6; border-radius: 8px; font-size: 14px; }";
    ss << "select { width: 100%; padding: 10px; border: 2px solid #dee2e6; border-radius: 8px; font-size: 14px; }";
    ss << "button { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; padding: 12px 24px; border-radius: 10px; font-size: 16px; cursor: pointer; margin-top: 10px; }";
    ss << ".result { background: #f8f9fa; padding: 20px; border-radius: 12px; margin-top: 20px; font-family: monospace; }";
    ss << ".back-link { display: inline-block; margin-top: 20px; color: #667eea; text-decoration: none; }";
    ss << "</style>";
    ss << "</head>";
    ss << "<body>";
    ss << "<div class=\"container\">";
    ss << "<h1>Демострация функциональных операций</h1>";
    ss << "<p>Map, Filter, Reduce над последовательностями</p>";
    
    ss << "<div class=\"input-group\">";
    ss << "<label>Введите последовательность чисел (через запятую):</label>";
    ss << "<input type=\"text\" id=\"inputSeq\" value=\"1,2,3,4,5,6,7,8,9,10\">";
    ss << "</div>";
    
    ss << "<div class=\"input-group\">";
    ss << "<label>Выберите операцию:</label>";
    ss << "<select id=\"operation\">";
    ss << "<option value=\"map_square\">Map: возвести в квадрат</option>";
    ss << "<option value=\"map_double\">Map: умножить на 2</option>";
    ss << "<option value=\"filter_even\">Filter: чётные числа</option>";
    ss << "<option value=\"filter_positive\">Filter: положительные числа</option>";
    ss << "<option value=\"reduce_sum\">Reduce: сумма</option>";
    ss << "<option value=\"reduce_product\">Reduce: произведение</option>";
    ss << "<option value=\"reduce_max\">Reduce: максимум</option>";
    ss << "<option value=\"reduce_min\">Reduce: минимум</option>";
    ss << "</select>";
    ss << "</div>";
    
    ss << "<button onclick=\"execute()\">Выполнить</button>";
    ss << "<div id=\"result\" class=\"result\">Результат появится здесь...</div>";
    ss << "<a href=\"/\" class=\"back-link\">← Вернуться на главную</a>";
    ss << "</div>";
    
    ss << "<script>";
    ss << "async function execute() {";
    ss << "  let input = document.getElementById('inputSeq').value;";
    ss << "  let operation = document.getElementById('operation').value;";
    ss << "  let formData = new URLSearchParams();";
    ss << "  formData.append('input', input);";
    ss << "  formData.append('operation', operation);";
    ss << "  try {";
    ss << "    let response = await fetch('/api/functional', { method: 'POST', body: formData });";
    ss << "    let result = await response.text();";
    ss << "    document.getElementById('result').innerHTML = '<strong>Результат:</strong><br>' + result;";
    ss << "  } catch(e) {";
    ss << "    document.getElementById('result').innerHTML = '<strong>Ошибка:</strong> ' + e.message;";
    ss << "  }";
    ss << "}";
    ss << "</script>";
    ss << "</body>";
    ss << "</html>";
    
    return ss.str();
}

string renderFullPage() 
{
    stringstream ss;
    ss << "<!DOCTYPE html>";
    ss << "<html lang=\"ru\">";
    ss << "<head>";
    ss << "<meta charset=\"UTF-8\">";
    ss << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    ss << "<title>Лабораторная работа</title>";
    ss << "<script src=\"https://unpkg.com/htmx.org@1.9.10\"></script>";
    ss << "<style>";
    ss << "* { margin: 0; padding: 0; box-sizing: border-box; }";
    ss << "body { font-family: 'Segoe UI', system-ui, sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; padding: 20px; }";
    ss << ".container { max-width: 1400px; margin: 0 auto; background: white; border-radius: 24px; box-shadow: 0 25px 50px -12px rgba(0,0,0,0.25); overflow: hidden; }";
    ss << "h1 { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 24px 32px; text-align: center; font-size: 28px; }";
    ss << ".demo-btn { padding: 10px 20px; margin: 0 10px; border: none; border-radius: 8px; color: white; font-size: 16px; cursor: pointer; transition: transform 0.2s; }";
    ss << ".demo-btn:hover { transform: translateY(-2px); }";
    ss << ".tabs { display: flex; background: #f8f9fa; border-bottom: 1px solid #e9ecef; padding: 0 16px; gap: 4px; }";
    ss << ".tab-btn { flex: 1; padding: 16px 24px; font-size: 16px; font-weight: 600; background: transparent; border: none; cursor: pointer; transition: all 0.3s; color: #6c757d; border-radius: 12px 12px 0 0; }";
    ss << ".tab-btn:hover { background: #e9ecef; color: #667eea; }";
    ss << ".tab-btn.active { background: white; color: #667eea; border-bottom: 3px solid #667eea; }";
    ss << ".tab-content { display: none; padding: 32px; }";
    ss << ".tab-content.active { display: block; }";
    ss << ".two-columns { display: flex; gap: 24px; margin-bottom: 32px; }";
    ss << ".column { flex: 1; background: #f8f9fa; border-radius: 16px; padding: 20px; }";
    ss << ".column h3 { margin-bottom: 16px; color: #667eea; }";
    ss << ".concat-section { background: linear-gradient(135deg, #e8f4f8 0%, #f0e6ff 100%); border-radius: 16px; padding: 24px; margin-top: 20px; text-align: center; }";
    ss << ".concat-section h3 { color: #764ba2; margin-bottom: 16px; }";
    ss << ".concat-result { background: #fff; border-radius: 12px; padding: 16px; margin-top: 16px; font-family: monospace; font-size: 16px; }";
    ss << ".panel-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 24px; flex-wrap: wrap; gap: 16px; }";
    ss << "h2 { color: #333; font-size: 24px; }";
    ss << "h3 { color: #555; font-size: 18px; margin-bottom: 16px; }";
    ss << ".create-btn { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; padding: 8px 16px; border-radius: 8px; cursor: pointer; font-weight: 600; font-size: 14px; }";
    ss << ".create-btn:hover { transform: translateY(-1px); }";
    ss << ".delete-btn { background: #dc3545; }";
    ss << ".delete-btn:hover { background: #c82333; }";
    ss << ".concat-btn { background: linear-gradient(135deg, #28a745 0%, #20c997 100%); color: white; border: none; padding: 12px 32px; border-radius: 12px; cursor: pointer; font-weight: 600; font-size: 18px; margin: 10px; }";
    ss << ".concat-btn:hover { transform: translateY(-2px); box-shadow: 0 4px 12px rgba(40,167,69,0.4); }";
    ss << ".subseq-btn { background: linear-gradient(135deg, #17a2b8 0%, #6f42c1 100%); color: white; border: none; padding: 6px 14px; border-radius: 8px; cursor: pointer; font-weight: 600; font-size: 13px; }";
    ss << ".subseq-btn:hover { transform: translateY(-1px); }";
    ss << ".sequence-select { display: flex; gap: 12px; align-items: center; flex-wrap: wrap; margin-bottom: 16px; }";
    ss << ".sequence-select label { font-weight: 600; color: #555; font-size: 14px; }";
    ss << ".sequence-select select { padding: 6px 10px; border: 2px solid #dee2e6; border-radius: 6px; background: white; font-size: 14px; }";
    ss << ".values-display { background: white; padding: 12px 16px; border-radius: 10px; margin-bottom: 20px; font-family: 'Courier New', monospace; border-left: 4px solid #667eea; }";
    ss << ".label { font-weight: bold; color: #667eea; margin-right: 12px; }";
    ss << ".values { color: #212529; font-size: 14px; word-break: break-all; }";
    ss << ".info-row { display: flex; gap: 24px; margin-bottom: 20px; padding: 8px 0; color: #6c757d; font-size: 13px; }";
    ss << ".input-group { background: white; padding: 16px; border-radius: 12px; margin-bottom: 16px; }";
    ss << ".operation { display: flex; gap: 10px; margin-bottom: 12px; align-items: center; flex-wrap: wrap; }";
    ss << ".operation button { background: #f8f9fa; border: 1px solid #dee2e6; color: #495057; padding: 6px 14px; border-radius: 6px; cursor: pointer; font-size: 13px; }";
    ss << ".operation button:hover { background: #667eea; color: white; border-color: #667eea; }";
    ss << ".operation input { padding: 6px 10px; border: 1px solid #dee2e6; border-radius: 6px; width: 100px; font-size: 13px; }";
    ss << ".operation input:focus { outline: none; border-color: #667eea; }";
    ss << ".info-note { background: #e7f3ff; padding: 10px 14px; border-radius: 8px; font-size: 12px; color: #333; border-left: 3px solid #667eea; margin-top: 16px; }";
    ss << ".subseq-result { margin-top: 10px; font-size: 13px; color: #6f42c1; }";
    ss << "#message-area { margin: 0 32px 32px 32px; padding: 12px 20px; border-radius: 12px; display: flex; justify-content: space-between; align-items: center; }";
    ss << ".error-message { background: #fee; border-left: 4px solid #dc3545; }";
    ss << ".success-message { background: #e8f4e8; border-left: 4px solid #28a745; }";
    ss << "#message-text { font-size: 14px; color: #333; }";
    ss << "#message-area button { background: none; border: none; font-size: 18px; cursor: pointer; color: #adb5bd; padding: 0 8px; }";
    ss << "#message-area button:hover { color: #dc3545; }";
    ss << "@media (max-width: 900px) { .two-columns { flex-direction: column; } }";
    ss << "</style>";
    ss << "</head>";
    ss << "<body>";
    ss << "<div class=\"container\">";
    ss << "<h1>Лабораторная работа №3</h1>";

    ss << "<div style=\"text-align:center; margin: 16px 0;\">";
    ss << "<button class=\"demo-btn\" onclick=\"window.open('/matrix', '_blank')\" style=\"background: linear-gradient(135deg, #28a745 0%, #20c997 100%);\">📊 Матричный калькулятор</button>";
    ss << "<button class=\"demo-btn\" onclick=\"window.open('/functional-test', '_blank')\" style=\"background: linear-gradient(135deg, #17a2b8 0%, #6f42c1 100%);\">🧪 Дополнительный функционал</button>";
    ss << "</div>";
    
    ss << "<div class=\"tabs\">";
    ss << "<button class=\"tab-btn active\" data-tab=\"dynamic\">📦 Динамический массив</button>";
    ss << "<button class=\"tab-btn\" data-tab=\"linked\">🔗 Связный список</button>";
    ss << "<button class=\"tab-btn\" data-tab=\"sequence\">📜 Последовательности</button>";
    ss << "</div>";
    
    // Вкладка Динамический массив
    ss << "<div id=\"dynamic-tab\" class=\"tab-content active\">";
    ss << "<div class=\"panel-header\"><h2>Динамический массив</h2>";
    ss << "<button class=\"create-btn\" onclick=\"apiCall('/api/dynamicarray', {action:'create'})\">➕ Создать</button></div>";
    ss << "<div class=\"values-display\"><span class=\"label\">Значения:</span><span class=\"values\" id=\"da-values\">" << g_appData.dynamicArray.getValuesString() << "</span></div>";
    ss << "<div class=\"info-row\"><span>📏 Размер: <span id=\"da-size\">" << g_appData.dynamicArray.getSize() << "</span></span>";
    ss << "<span>📌 Статус: <span id=\"da-status\">" << (g_appData.dynamicArray.isCreated() ? "✅ Создан" : "❌ Не создан") << "</span></span></div>";
    ss << "<div class=\"input-group\"><h3>Операции</h3>";
    ss << "<div class=\"operation\"><button onclick=\"daAppend()\">➕ Добавить в конец</button><input type=\"text\" id=\"da-append-val\" placeholder=\"Число\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"daPrepend()\">➕ Добавить в начало</button><input type=\"text\" id=\"da-prepend-val\" placeholder=\"Число\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"daInsert()\">📌 Вставить</button><input type=\"text\" id=\"da-insert-index\" placeholder=\"Индекс\"><input type=\"text\" id=\"da-insert-val\" placeholder=\"Число\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"daSet()\">✏️ Изменить</button><input type=\"text\" id=\"da-set-index\" placeholder=\"Индекс\"><input type=\"text\" id=\"da-set-val\" placeholder=\"Число\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"daGet()\">🔍 Получить</button><input type=\"text\" id=\"da-get-index\" placeholder=\"Индекс\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"daClear()\">🗑️ Очистить</button></div></div>";
    ss << "<div class=\"info-note\">💡 Динамический массив автоматически расширяется при добавлении элементов. Допустимы только целые числа.</div></div>";
    
    // Вкладка Связный список
    ss << "<div id=\"linked-tab\" class=\"tab-content\">";
    ss << "<div class=\"panel-header\"><h2>Связный список</h2>";
    ss << "<button class=\"create-btn\" onclick=\"apiCall('/api/linkedlist', {action:'create'})\">➕ Создать</button></div>";
    ss << "<div class=\"values-display\"><span class=\"label\">Значения:</span><span class=\"values\" id=\"ll-values\">" << g_appData.linkedList.getValuesString() << "</span></div>";
    ss << "<div class=\"info-row\"><span>📏 Размер: <span id=\"ll-size\">" << g_appData.linkedList.getSize() << "</span></span>";
    ss << "<span>📌 Статус: <span id=\"ll-status\">" << (g_appData.linkedList.isCreated() ? "✅ Создан" : "❌ Не создан") << "</span></span></div>";
    ss << "<div class=\"input-group\"><h3>Операции</h3>";
    ss << "<div class=\"operation\"><button onclick=\"llAppend()\">➕ Добавить в конец</button><input type=\"text\" id=\"ll-append-val\" placeholder=\"Число\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"llPrepend()\">➕ Добавить в начало</button><input type=\"text\" id=\"ll-prepend-val\" placeholder=\"Число\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"llInsert()\">📌 Вставить</button><input type=\"text\" id=\"ll-insert-index\" placeholder=\"Индекс\"><input type=\"text\" id=\"ll-insert-val\" placeholder=\"Число\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"llGet()\">🔍 Получить</button><input type=\"text\" id=\"ll-get-index\" placeholder=\"Индекс\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"llGetFirst()\">🔍 Первый элемент</button></div>";
    ss << "<div class=\"operation\"><button onclick=\"llGetLast()\">🔍 Последний элемент</button></div>";
    ss << "<div class=\"operation\"><button onclick=\"llClear()\">🗑️ Очистить</button></div></div>";
    ss << "<div class=\"info-note\">💡 Связный список хранит элементы в узлах, каждый указывает на следующий.</div></div>";
    
    // Вкладка Последовательности
    ss << "<div id=\"sequence-tab\" class=\"tab-content\">";
    ss << "<div class=\"two-columns\">";
    
    // Последовательность 1
    ss << "<div class=\"column\">";
    ss << "<div class=\"panel-header\"><h3>📋 Последовательность №1</h3>";
    ss << "<button class=\"create-btn\" onclick=\"seqCreate(1)\">✨ Создать</button>";
    ss << "<button class=\"create-btn delete-btn\" onclick=\"seqClear(1)\">🗑️ Очистить</button></div>";
    ss << "<div class=\"sequence-select\">";
    ss << "<label>Тип:</label><select id=\"seq1-type\">";
    ss << "<option value=\"ArraySequence\">ArraySequence</option>";
    ss << "<option value=\"ListSequence\">ListSequence</option>";
    ss << "<option value=\"BitSequence\">BitSequence</option>";
    ss << "</select>";
    ss << "<span id=\"seq1-impl-container\"><label>Реализация:</label><select id=\"seq1-impl\"><option value=\"Mutable\">Mutable</option><option value=\"Immutable\">Immutable</option></select></span>";
    ss << "</div>";
    ss << "<div class=\"values-display\"><span class=\"label\">Значения:</span><span class=\"values\" id=\"seq1-values\">" << g_appData.sequence.getValuesString(1) << "</span></div>";
    ss << "<div class=\"info-row\"><span>📏 Размер: <span id=\"seq1-size\">" << g_appData.sequence.getSize(1) << "</span></span>";
    ss << "<span>📌 Тип: <span id=\"seq1-type-display\">" << g_appData.sequence.getTypeString(1) << "</span></span></div>";
    ss << "<div class=\"input-group\"><h3>Базовые операции</h3>";
    ss << "<div class=\"operation\"><button onclick=\"seqAppend(1)\">➕ Добавить в конец</button><input type=\"text\" id=\"seq1-append-val\" placeholder=\"Значение\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqPrepend(1)\">➕ Добавить в начало</button><input type=\"text\" id=\"seq1-prepend-val\" placeholder=\"Значение\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqInsert(1)\">📌 Вставить</button><input type=\"text\" id=\"seq1-insert-index\" placeholder=\"Индекс\"><input type=\"text\" id=\"seq1-insert-val\" placeholder=\"Значение\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqGet(1)\">🔍 Получить по индексу</button><input type=\"text\" id=\"seq1-get-index\" placeholder=\"Индекс\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqGetFirst(1)\">🔍 Первый элемент</button></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqGetLast(1)\">🔍 Последний элемент</button></div>";
    ss << "</div>";
    ss << "<div class=\"input-group\"><h3>✂️ GetSubsequence</h3>";
    ss << "<div class=\"operation\">";
    ss << "<button class=\"subseq-btn\" onclick=\"seqGetSubsequence(1)\">✂️ Получить подпоследовательность</button>";
    ss << "<input type=\"text\" id=\"seq1-sub-start\" placeholder=\"Начало\" style=\"width:80px\">";
    ss << "<input type=\"text\" id=\"seq1-sub-end\" placeholder=\"Конец\" style=\"width:80px\">";
    ss << "</div>";
    ss << "<div id=\"seq1-sub-result\" class=\"subseq-result\"></div>";
    ss << "</div></div>";
    
    // Последовательность 2
    ss << "<div class=\"column\">";
    ss << "<div class=\"panel-header\"><h3>📋 Последовательность №2</h3>";
    ss << "<button class=\"create-btn\" onclick=\"seqCreate(2)\">✨ Создать</button>";
    ss << "<button class=\"create-btn delete-btn\" onclick=\"seqClear(2)\">🗑️ Очистить</button></div>";
    ss << "<div class=\"sequence-select\">";
    ss << "<label>Тип:</label><select id=\"seq2-type\">";
    ss << "<option value=\"ArraySequence\">ArraySequence</option>";
    ss << "<option value=\"ListSequence\">ListSequence</option>";
    ss << "<option value=\"BitSequence\">BitSequence</option>";
    ss << "</select>";
    ss << "<span id=\"seq2-impl-container\"><label>Реализация:</label><select id=\"seq2-impl\"><option value=\"Mutable\">Mutable</option><option value=\"Immutable\">Immutable</option></select></span>";
    ss << "</div>";
    ss << "<div class=\"values-display\"><span class=\"label\">Значения:</span><span class=\"values\" id=\"seq2-values\">" << g_appData.sequence.getValuesString(2) << "</span></div>";
    ss << "<div class=\"info-row\"><span>📏 Размер: <span id=\"seq2-size\">" << g_appData.sequence.getSize(2) << "</span></span>";
    ss << "<span>📌 Тип: <span id=\"seq2-type-display\">" << g_appData.sequence.getTypeString(2) << "</span></span></div>";
    ss << "<div class=\"input-group\"><h3>Базовые операции</h3>";
    ss << "<div class=\"operation\"><button onclick=\"seqAppend(2)\">➕ Добавить в конец</button><input type=\"text\" id=\"seq2-append-val\" placeholder=\"Значение\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqPrepend(2)\">➕ Добавить в начало</button><input type=\"text\" id=\"seq2-prepend-val\" placeholder=\"Значение\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqInsert(2)\">📌 Вставить</button><input type=\"text\" id=\"seq2-insert-index\" placeholder=\"Индекс\"><input type=\"text\" id=\"seq2-insert-val\" placeholder=\"Значение\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqGet(2)\">🔍 Получить по индексу</button><input type=\"text\" id=\"seq2-get-index\" placeholder=\"Индекс\"></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqGetFirst(2)\">🔍 Первый элемент</button></div>";
    ss << "<div class=\"operation\"><button onclick=\"seqGetLast(2)\">🔍 Последний элемент</button></div>";
    ss << "</div>";
    ss << "<div class=\"input-group\"><h3>✂️ GetSubsequence</h3>";
    ss << "<div class=\"operation\">";
    ss << "<button class=\"subseq-btn\" onclick=\"seqGetSubsequence(2)\">✂️ Получить подпоследовательность</button>";
    ss << "<input type=\"text\" id=\"seq2-sub-start\" placeholder=\"Начало\" style=\"width:80px\">";
    ss << "<input type=\"text\" id=\"seq2-sub-end\" placeholder=\"Конец\" style=\"width:80px\">";
    ss << "</div>";
    ss << "<div id=\"seq2-sub-result\" class=\"subseq-result\"></div>";
    ss << "</div></div></div>";
    
    // Concat секция
    ss << "<div class=\"concat-section\">";
    ss << "<h3>🔗 ОПЕРАЦИЯ CONCAT (ОБЪЕДИНЕНИЕ)</h3>";
    ss << "<p>Объединяет последовательность №1 и последовательность №2 в новую</p>";
    ss << "<button class=\"concat-btn\" onclick=\"seqConcat()\">🔗 ОБЪЕДИНИТЬ →</button>";
    ss << "<div class=\"concat-result\">";
    ss << "<span class=\"label\">Результат объединения:</span>";
    ss << "<span id=\"concat-result\">" << g_appData.sequence.getConcatResult() << "</span>";
    ss << "</div>";
    ss << "<div class=\"info-note\">💡 При объединении создаётся новая последовательность, исходные не изменяются.</div>";
    ss << "</div></div>";
    
    ss << "<div id=\"message-area\">";
    ss << "<span id=\"message-text\">✅ Готов к работе</span>";
    ss << "<button onclick=\"document.getElementById('message-text').innerHTML='✅ Готов к работе'; document.getElementById('message-area').classList.remove('error-message', 'success-message');\">✖</button>";
    ss << "</div>";
    ss << "</div>";
    
    // JavaScript
    ss << "<script>";
    ss << "async function apiCall(endpoint, params) {";
    ss << "  try {";
    ss << "    const formData = new URLSearchParams();";
    ss << "    for (const [key, value] of Object.entries(params)) formData.append(key, value);";
    ss << "    const response = await fetch(endpoint, { method: 'POST', headers: { 'Content-Type': 'application/x-www-form-urlencoded' }, body: formData });";
    ss << "    const msg = await response.text();";
    ss << "    showMessage(msg);";
    ss << "    refreshState();";
    ss << "    return msg;";
    ss << "  } catch(e) { showMessage('❌ Ошибка соединения: ' + e.message, true); return null; }";
    ss << "}";
    ss << "function showMessage(msg, isError = false) {";
    ss << "  const msgDiv = document.getElementById('message-text');";
    ss << "  msgDiv.innerHTML = msg;";
    ss << "  const container = document.getElementById('message-area');";
    ss << "  if (isError || msg.includes('❌')) { container.classList.add('error-message'); container.classList.remove('success-message'); }";
    ss << "  else { container.classList.add('success-message'); container.classList.remove('error-message'); }";
    ss << "}";
    ss << "async function refreshState() {";
    ss << "  try {";
    ss << "    const response = await fetch('/state');";
    ss << "    const state = await response.json();";
    ss << "    updateUI(state);";
    ss << "  } catch(e) { console.error(e); }";
    ss << "}";
    ss << "function updateUI(state) {";
    ss << "  document.getElementById('da-values').innerHTML = state.dynamicArrayValues || '[]';";
    ss << "  document.getElementById('da-size').innerHTML = state.dynamicArraySize || 0;";
    ss << "  document.getElementById('da-status').innerHTML = state.dynamicArrayCreated ? '✅ Создан' : '❌ Не создан';";
    ss << "  document.getElementById('ll-values').innerHTML = state.linkedListValues || '[]';";
    ss << "  document.getElementById('ll-size').innerHTML = state.linkedListSize || 0;";
    ss << "  document.getElementById('ll-status').innerHTML = state.linkedListCreated ? '✅ Создан' : '❌ Не создан';";
    ss << "  document.getElementById('seq1-values').innerHTML = state.seq1Values || '[]';";
    ss << "  document.getElementById('seq1-size').innerHTML = state.seq1Size || 0;";
    ss << "  document.getElementById('seq1-type-display').innerHTML = state.seq1Type || 'Не создана';";
    ss << "  document.getElementById('seq2-values').innerHTML = state.seq2Values || '[]';";
    ss << "  document.getElementById('seq2-size').innerHTML = state.seq2Size || 0;";
    ss << "  document.getElementById('seq2-type-display').innerHTML = state.seq2Type || 'Не создана';";
    ss << "  document.getElementById('concat-result').innerHTML = state.concatResult || 'Нет результата';";
    ss << "}";
    
    // Dynamic Array
    ss << "function daCreate() { apiCall('/api/dynamicarray', {action:'create'}); }";
    ss << "function daAppend() { let val=document.getElementById('da-append-val').value; if(val) apiCall('/api/dynamicarray', {action:'append', value:val}); else showMessage('❌ Введите значение', true); document.getElementById('da-append-val').value=''; }";
    ss << "function daPrepend() { let val=document.getElementById('da-prepend-val').value; if(val) apiCall('/api/dynamicarray', {action:'prepend', value:val}); else showMessage('❌ Введите значение', true); document.getElementById('da-prepend-val').value=''; }";
    ss << "function daInsert() { let idx=document.getElementById('da-insert-index').value, val=document.getElementById('da-insert-val').value; if(idx&&val) apiCall('/api/dynamicarray', {action:'insert', index:idx, value:val}); else showMessage('❌ Введите индекс и значение', true); document.getElementById('da-insert-index').value=''; document.getElementById('da-insert-val').value=''; }";
    ss << "function daSet() { let idx=document.getElementById('da-set-index').value, val=document.getElementById('da-set-val').value; if(idx&&val) apiCall('/api/dynamicarray', {action:'set', index:idx, value:val}); else showMessage('❌ Введите индекс и значение', true); document.getElementById('da-set-index').value=''; document.getElementById('da-set-val').value=''; }";
    ss << "function daGet() { let idx=document.getElementById('da-get-index').value; if(idx) apiCall('/api/dynamicarray', {action:'get', index:idx}); else showMessage('❌ Введите индекс', true); document.getElementById('da-get-index').value=''; }";
    ss << "function daClear() { apiCall('/api/dynamicarray', {action:'clear'}); }";
    
    // Linked List
    ss << "function llCreate() { apiCall('/api/linkedlist', {action:'create'}); }";
    ss << "function llAppend() { let val=document.getElementById('ll-append-val').value; if(val) apiCall('/api/linkedlist', {action:'append', value:val}); else showMessage('❌ Введите значение', true); document.getElementById('ll-append-val').value=''; }";
    ss << "function llPrepend() { let val=document.getElementById('ll-prepend-val').value; if(val) apiCall('/api/linkedlist', {action:'prepend', value:val}); else showMessage('❌ Введите значение', true); document.getElementById('ll-prepend-val').value=''; }";
    ss << "function llInsert() { let idx=document.getElementById('ll-insert-index').value, val=document.getElementById('ll-insert-val').value; if(idx&&val) apiCall('/api/linkedlist', {action:'insert', index:idx, value:val}); else showMessage('❌ Введите индекс и значение', true); document.getElementById('ll-insert-index').value=''; document.getElementById('ll-insert-val').value=''; }";
    ss << "function llGet() { let idx=document.getElementById('ll-get-index').value; if(idx) apiCall('/api/linkedlist', {action:'get', index:idx}); else showMessage('❌ Введите индекс', true); document.getElementById('ll-get-index').value=''; }";
    ss << "function llGetFirst() { apiCall('/api/linkedlist', {action:'getFirst'}); }";
    ss << "function llGetLast() { apiCall('/api/linkedlist', {action:'getLast'}); }";
    ss << "function llClear() { apiCall('/api/linkedlist', {action:'clear'}); }";
    
    // Sequence
    ss << "function seqCreate(id) {";
    ss << "  let type = document.getElementById('seq'+id+'-type').value;";
    ss << "  let impl = '';";
    ss << "  if (type === 'ArraySequence') impl = document.getElementById('seq'+id+'-impl').value;";
    ss << "  apiCall('/api/sequence', {action:'create', id:id, type:type, impl:impl});";
    ss << "}";
    ss << "function seqClear(id) { apiCall('/api/sequence', {action:'clear', id:id}); }";
    ss << "function seqAppend(id) { let val=document.getElementById('seq'+id+'-append-val').value; if(val) apiCall('/api/sequence', {action:'append', id:id, value:val}); else showMessage('❌ Введите значение', true); document.getElementById('seq'+id+'-append-val').value=''; }";
    ss << "function seqPrepend(id) { let val=document.getElementById('seq'+id+'-prepend-val').value; if(val) apiCall('/api/sequence', {action:'prepend', id:id, value:val}); else showMessage('❌ Введите значение', true); document.getElementById('seq'+id+'-prepend-val').value=''; }";
    ss << "function seqInsert(id) { let idx=document.getElementById('seq'+id+'-insert-index').value, val=document.getElementById('seq'+id+'-insert-val').value; if(idx&&val) apiCall('/api/sequence', {action:'insert', id:id, index:idx, value:val}); else showMessage('❌ Введите индекс и значение', true); document.getElementById('seq'+id+'-insert-index').value=''; document.getElementById('seq'+id+'-insert-val').value=''; }";
    ss << "function seqGet(id) { let idx=document.getElementById('seq'+id+'-get-index').value; if(idx) apiCall('/api/sequence', {action:'get', id:id, index:idx}); else showMessage('❌ Введите индекс', true); document.getElementById('seq'+id+'-get-index').value=''; }";
    ss << "function seqGetFirst(id) { apiCall('/api/sequence', {action:'getFirst', id:id}); }";
    ss << "function seqGetLast(id) { apiCall('/api/sequence', {action:'getLast', id:id}); }";
    ss << "function seqGetSubsequence(id) {";
    ss << "  let start = document.getElementById('seq'+id+'-sub-start').value;";
    ss << "  let end = document.getElementById('seq'+id+'-sub-end').value;";
    ss << "  if (!start || !end) { showMessage('❌ Укажите начальный и конечный индексы', true); return; }";
    ss << "  apiCall('/api/sequence/subsequence', {id:id, start:start, end:end}).then(msg => {";
    ss << "    if (msg && !msg.includes('❌')) document.getElementById('seq'+id+'-sub-result').innerHTML = '<span class=\"label\">Результат:</span> ' + msg;";
    ss << "  });";
    ss << "}";
    ss << "function seqConcat() { ";
    ss << "  if (!document.getElementById('seq1-values').innerHTML.includes('[') || ";
    ss << "      !document.getElementById('seq2-values').innerHTML.includes('[')) { ";
    ss << "    showMessage('❌ Ошибка: Обе последовательности должны быть созданы', true); ";
    ss << "    return; ";
    ss << "  } ";
    ss << "  apiCall('/api/sequence/concat', {}).then(msg => { ";
    ss << "    if (msg && msg.includes('Immutable')) { ";
    ss << "      showMessage(msg + ' ℹ️ Результат неизменяемый (Immutable)', false); ";
    ss << "    } ";
    ss << "  }); ";
    ss << "}";
    
    ss << "function updateImplVisibility(id) {";
    ss << "  let type = document.getElementById('seq'+id+'-type').value;";
    ss << "  let container = document.getElementById('seq'+id+'-impl-container');";
    ss << "  if (type === 'ArraySequence') { container.style.display = 'inline-block'; }";
    ss << "  else { container.style.display = 'none'; }";
    ss << "}";
    ss << "document.getElementById('seq1-type').addEventListener('change', () => updateImplVisibility(1));";
    ss << "document.getElementById('seq2-type').addEventListener('change', () => updateImplVisibility(2));";
    ss << "updateImplVisibility(1); updateImplVisibility(2);";
    
    ss << "document.querySelectorAll('.tab-btn').forEach(btn => {";
    ss << "  btn.addEventListener('click', () => {";
    ss << "    const tab = btn.dataset.tab;";
    ss << "    document.querySelectorAll('.tab-btn').forEach(b => b.classList.remove('active'));";
    ss << "    document.querySelectorAll('.tab-content').forEach(c => c.classList.remove('active'));";
    ss << "    btn.classList.add('active');";
    ss << "    document.getElementById(tab + '-tab').classList.add('active');";
    ss << "    refreshState();";
    ss << "  });";
    ss << "});";
    
    ss << "refreshState();";
    ss << "setInterval(refreshState, 3000);";
    ss << "</script>";
    
    ss << "</body>";
    ss << "</html>";
    
    return ss.str();
}

// ==================== HTTP ОТВЕТЫ ====================

string response(int code, const string& contentType, const string& body) 
{
    stringstream ss;
    ss << "HTTP/1.1 " << code;
    if (code == 200) ss << " OK";
    else if (code == 404) ss << " Not Found";
    else if (code == 400) ss << " Bad Request";
    ss << "\r\n";
    ss << "Content-Type: " << contentType << "\r\n";
    ss << "Access-Control-Allow-Origin: *\r\n";
    ss << "Content-Length: " << body.length() << "\r\n";
    ss << "Connection: close\r\n";
    ss << "\r\n";
    ss << body;
    return ss.str();
}

string responseHtml(int code, const string& htmlBody) 
{
    return response(code, "text/html; charset=utf-8", htmlBody);
}

string responseJson(int code, const string& jsonBody) 
{
    return response(code, "application/json", jsonBody);
}

// ==================== ОБРАБОТЧИКИ API ====================

string getStateJson() 
{
    stringstream ss;
    ss << "{";
    ss << "\"dynamicArrayCreated\":" << (g_appData.dynamicArray.isCreated() ? "true" : "false") << ",";
    ss << "\"dynamicArraySize\":" << g_appData.dynamicArray.getSize() << ",";
    ss << "\"dynamicArrayValues\":\"" << escapeHtml(g_appData.dynamicArray.getValuesString()) << "\",";
    ss << "\"linkedListCreated\":" << (g_appData.linkedList.isCreated() ? "true" : "false") << ",";
    ss << "\"linkedListSize\":" << g_appData.linkedList.getSize() << ",";
    ss << "\"linkedListValues\":\"" << escapeHtml(g_appData.linkedList.getValuesString()) << "\",";
    ss << "\"seq1Values\":\"" << escapeHtml(g_appData.sequence.getValuesString(1)) << "\",";
    ss << "\"seq1Size\":" << g_appData.sequence.getSize(1) << ",";
    ss << "\"seq1Type\":\"" << escapeHtml(g_appData.sequence.getTypeString(1)) << "\",";
    ss << "\"seq2Values\":\"" << escapeHtml(g_appData.sequence.getValuesString(2)) << "\",";
    ss << "\"seq2Size\":" << g_appData.sequence.getSize(2) << ",";
    ss << "\"seq2Type\":\"" << escapeHtml(g_appData.sequence.getTypeString(2)) << "\",";
    ss << "\"concatResult\":\"" << escapeHtml(g_appData.sequence.getConcatResult()) << "\"";
    ss << "}";
    return ss.str();
}

int weightedSum(int acc, int val, size_t idx) 
{
    return acc + val * (idx + 1);
}

// ==================== ОБРАБОТКА ЗАПРОСОВ ====================

void handleRequest(const string& request, string& responseStr) 
{
    stringstream ss(request);
    string method, path, version;
    ss >> method >> path >> version;
    
    if (method == "OPTIONS") 
    {
        responseStr = response(200, "text/plain", "");
        return;
    }
    
    if (method == "GET" && (path == "/" || path == "/index.html")) 
    {
        responseStr = responseHtml(200, renderFullPage());
        return;
    }
    
    if (method == "GET" && path == "/matrix") 
    {
        responseStr = responseHtml(200, renderMatrixPage());
        return;
    }
    
    if (method == "GET" && path == "/functional-test") 
    {
        responseStr = responseHtml(200, renderFunctionalTestPage());
        return;
    }
    
    if (method == "GET" && path == "/state") 
    {
        responseStr = responseJson(200, getStateJson());
        return;
    }
    
        // ==================== POST /api/matrix ====================
    if (method == "POST" && path == "/api/matrix") 
    {
        size_t bodyPos = request.find("\r\n\r\n");
        string body = (bodyPos != string::npos) ? request.substr(bodyPos + 4) : "";
        
        map<string, string> params;
        stringstream ssBody(body);
        string pair;
        while (getline(ssBody, pair, '&')) 
        {
            size_t eqPos = pair.find('=');
            if (eqPos != string::npos) 
            {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                string decoded;
                for (size_t i = 0; i < value.length(); i++) {
                    if (value[i] == '+') decoded += ' ';
                    else if (value[i] == '%' && i + 2 < value.length()) {
                        string hex = value.substr(i + 1, 2);
                        decoded += (char)strtol(hex.c_str(), NULL, 16);
                        i += 2;
                    } else decoded += value[i];
                }
                params[key] = decoded;
            }
        }
        
        string operation = params["operation"];
        
        // Простая функция проверки, является ли строка целым числом
        auto isInt = [](const string& s) -> bool {
            if (s.empty()) return false;
            size_t start = 0;
            if (s[0] == '-') start = 1;
            if (start >= s.length()) return false;
            for (size_t i = start; i < s.length(); i++) {
                if (!isdigit(s[i])) return false;
            }
            return true;
        };
        
        // Проверка на слишком большое число
        auto isTooLarge = [](const string& s) -> bool {
            string temp = s;
            if (temp[0] == '-') temp = temp.substr(1);
            if (temp.length() > 10) return true;
            if (temp.length() == 10) {
                string maxInt = "2147483647";
                for (size_t i = 0; i < temp.length(); i++) {
                    if (temp[i] > maxInt[i]) return true;
                    if (temp[i] < maxInt[i]) break;
                }
            }
            return false;
        };
        
        // Парсинг матрицы с проверкой
        auto parseMatrix = [&](const string& dataStr, int rows, int cols, const string& name, string& error) -> vector<vector<int>> {
            vector<vector<int>> result(rows, vector<int>(cols, 0));
            vector<int> numbers;
            string num;
            
            for (char c : dataStr) {
                if (c == '-' || (c >= '0' && c <= '9')) {
                    num += c;
                } else if (!num.empty()) {
                    if (!isInt(num)) {
                        error = "❌ Ошибка: в матрице " + name + " обнаружен некорректный символ в числе '" + num + "'";
                        return {};
                    }
                    if (isTooLarge(num)) {
                        error = "❌ Ошибка: в матрице " + name + " число '" + num + "' слишком большое (допустимый диапазон: -2147483648...2147483647)";
                        return {};
                    }
                    numbers.push_back(atoi(num.c_str()));
                    num.clear();
                }
            }
            if (!num.empty()) {
                if (!isInt(num)) {
                    error = "❌ Ошибка: в матрице " + name + " обнаружен некорректный символ в числе '" + num + "'";
                    return {};
                }
                if (isTooLarge(num)) {
                    error = "❌ Ошибка: в матрице " + name + " число '" + num + "' слишком большое";
                    return {};
                }
                numbers.push_back(atoi(num.c_str()));
            }
            
            if ((int)numbers.size() < rows * cols) {
                error = "❌ Ошибка: в матрице " + name + " недостаточно данных (" + to_string(numbers.size()) + " чисел, ожидается " + to_string(rows * cols) + ")";
                return {};
            }
            
            int idx = 0;
            for (int i = 0; i < rows && idx < (int)numbers.size(); i++) {
                for (int j = 0; j < cols && idx < (int)numbers.size(); j++) {
                    result[i][j] = numbers[idx++];
                }
            }
            return result;
        };

        auto isDouble = [](const string& s) -> bool {
            if (s.empty()) return false;
            char* end = nullptr;
            strtod(s.c_str(), &end);
            return end != s.c_str() && *end == '\0';
        };

        auto parseDoubleMatrix = [&](const string& dataStr, int rows, int cols, const string& name, string& error) -> vector<vector<double>> {
           vector<vector<double>> result(rows, vector<double>(cols, 0.0));
           vector<double> numbers;
           string num;


           for (char c : dataStr) {
               if (c == '-' || c == '.' || (c >= '0' && c <= '9')) {
                   num += c;
               } else if (!num.empty()) {
                   if (!isDouble(num)) {
                       error = "❌ Ошибка: в матрице " + name + " некорректное число '" + num + "'";
                       return {};
                   }
                   numbers.push_back(strtod(num.c_str(), nullptr));
                   num.clear();
               }
           }
           if (!num.empty()) {
               if (!isDouble(num)) {
                   error = "❌ Ошибка: в матрице " + name + " некорректное число '" + num + "'";
                   return {};
               }
               numbers.push_back(strtod(num.c_str(), nullptr));
           }


           if ((int)numbers.size() < rows * cols) {
               error = "❌ Ошибка: в матрице " + name + " недостаточно данных (" + to_string(numbers.size()) + " чисел, ожидается " + to_string(rows * cols) + ")";
               return {};
           }


           int idx = 0;
           for (int i = 0; i < rows && idx < (int)numbers.size(); i++) {
               for (int j = 0; j < cols && idx < (int)numbers.size(); j++) {
                   result[i][j] = numbers[idx++];
               }
           }
           return result;
        };

        auto parseComplexNumber = [&](const string& raw, Complex& value) -> bool {
           string s = trim(raw);
           if (s.empty()) return false;


           if (s.back() != 'i') {
               if (!isDouble(s)) return false;
               value = Complex(strtod(s.c_str(), nullptr), 0.0);
               return true;
           }


           string body = s.substr(0, s.length() - 1);
           if (body.empty() || body == "+") {
               value = Complex(0.0, 1.0);
               return true;
           }
           if (body == "-") {
               value = Complex(0.0, -1.0);
               return true;
           }


           size_t signPos = string::npos;
           for (size_t i = 1; i < body.length(); i++) {
               if (body[i] == '+' || body[i] == '-') {
                   signPos = i;
               }
           }


           if (signPos == string::npos) {
               if (!isDouble(body)) return false;
               value = Complex(0.0, strtod(body.c_str(), nullptr));
               return true;
           }


           string realPart = body.substr(0, signPos);
           string imagPart = body.substr(signPos);
           if (!isDouble(realPart)) return false;


           double imag = 0.0;
           if (imagPart == "+") {
               imag = 1.0;
           } else if (imagPart == "-") {
               imag = -1.0;
           } else {
               if (!isDouble(imagPart)) return false;
               imag = strtod(imagPart.c_str(), nullptr);
           }


           value = Complex(strtod(realPart.c_str(), nullptr), imag);
           return true;
       };


        auto parseComplexMatrix = [&](const string& dataStr, int rows, int cols, const string& name, string& error) -> vector<vector<Complex>> {
            vector<vector<Complex>> result(rows, vector<Complex>(cols));
            vector<string> tokens;
            string token;
            bool inQuotes = false;
            bool hasQuotes = false;


            for (char c : dataStr) {
                if (c == '"') {
                    hasQuotes = true;
                    if (inQuotes) {
                        tokens.push_back(token);
                        token.clear();
                    }
                    inQuotes = !inQuotes;
                } else if (inQuotes) {
                    token += c;
                }
            }


            if (!hasQuotes) {
                token.clear();
                for (char c : dataStr) {
                    if (c == '[' || c == ']' || c == ',' || isspace((unsigned char)c)) {
                        if (!token.empty()) {
                            tokens.push_back(token);
                            token.clear();
                        }
                    } else {
                        token += c;
                    }
                }
                if (!token.empty()) tokens.push_back(token);
            }


           if ((int)tokens.size() < rows * cols) {
               error = "❌ Ошибка: в матрице " + name + " недостаточно данных (" + to_string(tokens.size()) + " чисел, ожидается " + to_string(rows * cols) + ")";
               return {};
           }


           int idx = 0;
           for (int i = 0; i < rows && idx < (int)tokens.size(); i++) {
               for (int j = 0; j < cols && idx < (int)tokens.size(); j++) {
                   Complex parsed;
                   if (!parseComplexNumber(tokens[idx], parsed)) {
                       error = "❌ Ошибка: в матрице " + name + " некорректное комплексное число '" + tokens[idx] + "'";
                       return {};
                   }
                   result[i][j] = parsed;
                   idx++;
               }
           }


           return result;
       };
        


        string result;
        string errorMsg;

        try 
        {
            int rows1 = atoi(params["rows1"].c_str());
            int cols1 = atoi(params["cols1"].c_str());
            string data1Str = params["data1"];
            
            // Проверка размеров
            if (rows1 <= 0 || rows1 > 10 || cols1 <= 0 || cols1 > 10) {
                result = "❌ Ошибка: размеры матрицы должны быть от 1 до 10";
                responseStr = responseHtml(200, result);
                return;
            }

            if (operation == "add")
            {
                int rows2 = atoi(params["rows2"].c_str());
                int cols2 = atoi(params["cols2"].c_str());
                string data2Str = params["data2"];


                if (rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
                    result = "❌ Ошибка: размеры матрицы №2 должны быть от 1 до 10";
                    responseStr = responseHtml(200, result);
                    return;
                }


                auto m1Double = parseDoubleMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                auto m2Double = parseDoubleMatrix(data2Str, rows2, cols2, "№2", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                if (rows1 != rows2 || cols1 != cols2) {
                    result = "❌ Ошибка: Размеры матриц не совпадают! (" + to_string(rows1) + "x" + to_string(cols1) + " vs " + to_string(rows2) + "x" + to_string(cols2) + ")";
                } else {
                    double** arr1 = new double*[rows1];
                    double** arr2 = new double*[rows2];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new double[cols1];
                        arr2[i] = new double[cols2];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1Double[i][j];
                            arr2[i][j] = m2Double[i][j];
                        }
                    }
                    RectangularMatrix<double> mat1(arr1, rows1, cols1);
                    RectangularMatrix<double> mat2(arr2, rows2, cols2);
                    auto res = mat1 + mat2;
                    result = "Результат сложения:\n" + res.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    for (int i = 0; i < rows2; i++) delete[] arr2[i];
                    delete[] arr1;
                    delete[] arr2;
                }
                

                responseStr = responseHtml(200, result);
                return;
            }


            if (operation == "multiply_scalar")
            {
                string scalarStr = params["param1"];
                if (!isDouble(scalarStr)) {
                    result = "❌ Ошибка: скаляр содержит некорректные символы";
                    responseStr = responseHtml(200, result);
                    return;
                }
                double scalar = strtod(scalarStr.c_str(), nullptr);


                auto m1Double = parseDoubleMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                double** arr1 = new double*[rows1];
                for (int i = 0; i < rows1; i++) {
                    arr1[i] = new double[cols1];
                    for (int j = 0; j < cols1; j++) {
                        arr1[i][j] = m1Double[i][j];
                    }
                }
                RectangularMatrix<double> mat1(arr1, rows1, cols1);
                auto res = mat1 * scalar;
                result = "Умножение на " + scalarStr + ":\n" + res.ToString();
                for (int i = 0; i < rows1; i++) delete[] arr1[i];
                delete[] arr1;


                responseStr = responseHtml(200, result);
                return;
            }

            if (operation == "norm")
            {
                auto m1Double = parseDoubleMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                double** arr1 = new double*[rows1];
                for (int i = 0; i < rows1; i++) {
                    arr1[i] = new double[cols1];
                    for (int j = 0; j < cols1; j++) {
                        arr1[i][j] = m1Double[i][j];
                    }
                }
                RectangularMatrix<double> mat1(arr1, rows1, cols1);
                double norm = mat1.Norm();
                result = "Норма матрицы: " + to_string(norm) + "\n\n";
                result += mat1.ToString();
                for (int i = 0; i < rows1; i++) delete[] arr1[i];
                delete[] arr1;


                responseStr = responseHtml(200, result);
                return;
            }

            if (operation == "swap_rows")
            {
                int row1 = atoi(params["param1"].c_str());
                int row2 = atoi(params["param2"].c_str());




                auto m1Complex = parseComplexMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }




                if (row1 < 0 || row1 >= rows1 || row2 < 0 || row2 >= rows1) {
                    result = "❌ Ошибка: номера строк должны быть от 0 до " + to_string(rows1 - 1);
                } else {
                    Complex** arr1 = new Complex*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new Complex[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1Complex[i][j];
                        }
                    }
                    RectangularMatrix<Complex> mat1(arr1, rows1, cols1);
                    mat1.SwapRows(row1, row2);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }




                responseStr = responseHtml(200, result);
                return;
            }

            if (operation == "multiply_row")
            {
                int row = atoi(params["param1"].c_str());
                string scalarStr = params["param2"];


                if (!isDouble(scalarStr)) {
                    result = "❌ Ошибка: скаляр содержит некорректные символы";
                    responseStr = responseHtml(200, result);
                    return;
                }
                double scalar = strtod(scalarStr.c_str(), nullptr);


                auto m1Double = parseDoubleMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                if (row < 0 || row >= rows1) {
                    result = "❌ Ошибка: номер строки должен быть от 0 до " + to_string(rows1 - 1);
                } else {
                    double** arr1 = new double*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new double[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1Double[i][j];
                        }
                    }
                    RectangularMatrix<double> mat1(arr1, rows1, cols1);
                    mat1.MultiplyRow(row, scalar);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }


                responseStr = responseHtml(200, result);
                return;
            }
            if (operation == "add_row")
            {
                int from = atoi(params["param1"].c_str());
                int to = atoi(params["param2"].c_str());
                string lambdaStr = params.find("param3") != params.end() ? params["param3"] : "1";


                if (!isDouble(lambdaStr)) {
                    result = "❌ Ошибка: коэффициент содержит некорректные символы";
                    responseStr = responseHtml(200, result);
                    return;
                }
                double lambda = strtod(lambdaStr.c_str(), nullptr);


                auto m1Double = parseDoubleMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                if (from < 0 || from >= rows1 || to < 0 || to >= rows1) {
                    result = "❌ Ошибка: номера строк должны быть от 0 до " + to_string(rows1 - 1);
                } else {
                    double** arr1 = new double*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new double[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1Double[i][j];
                        }
                    }
                    RectangularMatrix<double> mat1(arr1, rows1, cols1);
                    mat1.AddRowToRow(from, to, lambda);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }


                responseStr = responseHtml(200, result);
                return;
            }
            if (operation == "swap_cols")
            {
                int col1 = atoi(params["param1"].c_str());
                int col2 = atoi(params["param2"].c_str());


                auto m1Double = parseDoubleMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                if (col1 < 0 || col1 >= cols1 || col2 < 0 || col2 >= cols1) {
                    result = "❌ Ошибка: номера столбцов должны быть от 0 до " + to_string(cols1 - 1);
                } else {
                    double** arr1 = new double*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new double[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1Double[i][j];
                        }
                    }
                    RectangularMatrix<double> mat1(arr1, rows1, cols1);
                    mat1.SwapCols(col1, col2);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }


                responseStr = responseHtml(200, result);
                return;
            }
            if (operation == "multiply_col")
            {
                int col = atoi(params["param1"].c_str());
                string scalarStr = params["param2"];


                if (!isDouble(scalarStr)) {
                    result = "❌ Ошибка: скаляр содержит некорректные символы";
                    responseStr = responseHtml(200, result);
                    return;
                }
                double scalar = strtod(scalarStr.c_str(), nullptr);


                auto m1Double = parseDoubleMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                if (col < 0 || col >= cols1) {
                    result = "❌ Ошибка: номер столбца должен быть от 0 до " + to_string(cols1 - 1);
                } else {
                    double** arr1 = new double*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new double[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1Double[i][j];
                        }
                    }
                    RectangularMatrix<double> mat1(arr1, rows1, cols1);
                    mat1.MultiplyCol(col, scalar);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }


                responseStr = responseHtml(200, result);
                return;
            }
            if (operation == "add_col")
            {
                int from = atoi(params["param1"].c_str());
                int to = atoi(params["param2"].c_str());
                string lambdaStr = params.find("param3") != params.end() ? params["param3"] : "1";


                if (!isDouble(lambdaStr)) {
                    result = "❌ Ошибка: коэффициент содержит некорректные символы";
                    responseStr = responseHtml(200, result);
                    return;
                }
                double lambda = strtod(lambdaStr.c_str(), nullptr);


                auto m1Double = parseDoubleMatrix(data1Str, rows1, cols1, "№1", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }


                if (from < 0 || from >= cols1 || to < 0 || to >= cols1) {
                    result = "❌ Ошибка: номера столбцов должны быть от 0 до " + to_string(cols1 - 1);
                } else {
                    double** arr1 = new double*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new double[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1Double[i][j];
                        }
                    }
                    RectangularMatrix<double> mat1(arr1, rows1, cols1);
                    mat1.AddColToCol(from, to, lambda);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }


                responseStr = responseHtml(200, result);
                return;
            }







            
            // Парсим первую матрицу
            auto m1 = parseMatrix(data1Str, rows1, cols1, "№1", errorMsg);
            if (!errorMsg.empty()) {
                responseStr = responseHtml(200, errorMsg);
                return;
            }
            
            if (operation == "norm") 
            {
                int** arr1 = new int*[rows1];
                for (int i = 0; i < rows1; i++) {
                    arr1[i] = new int[cols1];
                    for (int j = 0; j < cols1; j++) {
                        arr1[i][j] = m1[i][j];
                    }
                }
                RectangularMatrix<int> mat1(arr1, rows1, cols1);
                int norm = mat1.Norm();
                result = "Норма матрицы: " + to_string(norm) + "\n\n";
                for (int i = 0; i < rows1; i++) {
                    result += "[";
                    for (int j = 0; j < cols1; j++) {
                        if (j > 0) result += ", ";
                        result += to_string(m1[i][j]);
                    }
                    result += "]\n";
                }
                for (int i = 0; i < rows1; i++) delete[] arr1[i];
                delete[] arr1;
            }
            else if (operation == "multiply_scalar") 
            {
                string scalarStr = params["param1"];
                if (!isInt(scalarStr)) {
                    result = "❌ Ошибка: скаляр содержит некорректные символы";
                    responseStr = responseHtml(200, result);
                    return;
                }
                if (isTooLarge(scalarStr)) {
                    result = "❌ Ошибка: скаляр слишком большой";
                    responseStr = responseHtml(200, result);
                    return;
                }
                int scalar = atoi(scalarStr.c_str());
                
                int** arr1 = new int*[rows1];
                for (int i = 0; i < rows1; i++) {
                    arr1[i] = new int[cols1];
                    for (int j = 0; j < cols1; j++) {
                        arr1[i][j] = m1[i][j];
                    }
                }
                RectangularMatrix<int> mat1(arr1, rows1, cols1);
                auto res = mat1 * scalar;
                result = "Умножение на " + to_string(scalar) + ":\n" + res.ToString();
                for (int i = 0; i < rows1; i++) delete[] arr1[i];
                delete[] arr1;
            }
            else if (operation == "add") 
            {
                int rows2 = atoi(params["rows2"].c_str());
                int cols2 = atoi(params["cols2"].c_str());
                string data2Str = params["data2"];
                
                if (rows2 <= 0 || rows2 > 10 || cols2 <= 0 || cols2 > 10) {
                    result = "❌ Ошибка: размеры матрицы №2 должны быть от 1 до 10";
                    responseStr = responseHtml(200, result);
                    return;
                }
                
                auto m2 = parseMatrix(data2Str, rows2, cols2, "№2", errorMsg);
                if (!errorMsg.empty()) {
                    responseStr = responseHtml(200, errorMsg);
                    return;
                }
                
                if (rows1 != rows2 || cols1 != cols2) {
                    result = "❌ Ошибка: Размеры матриц не совпадают! (" + to_string(rows1) + "x" + to_string(cols1) + " vs " + to_string(rows2) + "x" + to_string(cols2) + ")";
                } else {
                    int** arr1 = new int*[rows1];
                    int** arr2 = new int*[rows2];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new int[cols1];
                        arr2[i] = new int[cols2];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1[i][j];
                            arr2[i][j] = m2[i][j];
                        }
                    }
                    RectangularMatrix<int> mat1(arr1, rows1, cols1);
                    RectangularMatrix<int> mat2(arr2, rows2, cols2);
                    auto res = mat1 + mat2;
                    result = "Результат сложения:\n" + res.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    for (int i = 0; i < rows2; i++) delete[] arr2[i];
                    delete[] arr1;
                    delete[] arr2;
                }
            }
            else if (operation == "swap_rows") 
            {
                int row1 = atoi(params["param1"].c_str());
                int row2 = atoi(params["param2"].c_str());
                
                if (row1 < 0 || row1 >= rows1 || row2 < 0 || row2 >= rows1) {
                    result = "❌ Ошибка: номера строк должны быть от 0 до " + to_string(rows1 - 1);
                } else {
                    int** arr1 = new int*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new int[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1[i][j];
                        }
                    }
                    RectangularMatrix<int> mat1(arr1, rows1, cols1);
                    mat1.SwapRows(row1, row2);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }
            }
            else if (operation == "multiply_row") 
            {
                int row = atoi(params["param1"].c_str());
                string scalarStr = params["param2"];
                
                if (!isInt(scalarStr)) {
                    result = "❌ Ошибка: скаляр содержит некорректные символы";
                    responseStr = responseHtml(200, result);
                    return;
                }
                if (isTooLarge(scalarStr)) {
                    result = "❌ Ошибка: скаляр слишком большой";
                    responseStr = responseHtml(200, result);
                    return;
                }
                int scalar = atoi(scalarStr.c_str());
                
                if (row < 0 || row >= rows1) {
                    result = "❌ Ошибка: номер строки должен быть от 0 до " + to_string(rows1 - 1);
                } else {
                    int** arr1 = new int*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new int[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1[i][j];
                        }
                    }
                    RectangularMatrix<int> mat1(arr1, rows1, cols1);
                    mat1.MultiplyRow(row, scalar);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }
            }
            else if (operation == "add_row") 
            {
                int from = atoi(params["param1"].c_str());
                int to = atoi(params["param2"].c_str());
                int lambda = params.find("param3") != params.end() ? atoi(params["param3"].c_str()) : 1;
                
                if (from < 0 || from >= rows1 || to < 0 || to >= rows1) {
                    result = "❌ Ошибка: номера строк должны быть от 0 до " + to_string(rows1 - 1);
                } else {
                    int** arr1 = new int*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new int[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1[i][j];
                        }
                    }
                    RectangularMatrix<int> mat1(arr1, rows1, cols1);
                    mat1.AddRowToRow(from, to, lambda);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }
            }
            else if (operation == "swap_cols") 
            {
                int col1 = atoi(params["param1"].c_str());
                int col2 = atoi(params["param2"].c_str());
                
                if (col1 < 0 || col1 >= cols1 || col2 < 0 || col2 >= cols1) {
                    result = "❌ Ошибка: номера столбцов должны быть от 0 до " + to_string(cols1 - 1);
                } else {
                    int** arr1 = new int*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new int[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1[i][j];
                        }
                    }
                    RectangularMatrix<int> mat1(arr1, rows1, cols1);
                    mat1.SwapCols(col1, col2);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }
            }
            else if (operation == "multiply_col") 
            {
                int col = atoi(params["param1"].c_str());
                string scalarStr = params["param2"];
                
                if (!isInt(scalarStr)) {
                    result = "❌ Ошибка: скаляр содержит некорректные символы";
                    responseStr = responseHtml(200, result);
                    return;
                }
                if (isTooLarge(scalarStr)) {
                    result = "❌ Ошибка: скаляр слишком большой";
                    responseStr = responseHtml(200, result);
                    return;
                }
                int scalar = atoi(scalarStr.c_str());
                
                if (col < 0 || col >= cols1) {
                    result = "❌ Ошибка: номер столбца должен быть от 0 до " + to_string(cols1 - 1);
                } else {
                    int** arr1 = new int*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new int[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1[i][j];
                        }
                    }
                    RectangularMatrix<int> mat1(arr1, rows1, cols1);
                    mat1.MultiplyCol(col, scalar);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }
            }
            else if (operation == "add_col") 
            {
                int from = atoi(params["param1"].c_str());
                int to = atoi(params["param2"].c_str());
                int lambda = params.find("param3") != params.end() ? atoi(params["param3"].c_str()) : 1;
                
                if (from < 0 || from >= cols1 || to < 0 || to >= cols1) {
                    result = "❌ Ошибка: номера столбцов должны быть от 0 до " + to_string(cols1 - 1);
                } else {
                    int** arr1 = new int*[rows1];
                    for (int i = 0; i < rows1; i++) {
                        arr1[i] = new int[cols1];
                        for (int j = 0; j < cols1; j++) {
                            arr1[i][j] = m1[i][j];
                        }
                    }
                    RectangularMatrix<int> mat1(arr1, rows1, cols1);
                    mat1.AddColToCol(from, to, lambda);
                    result = mat1.ToString();
                    for (int i = 0; i < rows1; i++) delete[] arr1[i];
                    delete[] arr1;
                }
            }
            else 
            {
                result = "❌ Неизвестная операция: " + operation;
            }
        }
        catch (const exception& e) 
        {
            result = "❌ Ошибка: " + string(e.what());
        }
        
        responseStr = responseHtml(200, result);
        return;
    }
    
    // ==================== POST /api/functional ====================
    if (method == "POST" && path == "/api/functional") 
    {
        size_t bodyPos = request.find("\r\n\r\n");
        string body = (bodyPos != string::npos) ? request.substr(bodyPos + 4) : "";
        
        map<string, string> params;
        stringstream ssBody(body);
        string pair;
        while (getline(ssBody, pair, '&')) 
        {
            size_t eqPos = pair.find('=');
            if (eqPos != string::npos) {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                params[key] = value;
            }
        }
        
        string operation = params["operation"];
        string inputStr = params["input"];
        
        vector<int> numbers;
        stringstream ssInput(inputStr);
        string num;
        while (getline(ssInput, num, ',')) {
            try { numbers.push_back(stoi(trim(num))); } catch(...) {}
        }
        
        if (numbers.empty()) {
            responseStr = responseHtml(200, "Ошибка: не удалось распознать числа");
            return;
        }
        
        MutableArraySequence<int> seq(numbers.data(), numbers.size());
        string result;
        
        try {
            if (operation == "map_square") {
                auto mapped = Map<int, int>(&seq, [](int x) { return x * x; });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += to_string(mapped->Get(i));
                }
                result += "]";
                delete mapped;
            }
            else if (operation == "map_double") {
                auto mapped = Map<int, int>(&seq, [](int x) { return x * 2; });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += to_string(mapped->Get(i));
                }
                result += "]";
                delete mapped;
            }
            else if (operation == "filter_even") {
                auto filtered = Filter<int>(&seq, [](int x) { return x % 2 == 0; });
                if (filtered->GetLength() == 0) {
                    result = "[] (нет чётных чисел)";
                } else {
                    result = "[";
                    for (size_t i = 0; i < filtered->GetLength(); i++) {
                        if (i > 0) result += ", ";
                        result += to_string(filtered->Get(i));
                    }
                    result += "]";
                }
                delete filtered;
            }
            else if (operation == "filter_positive") {
                auto filtered = Filter<int>(&seq, [](int x) { return x > 0; });
                if (filtered->GetLength() == 0) {
                    result = "[] (нет положительных чисел)";
                } else {
                    result = "[";
                    for (size_t i = 0; i < filtered->GetLength(); i++) {
                        if (i > 0) result += ", ";
                        result += to_string(filtered->Get(i));
                    }
                    result += "]";
                }
                delete filtered;
            }
            else if (operation == "reduce_sum") {
                int sum = Reduce<int, int>(&seq, 0, [](int acc, int x) { return acc + x; });
                result = "Сумма: " + to_string(sum);
            }
            else if (operation == "reduce_product") {
                long long product = Reduce<int, long long>(&seq, 1, [](long long acc, int x) { return acc * x; });
                result = "Произведение: " + to_string(product);
            }
            else if (operation == "reduce_max") {
                int max = Reduce<int, int>(&seq, numbers[0], [](int acc, int x) { return (x > acc) ? x : acc; });
                result = "Максимум: " + to_string(max);
            }
            else if (operation == "reduce_min") {
                int min = Reduce<int, int>(&seq, numbers[0], [](int acc, int x) { return (x < acc) ? x : acc; });
                result = "Минимум: " + to_string(min);
            }
            else {
                result = "Неизвестная операция";
            }
        }
        catch (const exception& e) {
            result = "Ошибка: " + string(e.what());
        }
        
        responseStr = responseHtml(200, result);
        return;
    }
    
    // ==================== POST /api/dynamicarray ====================
    if (method == "POST" && path == "/api/dynamicarray") 
    {
        size_t bodyPos = request.find("\r\n\r\n");
        string body = (bodyPos != string::npos) ? request.substr(bodyPos + 4) : "";
        
        map<string, string> params;
        stringstream ssBody(body);
        string pair;
        while (getline(ssBody, pair, '&')) 
        {
            size_t eqPos = pair.find('=');
            if (eqPos != string::npos) {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                for (char& c : value) if (c == '+') c = ' ';
                params[key] = value;
            }
        }
        
        string msg;
        auto it = params.find("action");
        if (it == params.end()) msg = "Ошибка: Не указано действие";
        else if (it->second == "create") { g_appData.dynamicArray.create(); msg = "Массив создан"; }
        else if (it->second == "append") {
            if (!g_appData.dynamicArray.isCreated()) msg = "Сначала создайте массив";
            else {
                string valStr = trim(params["value"]);
                string error = validateValue(valStr, false);
                if (!error.empty()) msg = error;
                else {
                    int val = stoi(valStr);
                    g_appData.dynamicArray.append(val);
                    msg = "Добавлено " + to_string(val);
                }
            }
        }
        else if (it->second == "prepend") {
            if (!g_appData.dynamicArray.isCreated()) msg = "Сначала создайте массив";
            else {
                string valStr = trim(params["value"]);
                string error = validateValue(valStr, false);
                if (!error.empty()) msg = error;
                else {
                    int val = stoi(valStr);
                    g_appData.dynamicArray.prepend(val);
                    msg = "Добавлено " + to_string(val) + " в начало";
                }
            }
        }
        else if (it->second == "insert") {
            if (!g_appData.dynamicArray.isCreated()) msg = "Сначала создайте массив";
            else {
                string idxStr = trim(params["index"]);
                string valStr = trim(params["value"]);
                string idxError = validateIndex(idxStr, g_appData.dynamicArray.getSize(), true);
                if (!idxError.empty()) msg = idxError;
                else {
                    string valError = validateValue(valStr, false);
                    if (!valError.empty()) msg = valError;
                    else {
                        size_t idx = stoul(idxStr);
                        int val = stoi(valStr);
                        g_appData.dynamicArray.insertAt(val, idx);
                        msg = "Вставлено " + to_string(val) + " по индексу " + to_string(idx);
                    }
                }
            }
        }
        else if (it->second == "set") {
            if (!g_appData.dynamicArray.isCreated()) msg = "Сначала создайте массив";
            else if (g_appData.dynamicArray.getSize() == 0) msg = "Массив пуст";
            else {
                string idxStr = trim(params["index"]);
                string valStr = trim(params["value"]);
                string idxError = validateIndex(idxStr, g_appData.dynamicArray.getSize(), false);
                if (!idxError.empty()) msg = idxError;
                else {
                    string valError = validateValue(valStr, false);
                    if (!valError.empty()) msg = valError;
                    else {
                        size_t idx = stoul(idxStr);
                        int val = stoi(valStr);
                        g_appData.dynamicArray.set(idx, val);
                        msg = "Изменён элемент по индексу " + to_string(idx);
                    }
                }
            }
        }
        else if (it->second == "get") {
            if (!g_appData.dynamicArray.isCreated()) msg = "Сначала создайте массив";
            else if (g_appData.dynamicArray.getSize() == 0) msg = "Массив пуст";
            else {
                string idxStr = trim(params["index"]);
                string idxError = validateIndex(idxStr, g_appData.dynamicArray.getSize(), false);
                if (!idxError.empty()) msg = idxError;
                else {
                    size_t idx = stoul(idxStr);
                    int val = g_appData.dynamicArray.get(idx);
                    msg = "Значение по индексу " + to_string(idx) + ": " + to_string(val);
                }
            }
        }
        else if (it->second == "clear") { g_appData.dynamicArray.clear(); msg = "Массив очищен"; }
        else msg = "Неизвестное действие";
        
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // ==================== POST /api/linkedlist ====================
    if (method == "POST" && path == "/api/linkedlist") 
    {
        size_t bodyPos = request.find("\r\n\r\n");
        string body = (bodyPos != string::npos) ? request.substr(bodyPos + 4) : "";
        
        map<string, string> params;
        stringstream ssBody(body);
        string pair;
        while (getline(ssBody, pair, '&')) 
        {
            size_t eqPos = pair.find('=');
            if (eqPos != string::npos) {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                for (char& c : value) if (c == '+') c = ' ';
                params[key] = value;
            }
        }
        
        string msg;
        auto it = params.find("action");
        if (it == params.end()) msg = "Ошибка: Не указано действие";
        else if (it->second == "create") { g_appData.linkedList.create(); msg = "Список создан"; }
        else if (it->second == "append") {
            if (!g_appData.linkedList.isCreated()) msg = "Сначала создайте список";
            else {
                string valStr = trim(params["value"]);
                string error = validateValue(valStr, false);
                if (!error.empty()) msg = error;
                else {
                    int val = stoi(valStr);
                    g_appData.linkedList.append(val);
                    msg = "Добавлено " + to_string(val);
                }
            }
        }
        else if (it->second == "prepend") {
            if (!g_appData.linkedList.isCreated()) msg = "Сначала создайте список";
            else {
                string valStr = trim(params["value"]);
                string error = validateValue(valStr, false);
                if (!error.empty()) msg = error;
                else {
                    int val = stoi(valStr);
                    g_appData.linkedList.prepend(val);
                    msg = "Добавлено " + to_string(val) + " в начало";
                }
            }
        }
        else if (it->second == "insert") {
            if (!g_appData.linkedList.isCreated()) msg = "Сначала создайте список";
            else {
                string idxStr = trim(params["index"]);
                string valStr = trim(params["value"]);
                string idxError = validateIndex(idxStr, g_appData.linkedList.getSize(), true);
                if (!idxError.empty()) msg = idxError;
                else {
                    string valError = validateValue(valStr, false);
                    if (!valError.empty()) msg = valError;
                    else {
                        size_t idx = stoul(idxStr);
                        int val = stoi(valStr);
                        g_appData.linkedList.insertAt(val, idx);
                        msg = "Вставлено " + to_string(val) + " по индексу " + to_string(idx);
                    }
                }
            }
        }
        else if (it->second == "get") {
            if (!g_appData.linkedList.isCreated()) msg = "Сначала создайте список";
            else if (g_appData.linkedList.getSize() == 0) msg = "Список пуст";
            else {
                string idxStr = trim(params["index"]);
                string idxError = validateIndex(idxStr, g_appData.linkedList.getSize(), false);
                if (!idxError.empty()) msg = idxError;
                else {
                    size_t idx = stoul(idxStr);
                    int val = g_appData.linkedList.get(idx);
                    msg = "Значение по индексу " + to_string(idx) + ": " + to_string(val);
                }
            }
        }
        else if (it->second == "getFirst") {
            if (!g_appData.linkedList.isCreated()) msg = "Сначала создайте список";
            else if (g_appData.linkedList.getSize() == 0) msg = "Список пуст";
            else {
                int val = g_appData.linkedList.getFirst();
                msg = "Первый элемент: " + to_string(val);
            }
        }
        else if (it->second == "getLast") {
            if (!g_appData.linkedList.isCreated()) msg = "Сначала создайте список";
            else if (g_appData.linkedList.getSize() == 0) msg = "Список пуст";
            else {
                int val = g_appData.linkedList.getLast();
                msg = "Последний элемент: " + to_string(val);
            }
        }
        else if (it->second == "clear") { g_appData.linkedList.clear(); msg = "Список очищен"; }
        else msg = "Неизвестное действие";
        
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // ==================== POST /api/sequence ====================
    if (method == "POST" && path == "/api/sequence") 
    {
        size_t bodyPos = request.find("\r\n\r\n");
        string body = (bodyPos != string::npos) ? request.substr(bodyPos + 4) : "";
        
        map<string, string> params;
        stringstream ssBody(body);
        string pair;
        while (getline(ssBody, pair, '&')) 
        {
            size_t eqPos = pair.find('=');
            if (eqPos != string::npos) {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                for (char& c : value) if (c == '+') c = ' ';
                params[key] = value;
            }
        }
        
        string msg;
        auto it = params.find("action");
        if (it == params.end()) msg = "Не указано действие";
        else if (it->second == "create") {
            int id = stoi(params["id"]);
            string type = params["type"];
            string impl = (params.find("impl") != params.end()) ? params["impl"] : "";
            g_appData.sequence.createSequence(id, type, impl);
            msg = "Последовательность " + to_string(id) + " создана (тип: " + type + ")";
        }
        else if (it->second == "clear") {
            int id = stoi(params["id"]);
            g_appData.sequence.clear(id);
            msg = "Последовательность " + to_string(id) + " очищена";
        }
        else if (it->second == "append") {
            int id = stoi(params["id"]);
            string valStr = trim(params["value"]);
            string typeStr = g_appData.sequence.getTypeString(id);
            bool isBit = (typeStr.find("BitSequence") != string::npos);
            string error = validateValue(valStr, isBit);
            if (!error.empty()) msg = error;
            else {
                int val = stoi(valStr);
                string result = g_appData.sequence.append(id, val);
                if (!result.empty()) msg = result;
                else msg = "Добавлено " + to_string(val);
            }
        }
        else if (it->second == "prepend") {
            int id = stoi(params["id"]);
            string valStr = trim(params["value"]);
            string typeStr = g_appData.sequence.getTypeString(id);
            bool isBit = (typeStr.find("BitSequence") != string::npos);
            string error = validateValue(valStr, isBit);
            if (!error.empty()) msg = error;
            else {
                int val = stoi(valStr);
                string result = g_appData.sequence.prepend(id, val);
                if (!result.empty()) msg = result;
                else msg = "Добавлено " + to_string(val) + " в начало";
            }
        }
        else if (it->second == "insert") {
            int id = stoi(params["id"]);
            string idxStr = trim(params["index"]);
            string valStr = trim(params["value"]);
            string typeStr = g_appData.sequence.getTypeString(id);
            bool isBit = (typeStr.find("BitSequence") != string::npos);
            size_t currentSize = g_appData.sequence.getSize(id);
            string idxError = validateIndex(idxStr, currentSize, true);
            if (!idxError.empty()) msg = idxError;
            else {
                string valError = validateValue(valStr, isBit);
                if (!valError.empty()) msg = valError;
                else {
                    size_t idx = stoul(idxStr);
                    int val = stoi(valStr);
                    string result = g_appData.sequence.insertAt(id, val, idx);
                    if (!result.empty()) msg = result;
                    else msg = "Вставлено " + to_string(val) + " по индексу " + to_string(idx);
                }
            }
        }
        else if (it->second == "get") {
            int id = stoi(params["id"]);
            string idxStr = trim(params["index"]);
            if (g_appData.sequence.getSize(id) == 0) msg = "Последовательность пуста";
            else {
                string idxError = validateIndex(idxStr, g_appData.sequence.getSize(id), false);
                if (!idxError.empty()) msg = idxError;
                else {
                    size_t idx = stoul(idxStr);
                    int val = g_appData.sequence.get(id, idx);
                    msg = "Значение по индексу " + to_string(idx) + ": " + to_string(val);
                }
            }
        }
        else if (it->second == "getFirst") {
            int id = stoi(params["id"]);
            if (g_appData.sequence.getSize(id) == 0) msg = "Последовательность пуста";
            else {
                int val = g_appData.sequence.getFirst(id);
                msg = "Первый элемент: " + to_string(val);
            }
        }
        else if (it->second == "getLast") {
            int id = stoi(params["id"]);
            if (g_appData.sequence.getSize(id) == 0) msg = "Последовательность пуста";
            else {
                int val = g_appData.sequence.getLast(id);
                msg = "Последний элемент: " + to_string(val);
            }
        }
        else msg = "Неизвестное действие";
        
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // ==================== POST /api/sequence/subsequence ====================
    if (method == "POST" && path == "/api/sequence/subsequence") 
    {
        size_t bodyPos = request.find("\r\n\r\n");
        string body = (bodyPos != string::npos) ? request.substr(bodyPos + 4) : "";
        
        map<string, string> params;
        stringstream ssBody(body);
        string pair;
        while (getline(ssBody, pair, '&')) 
        {
            size_t eqPos = pair.find('=');
            if (eqPos != string::npos) {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                for (char& c : value) if (c == '+') c = ' ';
                params[key] = value;
            }
        }
        
        string msg;
        int id = stoi(params["id"]);
        string startStr = trim(params["start"]);
        string endStr = trim(params["end"]);
        
        if (g_appData.sequence.getSize(id) == 0) {
            msg = "Последовательность пуста";
        } else {
            string startError = validateIndex(startStr, g_appData.sequence.getSize(id), false);
            if (!startError.empty()) msg = startError;
            else {
                string endError = validateIndex(endStr, g_appData.sequence.getSize(id), false);
                if (!endError.empty()) msg = endError;
                else {
                    size_t start = stoul(startStr);
                    size_t end = stoul(endStr);
                    if (start > end) msg = "Начальный индекс не может быть больше конечного";
                    else {
                        string result = g_appData.sequence.getSubsequence(id, start, end);
                        msg = result;
                    }
                }
            }
        }
        
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // ==================== POST /api/sequence/concat ====================
    if (method == "POST" && path == "/api/sequence/concat") {
        string msg = g_appData.sequence.concat();
        responseStr = responseHtml(200, msg);
        return;
    }
    
    responseStr = response(404, "text/html", "<h1>404 - Страница не найдена</h1>");
}

// ==================== ОБРАБОТЧИК КЛИЕНТОВ ====================

DWORD WINAPI clientHandler(LPVOID arg) 
{
    SOCKET clientSocket = *(SOCKET*)arg;
    delete (SOCKET*)arg;
    
    char buffer[65536];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead > 0) 
    {
        buffer[bytesRead] = '\0';
        string request(buffer);
        string responseStr;
        
        try 
        {
            handleRequest(request, responseStr);
            send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
        } 
        catch (const exception& e) 
        {
            string errorMsg = "Ошибка сервера: " + string(e.what());
            string httpResponse = responseHtml(500, errorMsg);
            send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);
        }
    }
    
    closesocket(clientSocket);
    return 0;
}

// ==================== MAIN ====================

int main() 
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        cerr << "WSAStartup failed" << endl;
        return 1;
    }
    
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) 
    {
        cerr << "Failed to create socket" << endl;
        WSACleanup();
        return 1;
    }
    
    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
    
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8082);
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
    {
        cerr << "Failed to bind to port 8082" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    if (listen(serverSocket, 10) == SOCKET_ERROR) 
    {
        cerr << "Failed to listen" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "========================================" << endl;
    cout << "  Server started!" << endl;
    cout << "  Open browser at: http://localhost:8082" << endl;
    cout << "  Matrix page: http://localhost:8082/matrix" << endl;
    cout << "========================================" << endl;
    
    while (true) 
    {
        struct sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        SOCKET* clientSocket = new SOCKET;
        *clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        
        if (*clientSocket != INVALID_SOCKET) 
        {
            CreateThread(NULL, 0, clientHandler, clientSocket, 0, NULL);
        } 
        else 
        {
            delete clientSocket;
        }
    }
    
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}