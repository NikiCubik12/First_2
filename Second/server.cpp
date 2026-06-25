#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <map>
#include <fstream>
#include <climits>
#include <cctype>
#include <memory>
#include <regex>
#include <algorithm>

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

using namespace std;

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

bool safeStoi(const string& str, int& result) 
{
    if (str.empty()) return false;
    
    try 
    {
        size_t pos = 0;
        long long val = stoll(str, &pos);
        
        if (pos != str.length() || val < INT_MIN || val > INT_MAX) 
        {
            return false;
        }
        
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
        return "❌ Ошибка: значение содержит недопустимые спецсимволы (!@#$%^&*() и т.д.)";
    }
    
    if (hasLetters(trimmed)) {
        return "❌ Ошибка: значение содержит буквы. Допустимы только цифры" + string(isBitSequence ? " (0 или 1)" : "");
    }
    
    int val;
    if (isBitSequence) {
        if (!safeStoi(trimmed, val) || (val != 0 && val != 1)) {
            return "❌ Ошибка: для BitSequence допустимы только значения 0 или 1";
        }
    } else {
        if (!safeStoi(trimmed, val)) {
            return "❌ Ошибка: значение должно быть целым числом от " + to_string(INT_MIN) + " до " + to_string(INT_MAX);
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
        return "❌ Ошибка: индекс содержит буквы. Допустимы только цифры";
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
    
    ~DynamicArrayManager() 
    {
        if (data) delete data;
    }
    
    void create() 
    {
        if (data) delete data;
        data = new DynamicArray<T>();
        created = true;
    }
    
    void clear() 
    {
        if (created && data) 
        {
            data->Resize(0);
        }
    }
    
    bool isCreated() const { return created; }
    
    void append(const T& value) 
    {
        if (created && data) data->Append(value);
    }
    
    void prepend(const T& value) 
    {
        if (created && data) data->Prepend(value);
    }
    
    void insertAt(const T& value, size_t index) 
    {
        if (created && data) data->InsertAt(value, index);
    }
    
    void set(size_t index, const T& value) 
    {
        if (created && data) data->Set(index, value);
    }
    
    T get(size_t index) const 
    {
        if (created && data) return data->Get(index);
        throw runtime_error("Array not created");
    }
    
    size_t getSize() const 
    {
        return created && data ? data->GetSize() : 0;
    }
    
    string getValuesString() const 
    {
        stringstream ss;
        if (created && data) 
        {
            ss << "[";
            for (size_t i = 0; i < data->GetSize(); i++) 
            {
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
    
    ~LinkedListManager() 
    {
        if (data) delete data;
    }
    
    void create() 
    {
        if (data) delete data;
        data = new LinkedList<T>();
        created = true;
    }
    
    void clear() 
    {
        if (created && data) 
        {
            data->ClearList();
        }
    }
    
    bool isCreated() const { return created; }
    
    void append(const T& value) 
    {
        if (created && data) data->Append(value);
    }
    
    void prepend(const T& value) 
    {
        if (created && data) data->Prepend(value);
    }
    
    void insertAt(const T& value, size_t index) 
    {
        if (created && data) data->InsertAt(value, index);
    }
    
    T get(size_t index) const 
    {
        if (created && data) return data->Get(index);
        throw runtime_error("List not created");
    }
    
    T getFirst() const 
    {
        if (created && data) return data->GetFirst();
        throw runtime_error("List not created");
    }
    
    T getLast() const 
    {
        if (created && data) return data->GetLast();
        throw runtime_error("List not created");
    }
    
    size_t getSize() const 
    {
        return created && data ? data->GetLength() : 0;
    }
    
    string getValuesString() const 
    {
        stringstream ss;
        if (created && data) 
        {
            ss << "[";
            for (size_t i = 0; i < data->GetLength(); i++) 
            {
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

        DynamicArray<Sequence<int>*> tempIntSeqs;
        DynamicArray<BitSequence*> tempBitSeqs;
        
        SequenceData() : intSeq(nullptr), bitSeq(nullptr), type(ARRAY_MUTABLE), isBit(false) {}
        
        ~SequenceData() {
            clear();
            for (size_t i = 0; i < tempIntSeqs.GetSize(); i++)
            {
                delete tempIntSeqs.Get(i);
            }
            for (size_t i = 0; i < tempBitSeqs.GetSize(); i++)
            {
                delete tempBitSeqs.Get(i);
            }
        }
        
        void clear() {
            if (intSeq) delete intSeq;
            if (bitSeq) delete bitSeq;
            intSeq = nullptr;
            bitSeq = nullptr;
            isBit = false;
        }
        
        size_t getSize() const {
            if (isBit) return bitSeq ? bitSeq->GetLength() : 0;
            return intSeq ? intSeq->GetLength() : 0;
        }
        
        string getValuesString() const {
            stringstream ss;
            ss << "[";
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
            if (isBit) {
                if (value != 0 && value != 1) return "❌ Ошибка: для BitSequence допустимы только значения 0 или 1";
                if (bitSeq) bitSeq->Append(value);
            } else {
                if (intSeq) intSeq->Append(value);
            }
            return "";
        }
        
        string prepend(int value) {
            if (isBit) {
                if (value != 0 && value != 1) return "❌ Ошибка: для BitSequence допустимы только значения 0 или 1";
                if (bitSeq) bitSeq->Prepend(value);
            } else {
                if (intSeq) intSeq->Prepend(value);
            }
            return "";
        }
        
        string insertAt(int value, size_t index) {
            if (isBit) {
                if (value != 0 && value != 1) return "❌ Ошибка: для BitSequence допустимы только значения 0 или 1";
                if (bitSeq) bitSeq->InsertAt(value, index);
            } else {
                if (intSeq) intSeq->InsertAt(value, index);
            }
            return "";
        }
        
        int get(size_t index) const {
            if (isBit) return bitSeq ? bitSeq->Get(index) : 0;
            return intSeq ? intSeq->Get(index) : 0;
        }
        
        int getFirst() const {
            if (isBit) return bitSeq ? bitSeq->GetFirst() : 0;
            return intSeq ? intSeq->GetFirst() : 0;
        }
        
        int getLast() const {
            if (isBit) return bitSeq ? bitSeq->GetLast() : 0;
            return intSeq ? intSeq->GetLast() : 0;
        }
        
        string getSubsequence(size_t start, size_t end) {
            if (!intSeq && !bitSeq) return "❌ Ошибка: последовательность не создана";
            if (start > end) return "❌ Ошибка: начальный индекс не может быть больше конечного";
            if (end >= getSize()) return "❌ Ошибка: конечный индекс выходит за границы (максимум " + to_string(getSize() - 1) + ")";
            
            stringstream ss;
            ss << "[";
            
            if (isBit) {
                BitSequence* sub = bitSeq->GetSubsequence(start, end);
                tempBitSeqs.Append(sub);
                for (size_t i = 0; i < sub->GetLength(); i++) {
                    if (i > 0) ss << ", ";
                    ss << sub->Get(i);
                }
            } else {
                Sequence<int>* sub = intSeq->GetSubsequence(start, end);
                tempIntSeqs.Append(sub);
                for (size_t i = 0; i < sub->GetLength(); i++) {
                    if (i > 0) ss << ", ";
                    ss << sub->Get(i);
                }
            }
            
            ss << "]";
            return ss.str();
        }
        
        void clearTempSubsequences()
        {
            for (size_t i = 0; i < tempIntSeqs.GetSize(); i++)
            {
                delete tempIntSeqs.Get(i);
            }
            for (size_t i = 0; i < tempBitSeqs.GetSize(); i++)
            {
                delete tempBitSeqs.Get(i);
            }
            tempIntSeqs.Resize(0);
            tempBitSeqs.Resize(0);
        }
    };
    
    SequenceData seq1;
    SequenceData seq2;
    SequenceData concatResult;

    string getBaseType(const string& type) const {
        string base = type;
        size_t pos = base.find(" (");
        if (pos != string::npos) {
            base = base.substr(0, pos);
        }
        return base;
    }
    
public:
    SequenceManager() {}
    
    ~SequenceManager() 
    {
        seq1.clear();
        seq2.clear();
        concatResult.clear();
    }
    
    void createSequence(int id, const string& type, const string& impl) 
    {
        SequenceData* target = (id == 1) ? &seq1 : &seq2;
        target->clear();
        target->clearTempSubsequences();
        
        if (type == "ArraySequence") 
        {
            if (impl == "Mutable") 
            {
                target->intSeq = new MutableArraySequence<int>();
                target->type = ARRAY_MUTABLE;
            } 
            else 
            {
                target->intSeq = new ImmutableArraySequence<int>();
                target->type = ARRAY_IMMUTABLE;
            }
            target->isBit = false;
        }
        else if (type == "ListSequence") 
        {
            target->intSeq = new ListSequence<int>();
            target->type = LIST;
            target->isBit = false;
        }
        else if (type == "BitSequence") 
        {
            target->bitSeq = new BitSequence();
            target->type = BIT;
            target->isBit = true;
        }
    }
    
    bool isCreated(int id) const 
    {
        const SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->intSeq != nullptr || target->bitSeq != nullptr;
    }
    
    string append(int id, int value) 
    {
        SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->append(value);
    }
    
    string prepend(int id, int value) 
    {
        SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->prepend(value);
    }
    
    string insertAt(int id, int value, size_t index) 
    {
        SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->insertAt(value, index);
    }
    
    int get(int id, size_t index) const 
    {
        const SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->get(index);
    }
    
    int getFirst(int id) const 
    {
        const SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->getFirst();
    }
    
    int getLast(int id) const 
    {
        const SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->getLast();
    }
    
    size_t getSize(int id) const 
    {
        const SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->getSize();
    }
    
    string getValuesString(int id) const 
    {
        const SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->getValuesString();
    }
    
    string getTypeString(int id) const 
    {
        const SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->getTypeString();
    }
    
    string getSubsequence(int id, size_t start, size_t end) 
    {
        SequenceData* target = (id == 1) ? &seq1 : &seq2;
        return target->getSubsequence(start, end);
    }
    
    string concat() 
    {
        // ========== 1. ПРОВЕРКА: Обе последовательности созданы ==========
        if (!seq1.intSeq && !seq1.bitSeq) {
            return "❌ Ошибка: Первая последовательность не создана. Нажмите 'Создать'.";
        }
        if (!seq2.intSeq && !seq2.bitSeq) {
            return "❌ Ошибка: Вторая последовательность не создана. Нажмите 'Создать'.";
        }
        
        // ========== 2. ПРОВЕРКА: BitSequence только с BitSequence ==========
        if (seq1.isBit != seq2.isBit) {
            string first = seq1.isBit ? "BitSequence" : getBaseType(seq1.getTypeString());
            string second = seq2.isBit ? "BitSequence" : getBaseType(seq2.getTypeString());
            return "❌ Ошибка: Нельзя объединить " + first + " с " + second + ".\n"
                "BitSequence может объединяться только с BitSequence.";
        }
        
        // ========== 3. ПРОВЕРКА: ArraySequence только с ArraySequence ==========
        string base1 = getBaseType(seq1.getTypeString());
        string base2 = getBaseType(seq2.getTypeString());
        
        if (!seq1.isBit && base1 != base2) {
            return "❌ Ошибка: Нельзя объединить " + base1 + " с " + base2 + ".\n"
                "ArraySequence можно объединять только с ArraySequence.\n"
                "ListSequence можно объединять только с ListSequence.";
        }
        
        // ========== 4. ОБЪЕДИНЕНИЕ ==========
        concatResult.clear();
        concatResult.clearTempSubsequences();
        
        string resultStr;
        
        // BitSequence + BitSequence
        if (seq1.isBit) {
            BitSequence* result = new BitSequence();
            for (size_t i = 0; i < seq1.bitSeq->GetLength(); i++) {
                result->Append(seq1.bitSeq->Get(i));
            }
            for (size_t i = 0; i < seq2.bitSeq->GetLength(); i++) {
                result->Append(seq2.bitSeq->Get(i));
            }
            concatResult.bitSeq = result;
            concatResult.isBit = true;
            concatResult.type = BIT;
            resultStr = concatResult.getValuesString();
            
            return "✅ Результат объединения (BitSequence + BitSequence): " + resultStr;
        }
        
        // ListSequence + ListSequence
        if (base1 == "ListSequence") {
            Sequence<int>* result = seq1.intSeq->Concat(seq2.intSeq);
            concatResult.intSeq = result;
            concatResult.isBit = false;
            concatResult.type = LIST;
            resultStr = concatResult.getValuesString();
            
            return "✅ Результат объединения (ListSequence + ListSequence): " + resultStr;
        }
        
        // ArraySequence + ArraySequence
        // ArraySequence с ArraySequence (учёт Mutable/Immutable)
        bool isFirstMutable = (seq1.getTypeString().find("Mutable") != string::npos);
        bool isSecondMutable = (seq2.getTypeString().find("Mutable") != string::npos);
        
        // Если хотя бы одна Immutable → результат Immutable
        bool resultIsMutable = (isFirstMutable && isSecondMutable);
        string resultType = resultIsMutable ? "Mutable" : "Immutable";
        
        // Сообщение если смешиваются Mutable и Immutable
        string warning = "";
        if (isFirstMutable != isSecondMutable) {
            string firstType = isFirstMutable ? "Mutable" : "Immutable";
            string secondType = isSecondMutable ? "Mutable" : "Immutable";
            warning = "\n⚠️ Внимание: Объединяются " + firstType + " и " + secondType + 
                    " последовательности. Результат будет Immutable (неизменяемым).";
        }
        
        Sequence<int>* result = nullptr;
        if (resultIsMutable) {
            // Оба Mutable → создаём Mutable результат через Concat
            result = seq1.intSeq->Concat(seq2.intSeq);
            concatResult.type = ARRAY_MUTABLE;
        } else {
            // Хотя бы один Immutable → создаём Immutable результат вручную
            ImmutableArraySequence<int>* immutableResult = new ImmutableArraySequence<int>();
            for (size_t i = 0; i < seq1.intSeq->GetLength(); i++) {
                immutableResult->Append(seq1.intSeq->Get(i));
            }
            for (size_t i = 0; i < seq2.intSeq->GetLength(); i++) {
                immutableResult->Append(seq2.intSeq->Get(i));
            }
            result = immutableResult;
            concatResult.type = ARRAY_IMMUTABLE;
        }
        
        concatResult.intSeq = result;
        concatResult.isBit = false;
        resultStr = concatResult.getValuesString();
        
        return "✅ Результат объединения (ArraySequence + ArraySequence): " + resultStr + 
            "\n📌 Тип результата: ArraySequence (" + resultType + ")" + warning;
    }
    
    string getConcatResult() const 
    {
        if (!concatResult.intSeq && !concatResult.bitSeq) return "Нет результата";
        return concatResult.getValuesString();
    }
    
    void clearConcatResult() 
    {
        concatResult.clear();
    }
    
    void clear(int id) 
    {
        SequenceData* target = (id == 1) ? &seq1 : &seq2;
        target->clear();
        target->clearTempSubsequences();
    }
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


string renderFunctionalTestPage()
{
    stringstream ss;
    ss << "<!DOCTYPE html>";
    ss << "<html lang=\"ru\">";
    ss << "<head>";
    ss << "<meta charset=\"UTF-8\">";
    ss << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
    ss << "<title>Демострация дополнительного функционала</title>";
    ss << "<style>";
    ss << "* { margin: 0; padding: 0; box-sizing: border-box; }";
    ss << "body { font-family: 'Segoe UI', sans-serif; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); min-height: 100vh; padding: 20px; }";
    ss << ".container { max-width: 1200px; margin: 0 auto; background: white; border-radius: 24px; padding: 32px; }";
    ss << "h1 { color: #667eea; border-bottom: 3px solid #667eea; padding-bottom: 10px; margin-bottom: 20px; }";
    ss << "h2 { color: #764ba2; margin: 20px 0 10px 0; }";
    ss << ".input-group { margin-bottom: 20px; }";
    ss << "label { display: block; font-weight: 600; margin-bottom: 5px; color: #555; }";
    ss << "input[type=text], input[type=number] { width: 100%; padding: 10px; border: 2px solid #dee2e6; border-radius: 8px; font-size: 14px; }";
    ss << "input:focus { outline: none; border-color: #667eea; }";
    ss << "select { width: 100%; padding: 10px; border: 2px solid #dee2e6; border-radius: 8px; font-size: 14px; background: white; cursor: pointer; }";
    ss << "button { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; border: none; padding: 12px 24px; border-radius: 10px; font-size: 16px; cursor: pointer; transition: transform 0.2s; margin-top: 10px; }";
    ss << "button:hover { transform: translateY(-2px); }";
    ss << ".result { background: #f8f9fa; padding: 20px; border-radius: 12px; margin-top: 20px; border-left: 4px solid #28a745; font-family: monospace; font-size: 16px; word-break: break-all; }";
    ss << ".error { border-left-color: #dc3545; background: #fee; }";
    ss << ".back-link { display: inline-block; margin-top: 20px; color: #667eea; text-decoration: none; }";
    ss << ".function-list { background: #e8f4f8; padding: 15px; border-radius: 12px; margin-bottom: 20px; }";
    ss << ".function-list code { background: #fff; padding: 2px 6px; border-radius: 4px; }";
    ss << "</style>";
    ss << "</head>";
    ss << "<body>";
    ss << "<div class=\"container\">";
    ss << "<h1>Демострация дополнительного функционала</h1>";
    ss << "<p>Введите последовательность чисел и выберите операцию для проверки</p>";
    
    ss << "<div class=\"function-list\">";
    ss << "<strong>📋 Доступные операции:</strong><br>";
    ss << "• <code>Map</code> - возвести в квадрат<br>";
    ss << "• <code>Map</code> - умножить на 2<br>";
    ss << "• <code>Map</code> - сделать отрицательными<br>";
    ss << "• <code>Map</code> - модуль числа<br>";
    ss << "• <code>Map</code> - прибавить 10<br>";
    ss << "• <code>Map</code> - преобразовать в строку<br>";
    ss << "• <code>Filter</code> - оставить чётные<br>";
    ss << "• <code>Filter</code> - оставить положительные<br>";
    ss << "• <code>Filter</code> - оставить больше 5<br>";
    ss << "• <code>Reduce</code> - сумма<br>";
    ss << "• <code>Reduce</code> - произведение<br>";
    ss << "• <code>Reduce</code> - максимум<br>";
    ss << "• <code>Reduce</code> - минимум<br>";
    ss << "• <code>ReduceWithIndex</code> - взвешенная сумма<br>";
    ss << "• <code>MapWithIndex</code> - показать индекс<br>";
    ss << "• <code>TryGet</code> - получить по индексу<br>";
    ss << "• <code>TryFind</code> - найти первый > 5<br>";
    ss << "• <code>GetAllPrefixes</code> - все префиксы<br>";
    ss << "• <code>GetAllPostfixes</code> - все постфиксы<br>";
    ss << "</div>";
    
    ss << "<div class=\"input-group\">";
    ss << "<label>📋 Введите последовательность чисел (через запятую):</label>";
    ss << "<input type=\"text\" id=\"inputSeq\" placeholder=\"Пример: 1,2,3,4,5,6,7,8,9,10\" value=\"1,2,3,4,5,6,7,8,9,10\" ";
    ss << "pattern=\"[0-9,\\s\\-]+\" ";
    ss << "title=\"Только цифры, запятые, пробелы и знак минуса. Пример: 1,2,3 или -1,0,5\">";
    ss << "</div>";
    
    ss << "<div class=\"input-group\">";
    ss << "<label>🔧 Выберите операцию:</label>";
    ss << "<select id=\"operation\">";
    ss << "<option value=\"map_square\">Map: возвести в квадрат</option>";
    ss << "<option value=\"map_double\">Map: умножить на 2</option>";
    ss << "<option value=\"map_negative\">Map: сделать отрицательными</option>";
    ss << "<option value=\"map_abs\">Map: модуль числа</option>";
    ss << "<option value=\"map_add10\">Map: прибавить 10</option>";
    ss << "<option value=\"map_to_string\">Map: преобразовать в строку</option>";
    ss << "<option value=\"filter_even\">Filter: чётные числа</option>";
    ss << "<option value=\"filter_positive\">Filter: положительные числа</option>";
    ss << "<option value=\"filter_gt5\">Filter: больше 5</option>";
    ss << "<option value=\"reduce_sum\">Reduce: сумма</option>";
    ss << "<option value=\"reduce_product\">Reduce: произведение</option>";
    ss << "<option value=\"reduce_max\">Reduce: максимум</option>";
    ss << "<option value=\"reduce_min\">Reduce: минимум</option>";
    ss << "<option value=\"reducewithindex\">ReduceWithIndex: взвешенная сумма</option>";
    ss << "<option value=\"mapwithindex\">MapWithIndex: показать индекс</option>";
    ss << "<option value=\"tryget\">TryGet: получить по индексу</option>";
    ss << "<option value=\"tryfind\">TryFind: найти первый > 5</option>";
    ss << "<option value=\"prefixes\">GetAllPrefixes: все префиксы</option>";
    ss << "<option value=\"postfixes\">GetAllPostfixes: все постфиксы</option>";
    ss << "</select>";
    ss << "</div>";
    
    ss << "<div class=\"input-group\" id=\"indexGroup\" style=\"display:none;\">";
    ss << "<label>🔢 Введите индекс (для TryGet):</label>";
    ss << "<input type=\"number\" id=\"indexValue\" value=\"0\" min=\"0\">";
    ss << "</div>";
    
    ss << "<button onclick=\"execute()\">🚀 Выполнить</button>";
    ss << "<div class=\"result\" id=\"result\">Результат появится здесь...</div>";
    ss << "<a href=\"/\" class=\"back-link\">← Вернуться на главную</a>";
    ss << "</div>";
    
    ss << "<script>";
    ss << "document.getElementById('operation').addEventListener('change', function() {";
    ss << "  let indexGroup = document.getElementById('indexGroup');";
    ss << "  if (this.value === 'tryget') { indexGroup.style.display = 'block'; }";
    ss << "  else { indexGroup.style.display = 'none'; }";
    ss << "});";
    ss << "async function execute() {";
    ss << "  let input = document.getElementById('inputSeq').value;";
    ss << "  let operation = document.getElementById('operation').value;";
    ss << "  let index = document.getElementById('indexValue').value;";
    ss << "  let formData = new URLSearchParams();";
    ss << "  formData.append('input', input);";
    ss << "  formData.append('operation', operation);";
    ss << "  if (operation === 'tryget') formData.append('index', index);";
    ss << "  try {";
    ss << "    let response = await fetch('/api/functional', { method: 'POST', body: formData });";
    ss << "    let result = await response.text();";
    ss << "    let resultDiv = document.getElementById('result');";
    ss << "    if (result.includes('Ошибка') || result.includes('❌')) {";
    ss << "      resultDiv.className = 'result error';";
    ss << "    } else {";
    ss << "      resultDiv.className = 'result';";
    ss << "    }";
    ss << "    resultDiv.innerHTML = '<strong>📊 Результат:</strong><br>' + result;";
    ss << "  } catch(e) {";
    ss << "    let resultDiv = document.getElementById('result');";
    ss << "    resultDiv.className = 'result error';";
    ss << "    resultDiv.innerHTML = '❌ Ошибка: ' + e.message;";
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
    ss << "<h1>Лабораторная работа </h1>";

    ss << "<div style=\"text-align:center; margin: 16px 0;\">";
    ss << "<button class=\"demo-btn\" onclick=\"window.open('/functional-test', '_blank')\">🧪 Дополнительный функционал</button>";
    ss << "</div>";
    
    ss << "<div class=\"tabs\">";
    ss << "<button class=\"tab-btn active\" data-tab=\"dynamic\">📦 Динамический массив</button>";
    ss << "<button class=\"tab-btn\" data-tab=\"linked\">🔗 Связный список</button>";
    ss << "<button class=\"tab-btn\" data-tab=\"sequence\">📜 Последовательности </button>";
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
    return response(code, "text/html", htmlBody);
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
    ss << "\"seq1Created\":" << (g_appData.sequence.isCreated(1) ? "true" : "false") << ",";
    ss << "\"seq1Size\":" << g_appData.sequence.getSize(1) << ",";
    ss << "\"seq1Values\":\"" << escapeHtml(g_appData.sequence.getValuesString(1)) << "\",";
    ss << "\"seq1Type\":\"" << escapeHtml(g_appData.sequence.getTypeString(1)) << "\",";
    ss << "\"seq2Created\":" << (g_appData.sequence.isCreated(2) ? "true" : "false") << ",";
    ss << "\"seq2Size\":" << g_appData.sequence.getSize(2) << ",";
    ss << "\"seq2Values\":\"" << escapeHtml(g_appData.sequence.getValuesString(2)) << "\",";
    ss << "\"seq2Type\":\"" << escapeHtml(g_appData.sequence.getTypeString(2)) << "\",";
    ss << "\"concatResult\":\"" << escapeHtml(g_appData.sequence.getConcatResult()) << "\"";
    ss << "}";
    return ss.str();
}

// ==================== ОБРАБОТКА ЗАПРОСОВ ====================


int weightedSum(int acc, int val, size_t idx) 
{
    return acc + val * (idx + 1);
}

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
    
    if (method == "GET" && path == "/state") 
    {
        responseStr = responseJson(200, getStateJson());
        return;
    }

    if (method == "GET" && path == "/functional-test") 
    {
        responseStr = responseHtml(200, renderFunctionalTestPage());
        return;
    }

    if (method == "POST" && path == "/api/functional") 
    {
        size_t bodyPos = request.find("\r\n\r\n");
        string body = (bodyPos != string::npos) ? request.substr(bodyPos + 4) : "";
        
        string decodedBody;
        for (size_t i = 0; i < body.length(); i++) {
            if (body[i] == '%' && i + 2 < body.length()) {
                string hex = body.substr(i + 1, 2);
                char ch = (char)strtol(hex.c_str(), NULL, 16);
                decodedBody += ch;
                i += 2;
            } else if (body[i] == '+') {
                decodedBody += ' ';
            } else {
                decodedBody += body[i];
            }
        }
        
        map<string, string> params;
        stringstream ssBody(decodedBody);
        string pair;
        while (getline(ssBody, pair, '&')) 
        {
            size_t eqPos = pair.find('=');
            if (eqPos != string::npos) 
            {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                params[key] = value;
            }
        }
        
        string operation = params["operation"];
        string inputStr = params["input"];
        
        // ========== ВАЛИДАЦИЯ ВХОДНЫХ ДАННЫХ ==========
        
        // Проверка на пустую строку
        if (inputStr.empty()) {
            responseStr = responseHtml(200, "❌ Ошибка: поле ввода пустое. Введите числа через запятую. Пример: 1,2,3,4,5");
            return;
        }
        
        // Проверка на недопустимые символы (буквы, спецсимволы)
        bool hasInvalidChars = false;
        bool hasDot = false;
        string invalidChars;
        
        for (char c : inputStr) {
            if (c == '.') {
                hasDot = true;
            }
            else if (c == ',') {
                // запятая - разделитель, это нормально
                continue;
            }
            else if (c == ' ') {
                // пробелы пропускаем (они будут удалены trim)
                continue;
            }
            else if (c == '-') {
                // минус допустим для отрицательных чисел
                continue;
            }
            else if (!isdigit(c)) {
                hasInvalidChars = true;
                invalidChars += c;
            }
        }
        
        if (hasDot) {
            responseStr = responseHtml(200, "❌ Ошибка: обнаружена точка ('.'). Введите целые числа без десятичной части. Пример: 1,2,3, а не 1.5,2.5,3.5");
            return;
        }
        
        if (hasInvalidChars) {
            responseStr = responseHtml(200, "❌ Ошибка: введены недопустимые символы: " + invalidChars + "\n"
                                        "Разрешены только: цифры, запятые (,) и знак минуса (-) для отрицательных чисел.\n"
                                        "Пример правильного ввода: 1,2,3,4,5 или -1,0,5,10");
            return;
        }
        
        // Парсим числа
        DynamicArray<int> numbers;
        stringstream ssInput(inputStr);
        string num;
        
        while (getline(ssInput, num, ',')) 
        {
            // Удаляем пробелы в начале и конце
            size_t start = num.find_first_not_of(" \t\n\r");
            size_t end = num.find_last_not_of(" \t\n\r");
            if (start == string::npos) {
                // Пустая строка между запятыми (например: "1,,3")
                responseStr = responseHtml(200, "❌ Ошибка: обнаружена пустая запись между запятыми. Пример правильного ввода: 1,2,3,4,5");
                return;
            }
            string trimmed = num.substr(start, end - start + 1);
            
            // Проверка на пустой элемент
            if (trimmed.empty()) {
                responseStr = responseHtml(200, "❌ Ошибка: обнаружен пустой элемент. Проверьте ввод. Пример: 1,2,3,4,5");
                return;
            }
            
            // Проверка на несколько минусов подряд
            int minusCount = 0;
            for (char c : trimmed) {
                if (c == '-') minusCount++;
                if (minusCount > 1) {
                    responseStr = responseHtml(200, "❌ Ошибка: в числе не может быть несколько знаков минуса. Пример: -5, а не --5");
                    return;
                }
            }
            
            // Проверка, что минус не в середине
            if (trimmed.find('-') != string::npos && trimmed.find('-') != 0) {
                responseStr = responseHtml(200, "❌ Ошибка: знак минуса может быть только в начале числа. Пример: -5, а не 5- или 5-5");
                return;
            }
            
            // Преобразуем в число
            try {
                numbers.Append(stoi(trimmed));
            } catch (const std::out_of_range&) {
                responseStr = responseHtml(200, "❌ Ошибка: число слишком большое. Введите числа в диапазоне от -2147483648 до 2147483647");
                return;
            } catch (const std::invalid_argument&) {
                responseStr = responseHtml(200, "❌ Ошибка: '" + trimmed + "' не является числом. Введите только целые числа.");
                return;
            }
        }
        
        if (numbers.GetSize() == 0) {
            responseStr = responseHtml(200, "❌ Ошибка: не удалось распознать числа. Введите числа через запятую. Пример: 1,2,3,4,5");
            return;
        }
        
        // Проверка на слишком длинную последовательность
        if (numbers.GetSize() > 1000) {
            responseStr = responseHtml(200, "❌ Ошибка: слишком много элементов (максимум 1000). Введите меньше чисел.");
            return;
        }
        
        MutableArraySequence<int> seq(numbers.GetData(), numbers.GetSize());
        string result;
        
        try 
        {
            // ==================== MAP операции ====================
            if (operation == "map_square") 
            {
                auto mapped = Map<int, int>(&seq, [](int x) -> int { return x * x; });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += to_string(mapped->Get(i));
                }
                result += "]";
                delete mapped;
            }
            else if (operation == "map_double") 
            {
                auto mapped = Map<int, int>(&seq, [](int x) -> int { return x * 2; });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += to_string(mapped->Get(i));
                }
                result += "]";
                delete mapped;
            }
            else if (operation == "map_negative") 
            {
                auto mapped = Map<int, int>(&seq, [](int x) -> int { return -x; });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += to_string(mapped->Get(i));
                }
                result += "]";
                delete mapped;
            }
            else if (operation == "map_abs") 
            {
                auto mapped = Map<int, int>(&seq, [](int x) -> int { return (x < 0) ? -x : x; });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += to_string(mapped->Get(i));
                }
                result += "]";
                delete mapped;
            }
            else if (operation == "map_add10") 
            {
                auto mapped = Map<int, int>(&seq, [](int x) -> int { return x + 10; });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += to_string(mapped->Get(i));
                }
                result += "]";
                delete mapped;
            }
            else if (operation == "map_to_string") 
            {
                auto mapped = Map<int, string>(&seq, [](int x) -> string { 
                    return "'" + to_string(x) + "'"; 
                });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += mapped->Get(i);
                }
                result += "]";
                delete mapped;
            }
            
            // ==================== FILTER операции ====================
            else if (operation == "filter_even") 
            {
                auto filtered = Filter<int>(&seq, [](int x) -> bool { return x % 2 == 0; });
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
            else if (operation == "filter_positive") 
            {
                auto filtered = Filter<int>(&seq, [](int x) -> bool { return x > 0; });
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
            else if (operation == "filter_gt5") 
            {
                auto filtered = Filter<int>(&seq, [](int x) -> bool { return x > 5; });
                if (filtered->GetLength() == 0) {
                    result = "[] (нет чисел больше 5)";
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
            
            // ==================== REDUCE операции ====================
            else if (operation == "reduce_sum") 
            {
                int sum = Reduce<int, int>(&seq, 0, [](int acc, int x) -> int { return acc + x; });
                result = "Сумма: " + to_string(sum);
            }
            else if (operation == "reduce_product") 
            {
                long long product = Reduce<int, long long>(&seq, 1, [](long long acc, int x) -> long long { return acc * x; });
                result = "Произведение: " + to_string(product);
            }
            else if (operation == "reduce_max") 
            {
                if (numbers.GetSize() == 0) {
                    result = "❌ Нет элементов для поиска максимума";
                } else {
                    int max = Reduce<int, int>(&seq, numbers.GetData()[0], [](int acc, int x) -> int { return (x > acc) ? x : acc; });
                    result = "Максимум: " + to_string(max);
                }
            }
            else if (operation == "reduce_min") 
            {
                if (numbers.GetSize() == 0) {
                    result = "❌ Нет элементов для поиска минимума";
                } else {
                    int min = Reduce<int, int>(&seq, numbers.GetData()[0], [](int acc, int x) -> int { return (x < acc) ? x : acc; });
                    result = "Минимум: " + to_string(min);
                }
            }
            else if (operation == "reducewithindex") 
            {
                int weighted = ReduceWithIndex(&seq, 0, weightedSum);
                result = "Взвешенная сумма (элемент × позиция): " + to_string(weighted);
            }
            else if (operation == "mapwithindex") 
            {
                auto mapped = MapWithIndex<int, string>(&seq, [](int val, size_t idx) -> string {
                    return "[" + to_string(idx) + "]=" + to_string(val);
                });
                result = "[";
                for (size_t i = 0; i < mapped->GetLength(); i++) {
                    if (i > 0) result += ", ";
                    result += mapped->Get(i);
                }
                result += "]";
                delete mapped;
            }
            else if (operation == "tryget") 
            {
                int index = 0;
                if (params.find("index") != params.end()) {
                    try {
                        index = stoi(params["index"]);
                    } catch (...) {
                        result = "❌ Ошибка: индекс должен быть целым числом";
                        responseStr = responseHtml(200, result);
                        return;
                    }
                }
                
                if (index < 0) {
                    result = "❌ Ошибка: индекс не может быть отрицательным. Введите индекс от 0 до " + to_string(seq.GetLength() - 1);
                } else {
                    Option<int> opt = TryHelpers<int>::TryGet(&seq, index);
                    if (opt.IsSome()) {
                        result = "Элемент по индексу " + to_string(index) + ": " + to_string(opt.GetValue());
                    } else {
                        result = "❌ Элемент по индексу " + to_string(index) + " не найден (индекс вне границ, всего элементов: " + to_string(seq.GetLength()) + ")";
                    }
                }
            }
            else if (operation == "tryfind") 
            {
                Option<int> opt = TryHelpers<int>::TryFind(&seq, [](int x) -> bool { return x > 5; });
                if (opt.IsSome()) {
                    result = "Первый элемент > 5: " + to_string(opt.GetValue());
                } else {
                    result = "❌ Элемент > 5 не найден";
                }
            }
            else if (operation == "prefixes") 
            {
                auto prefixes = GetAllPrefixes(&seq);
                result = "";
                for (size_t i = 0; i < prefixes->GetLength(); i++) {
                    auto p = prefixes->Get(i);
                    result += "[";
                    for (size_t j = 0; j < p->GetLength(); j++) {
                        if (j > 0) result += ", ";
                        result += to_string(p->Get(j));
                    }
                    result += "] ";
                    delete p;
                }
                delete prefixes;
            }
            else if (operation == "postfixes") 
            {
                auto postfixes = GetAllPostfixes(&seq);
                result = "";
                for (size_t i = 0; i < postfixes->GetLength(); i++) {
                    auto p = postfixes->Get(i);
                    result += "[";
                    for (size_t j = 0; j < p->GetLength(); j++) {
                        if (j > 0) result += ", ";
                        result += to_string(p->Get(j));
                    }
                    result += "] ";
                    delete p;
                }
                delete postfixes;
            }
            else 
            {
                result = "❌ Неизвестная операция: " + operation;
            }
        }
        catch (const exception& e) 
        {
            result = "❌ Ошибка выполнения: " + string(e.what());
        }
        
        responseStr = responseHtml(200, result);
        return;
    }
        
    // POST /api/dynamicarray
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
            if (eqPos != string::npos) 
            {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                for (char& c : value) if (c == '+') c = ' ';
                params[key] = value;
            }
        }
        
        string msg;
        try 
        {
            auto it = params.find("action");
            if (it == params.end()) 
            {
                msg = "❌ Ошибка: Не указано действие";
            }
            else if (it->second == "create") 
            { 
                g_appData.dynamicArray.create(); 
                msg = "✅ Динамический массив создан"; 
            }
            else if (it->second == "append") 
            {
                if (!g_appData.dynamicArray.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте массив (кнопка 'Создать')";
                }
                else 
                {
                    string valStr = trim(params["value"]);
                    string error = validateValue(valStr, false);
                    if (!error.empty()) 
                    {
                        msg = error;
                    }
                    else 
                    {
                        int val = stoi(valStr);
                        g_appData.dynamicArray.append(val);
                        msg = "✅ Значение " + to_string(val) + " добавлено в конец";
                    }
                }
            }
            else if (it->second == "prepend") 
            {
                if (!g_appData.dynamicArray.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте массив (кнопка 'Создать')";
                }
                else 
                {
                    string valStr = trim(params["value"]);
                    string error = validateValue(valStr, false);
                    if (!error.empty()) 
                    {
                        msg = error;
                    }
                    else 
                    {
                        int val = stoi(valStr);
                        g_appData.dynamicArray.prepend(val);
                        msg = "✅ Значение " + to_string(val) + " добавлено в начало";
                    }
                }
            }
            else if (it->second == "insert") 
            {
                if (!g_appData.dynamicArray.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте массив (кнопка 'Создать')";
                }
                else 
                {
                    string idxStr = trim(params["index"]);
                    string valStr = trim(params["value"]);
                    
                    string idxError = validateIndex(idxStr, g_appData.dynamicArray.getSize(), true);
                    if (!idxError.empty()) 
                    {
                        msg = idxError;
                    }
                    else 
                    {
                        string valError = validateValue(valStr, false);
                        if (!valError.empty()) 
                        {
                            msg = valError;
                        }
                        else 
                        {
                            size_t idx = stoul(idxStr);
                            int val = stoi(valStr);
                            g_appData.dynamicArray.insertAt(val, idx);
                            msg = "✅ Значение " + to_string(val) + " вставлено по индексу " + to_string(idx);
                        }
                    }
                }
            }
            else if (it->second == "set") 
            {
                if (!g_appData.dynamicArray.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте массив (кнопка 'Создать')";
                }
                else if (g_appData.dynamicArray.getSize() == 0)
                {
                    msg = "❌ Ошибка: Массив пуст, нечего изменять";
                }
                else 
                {
                    string idxStr = trim(params["index"]);
                    string valStr = trim(params["value"]);
                    
                    string idxError = validateIndex(idxStr, g_appData.dynamicArray.getSize(), false);
                    if (!idxError.empty()) 
                    {
                        msg = idxError;
                    }
                    else 
                    {
                        string valError = validateValue(valStr, false);
                        if (!valError.empty()) 
                        {
                            msg = valError;
                        }
                        else 
                        {
                            size_t idx = stoul(idxStr);
                            int val = stoi(valStr);
                            g_appData.dynamicArray.set(idx, val);
                            msg = "✅ Значение по индексу " + to_string(idx) + " изменено на " + to_string(val);
                        }
                    }
                }
            }
            else if (it->second == "get") 
            {
                if (!g_appData.dynamicArray.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте массив (кнопка 'Создать')";
                }
                else if (g_appData.dynamicArray.getSize() == 0)
                {
                    msg = "❌ Ошибка: Массив пуст, нет элементов для получения";
                }
                else 
                {
                    string idxStr = trim(params["index"]);
                    string idxError = validateIndex(idxStr, g_appData.dynamicArray.getSize(), false);
                    if (!idxError.empty()) 
                    {
                        msg = idxError;
                    }
                    else 
                    {
                        size_t idx = stoul(idxStr);
                        int val = g_appData.dynamicArray.get(idx);
                        msg = "✅ Значение по индексу " + to_string(idx) + ": " + to_string(val);
                    }
                }
            }
            else if (it->second == "clear") 
            { 
                g_appData.dynamicArray.clear(); 
                msg = "✅ Массив очищен"; 
            }
            else 
            {
                msg = "❌ Ошибка: Неизвестное действие '" + it->second + "'";
            }
        } 
        catch (const exception& e) 
        { 
            msg = "❌ Ошибка: " + string(e.what()); 
        }
        
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // POST /api/linkedlist
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
            if (eqPos != string::npos) 
            {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                for (char& c : value) if (c == '+') c = ' ';
                params[key] = value;
            }
        }
        
        string msg;
        try 
        {
            auto it = params.find("action");
            if (it == params.end()) 
            {
                msg = "❌ Ошибка: Не указано действие";
            }
            else if (it->second == "create") 
            { 
                g_appData.linkedList.create(); 
                msg = "✅ Связный список создан"; 
            }
            else if (it->second == "append") 
            {
                if (!g_appData.linkedList.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте список (кнопка 'Создать')";
                }
                else 
                {
                    string valStr = trim(params["value"]);
                    string error = validateValue(valStr, false);
                    if (!error.empty()) 
                    {
                        msg = error;
                    }
                    else 
                    {
                        int val = stoi(valStr);
                        g_appData.linkedList.append(val);
                        msg = "✅ Значение " + to_string(val) + " добавлено в конец";
                    }
                }
            }
            else if (it->second == "prepend") 
            {
                if (!g_appData.linkedList.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте список (кнопка 'Создать')";
                }
                else 
                {
                    string valStr = trim(params["value"]);
                    string error = validateValue(valStr, false);
                    if (!error.empty()) 
                    {
                        msg = error;
                    }
                    else 
                    {
                        int val = stoi(valStr);
                        g_appData.linkedList.prepend(val);
                        msg = "✅ Значение " + to_string(val) + " добавлено в начало";
                    }
                }
            }
            else if (it->second == "insert") 
            {
                if (!g_appData.linkedList.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте список (кнопка 'Создать')";
                }
                else 
                {
                    string idxStr = trim(params["index"]);
                    string valStr = trim(params["value"]);
                    
                    string idxError = validateIndex(idxStr, g_appData.linkedList.getSize(), true);
                    if (!idxError.empty()) 
                    {
                        msg = idxError;
                    }
                    else 
                    {
                        string valError = validateValue(valStr, false);
                        if (!valError.empty()) 
                        {
                            msg = valError;
                        }
                        else 
                        {
                            size_t idx = stoul(idxStr);
                            int val = stoi(valStr);
                            g_appData.linkedList.insertAt(val, idx);
                            msg = "✅ Значение " + to_string(val) + " вставлено по индексу " + to_string(idx);
                        }
                    }
                }
            }
            else if (it->second == "get") 
            {
                if (!g_appData.linkedList.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте список (кнопка 'Создать')";
                }
                else if (g_appData.linkedList.getSize() == 0)
                {
                    msg = "❌ Ошибка: Список пуст, нет элементов для получения";
                }
                else 
                {
                    string idxStr = trim(params["index"]);
                    string idxError = validateIndex(idxStr, g_appData.linkedList.getSize(), false);
                    if (!idxError.empty()) 
                    {
                        msg = idxError;
                    }
                    else 
                    {
                        size_t idx = stoul(idxStr);
                        int val = g_appData.linkedList.get(idx);
                        msg = "✅ Значение по индексу " + to_string(idx) + ": " + to_string(val);
                    }
                }
            }
            else if (it->second == "getFirst") 
            {
                if (!g_appData.linkedList.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте список (кнопка 'Создать')";
                }
                else if (g_appData.linkedList.getSize() == 0)
                {
                    msg = "❌ Ошибка: Список пуст, нет первого элемента";
                }
                else 
                {
                    int val = g_appData.linkedList.getFirst();
                    msg = "✅ Первый элемент: " + to_string(val);
                }
            }
            else if (it->second == "getLast") 
            {
                if (!g_appData.linkedList.isCreated()) 
                {
                    msg = "❌ Ошибка: Сначала создайте список (кнопка 'Создать')";
                }
                else if (g_appData.linkedList.getSize() == 0)
                {
                    msg = "❌ Ошибка: Список пуст, нет последнего элемента";
                }
                else 
                {
                    int val = g_appData.linkedList.getLast();
                    msg = "✅ Последний элемент: " + to_string(val);
                }
            }
            else if (it->second == "clear") 
            { 
                g_appData.linkedList.clear(); 
                msg = "✅ Список очищен"; 
            }
            else 
            {
                msg = "❌ Ошибка: Неизвестное действие '" + it->second + "'";
            }
        } 
        catch (const exception& e) 
        { 
            msg = "❌ Ошибка: " + string(e.what()); 
        }
        
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // POST /api/sequence
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
            if (eqPos != string::npos) 
            {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                for (char& c : value) if (c == '+') c = ' ';
                params[key] = value;
            }
        }
        
        string msg;
        try 
        {
            auto it = params.find("action");
            if (it == params.end()) 
            {
                msg = "❌ Ошибка: Не указано действие";
            }
            else 
            {
                int id = 1;
                auto idIt = params.find("id");
                if (idIt != params.end() && !idIt->second.empty()) 
                {
                    id = stoi(idIt->second);
                }
                
                string action = it->second;
                
                if (action == "create") 
                {
                    string type = params["type"];
                    string impl = (params.find("impl") != params.end()) ? params["impl"] : "";
                    g_appData.sequence.createSequence(id, type, impl);
                    msg = "✅ Последовательность " + to_string(id) + " создана (тип: " + type + ")";
                }
                else if (action == "clear") 
                {
                    g_appData.sequence.clear(id);
                    msg = "✅ Последовательность " + to_string(id) + " очищена";
                }
                else if (action == "append") 
                {
                    if (!g_appData.sequence.isCreated(id)) 
                    {
                        msg = "❌ Ошибка: Сначала создайте последовательность (кнопка 'Создать')";
                    }
                    else 
                    {
                        string valStr = trim(params["value"]);
                        string typeStr = g_appData.sequence.getTypeString(id);
                        bool isBit = (typeStr.find("BitSequence") != string::npos);
                        
                        string error = validateValue(valStr, isBit);
                        if (!error.empty()) 
                        {
                            msg = error;
                        }
                        else 
                        {
                            int val = stoi(valStr);
                            string result = g_appData.sequence.append(id, val);
                            if (!result.empty()) 
                            {
                                msg = result;
                            }
                            else 
                            {
                                msg = "✅ Значение " + to_string(val) + " добавлено в конец последовательности " + to_string(id);
                            }
                        }
                    }
                }
                else if (action == "prepend") 
                {
                    if (!g_appData.sequence.isCreated(id)) 
                    {
                        msg = "❌ Ошибка: Сначала создайте последовательность (кнопка 'Создать')";
                    }
                    else 
                    {
                        string valStr = trim(params["value"]);
                        string typeStr = g_appData.sequence.getTypeString(id);
                        bool isBit = (typeStr.find("BitSequence") != string::npos);
                        
                        string error = validateValue(valStr, isBit);
                        if (!error.empty()) 
                        {
                            msg = error;
                        }
                        else 
                        {
                            int val = stoi(valStr);
                            string result = g_appData.sequence.prepend(id, val);
                            if (!result.empty()) 
                            {
                                msg = result;
                            }
                            else 
                            {
                                msg = "✅ Значение " + to_string(val) + " добавлено в начало последовательности " + to_string(id);
                            }
                        }
                    }
                }
                else if (action == "insert") 
                {
                    if (!g_appData.sequence.isCreated(id)) 
                    {
                        msg = "❌ Ошибка: Сначала создайте последовательность (кнопка 'Создать')";
                    }
                    else 
                    {
                        string idxStr = trim(params["index"]);
                        string valStr = trim(params["value"]);
                        
                        string typeStr = g_appData.sequence.getTypeString(id);
                        bool isBit = (typeStr.find("BitSequence") != string::npos);
                        size_t currentSize = g_appData.sequence.getSize(id);
                        
                        string idxError = validateIndex(idxStr, currentSize, true);
                        if (!idxError.empty()) 
                        {
                            msg = idxError;
                        }
                        else 
                        {
                            string valError = validateValue(valStr, isBit);
                            if (!valError.empty()) 
                            {
                                msg = valError;
                            }
                            else 
                            {
                                size_t idx = stoul(idxStr);
                                int val = stoi(valStr);
                                string result = g_appData.sequence.insertAt(id, val, idx);
                                if (!result.empty()) 
                                {
                                    msg = result;
                                }
                                else 
                                {
                                    msg = "✅ Значение " + to_string(val) + " вставлено по индексу " + to_string(idx) + " в последовательность " + to_string(id);
                                }
                            }
                        }
                    }
                }
                else if (action == "get") 
                {
                    if (!g_appData.sequence.isCreated(id)) 
                    {
                        msg = "❌ Ошибка: Сначала создайте последовательность (кнопка 'Создать')";
                    }
                    else if (g_appData.sequence.getSize(id) == 0)
                    {
                        msg = "❌ Ошибка: Последовательность пуста, нет элементов для получения";
                    }
                    else 
                    {
                        string idxStr = trim(params["index"]);
                        string idxError = validateIndex(idxStr, g_appData.sequence.getSize(id), false);
                        if (!idxError.empty()) 
                        {
                            msg = idxError;
                        }
                        else 
                        {
                            size_t idx = stoul(idxStr);
                            int val = g_appData.sequence.get(id, idx);
                            msg = "✅ Значение по индексу " + to_string(idx) + " в последовательности " + to_string(id) + ": " + to_string(val);
                        }
                    }
                }
                else if (action == "getFirst") 
                {
                    if (!g_appData.sequence.isCreated(id)) 
                    {
                        msg = "❌ Ошибка: Сначала создайте последовательность (кнопка 'Создать')";
                    }
                    else if (g_appData.sequence.getSize(id) == 0)
                    {
                        msg = "❌ Ошибка: Последовательность пуста, нет первого элемента";
                    }
                    else 
                    {
                        int val = g_appData.sequence.getFirst(id);
                        msg = "✅ Первый элемент последовательности " + to_string(id) + ": " + to_string(val);
                    }
                }
                else if (action == "getLast") 
                {
                    if (!g_appData.sequence.isCreated(id)) 
                    {
                        msg = "❌ Ошибка: Сначала создайте последовательность (кнопка 'Создать')";
                    }
                    else if (g_appData.sequence.getSize(id) == 0)
                    {
                        msg = "❌ Ошибка: Последовательность пуста, нет последнего элемента";
                    }
                    else 
                    {
                        int val = g_appData.sequence.getLast(id);
                        msg = "✅ Последний элемент последовательности " + to_string(id) + ": " + to_string(val);
                    }
                }
                else 
                {
                    msg = "❌ Ошибка: Неизвестное действие '" + action + "'";
                }
            }
        } 
        catch (const exception& e) 
        { 
            msg = "❌ Ошибка: " + string(e.what()); 
        }
        
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // POST /api/sequence/subsequence
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
            if (eqPos != string::npos) 
            {
                string key = pair.substr(0, eqPos);
                string value = pair.substr(eqPos + 1);
                for (char& c : value) if (c == '+') c = ' ';
                params[key] = value;
            }
        }
        
        string msg;
        try 
        {
            int id = 1;
            auto idIt = params.find("id");
            if (idIt != params.end() && !idIt->second.empty()) 
            {
                id = stoi(idIt->second);
            }
            
            if (!g_appData.sequence.isCreated(id)) 
            {
                msg = "❌ Ошибка: Сначала создайте последовательность (кнопка 'Создать')";
            }
            else if (g_appData.sequence.getSize(id) == 0)
            {
                msg = "❌ Ошибка: Последовательность пуста, невозможно получить подпоследовательность";
            }
            else 
            {
                string startStr = trim(params["start"]);
                string endStr = trim(params["end"]);
                
                string startError = validateIndex(startStr, g_appData.sequence.getSize(id), false);
                if (!startError.empty()) 
                {
                    msg = startError;
                }
                else 
                {
                    string endError = validateIndex(endStr, g_appData.sequence.getSize(id), false);
                    if (!endError.empty()) 
                    {
                        msg = endError;
                    }
                    else 
                    {
                        size_t start = stoul(startStr);
                        size_t end = stoul(endStr);
                        
                        if (start > end) 
                        {
                            msg = "❌ Ошибка: Начальный индекс (" + to_string(start) + ") не может быть больше конечного (" + to_string(end) + ")";
                        }
                        else 
                        {
                            string result = g_appData.sequence.getSubsequence(id, start, end);
                            msg = result;
                        }
                    }
                }
            }
        } 
        catch (const exception& e) 
        { 
            msg = "❌ Ошибка: " + string(e.what()); 
        }
        
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // POST /api/sequence/concat
    if (method == "POST" && path == "/api/sequence/concat") 
    {
        string msg = g_appData.sequence.concat();
        responseStr = responseHtml(200, msg);
        return;
    }
    
    // GET главная страница
    if (method == "GET" && (path == "/" || path == "/index.html")) 
    {
        responseStr = responseHtml(200, renderFullPage());
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
            string errorMsg = "❌ Ошибка сервера: " + string(e.what());
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
    serverAddr.sin_port = htons(8080);
    
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
    {
        cerr << "Failed to bind to port 8080" << endl;
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
    cout << "  Web Server Started!" << endl;
    cout << "  Open browser at: http://localhost:8080" << endl;
    cout << "  Press Ctrl+C to stop" << endl;
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