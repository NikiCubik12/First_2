#ifndef TESTS
#define TESTS

#include <iostream>
#include <climits>
#include <string>
#include <exception>
#include <sstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using out_of_range = std::out_of_range;
using invalid_argument = std::invalid_argument;
using runtime_error = std::runtime_error;

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "ArraySequence.hpp"
#include "MutableArraySequence.hpp"
#include "ImmutableArraySequence.hpp"
#include "ListSequence.hpp"
#include "BitSequence.hpp"

size_t totalTests = 0;
size_t passedTests = 0;

void print_separator()
{
    cout << "------------------------------------------------------------" << endl;
}

void print_test_header(const string& name)
{
    cout << "\n============================================================" << endl;
    cout << "  " << name << endl;
    cout << "============================================================" << endl;
}

string array_to_string(size_t* arr, size_t size)
{
    stringstream ss;
    ss << "{";
    for (size_t i = 0; i < size; i++) 
    {
        if (i > 0) 
            ss << ", ";
        ss << arr[i];
    }
    ss << "}";
    return ss.str();
}

void check_size_t(size_t expected, size_t got, const string& testName, const string& input = "", const string& context = "")
{
    totalTests++;
    if (expected == got) {
        passedTests++;
        cout << "  [OK] " << testName;
        if (!input.empty()) 
        {
            cout << " [вход: " << input << "]";
        }
        if (!context.empty()) 
        {
            cout << " (" << context << ")";
        }
        cout << " -> ожидалось " << expected << ", получено " << got << endl;
    } 
    else 
    {
        cout << "  [ОШИБКА] " << testName;
        if (!input.empty()) 
        {
            cout << " [вход: " << input << "]";
        }
        if (!context.empty()) 
        {
            cout << " (" << context << ")";
        }
        cout << " -> ожидалось " << expected << ", получено " << got << endl;
    }
}


void test_DynamicArray_empty_constructor_and_getters()
{
    print_test_header("DynamicArray: пустой конструктор + GetSize");
    cout << "  Вход: нет параметров" << endl;
    cout << "  Вызов: DynamicArray<size_t> arr;" << endl;
    
    try {
        DynamicArray<size_t> arr;
        cout << "  Результат: объект создан" << endl;
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(0, arr.GetSize(), "GetSize()", "нет параметров", "пустой конструктор + геттеры");
        cout << "  [OK] Пустой конструктор и GetSize работают корректно" << endl;
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests++;
    }
}

void test_DynamicArray_constructor_and_getters()
{
    print_test_header("DynamicArray: конструктор из массива + GetSize + Get");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << ", размер 3" << endl;
    cout << "  Вызов: DynamicArray<size_t> arr(mas, 3)" << endl;
    
    try {
        DynamicArray<size_t> arr(mas, 3);
        cout << "  Результат: объект создан" << endl;
        
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(3, arr.GetSize(), "GetSize()", input, "конструктор из массива + геттеры");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, arr.Get(0), "Get(0)", "индекс 0", "конструктор из массива + геттеры");
        check_size_t(20, arr.Get(1), "Get(1)", "индекс 1", "конструктор из массива + геттеры");
        check_size_t(30, arr.Get(2), "Get(2)", "индекс 2", "конструктор из массива + геттеры");
        
        cout << "  [OK] Конструктор из массива и геттеры работают корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_DynamicArray_copy_constructor()
{
    print_test_header("DynamicArray: копирующий конструктор");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: DynamicArray<size_t> arr1(mas, 3)" << endl;
    cout << "  Вызов: DynamicArray<size_t> arr2(arr1)" << endl;
    
    try {
        DynamicArray<size_t> arr1(mas, 3);
        DynamicArray<size_t> arr2(arr1);
        cout << "  Результат: arr2 создан" << endl;
        
        cout << "  Проверка: arr2.GetSize()" << endl;
        check_size_t(3, arr2.GetSize(), "GetSize()", "копия arr1", "копирующий конструктор");
        
        cout << "  Проверка элементов arr2:" << endl;
        check_size_t(10, arr2.Get(0), "Get(0)", "индекс 0", "копирующий конструктор");
        check_size_t(20, arr2.Get(1), "Get(1)", "индекс 1", "копирующий конструктор");
        check_size_t(30, arr2.Get(2), "Get(2)", "индекс 2", "копирующий конструктор");
        
        cout << "  Проверка, что arr1 не изменился:" << endl;
        check_size_t(3, arr1.GetSize(), "arr1.GetSize()", "оригинал", "копирующий конструктор");
        check_size_t(10, arr1.Get(0), "arr1.Get(0)", "оригинал индекс 0", "копирующий конструктор");
        
        cout << "  [OK] Копирующий конструктор работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 6;
    }
}

void test_DynamicArray_copy_from_list()
{
    print_test_header("DynamicArray: конструктор из LinkedList");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: список " << input << endl;
    cout << "  Вызов: LinkedList<size_t> list(mas, 3)" << endl;
    cout << "  Вызов: DynamicArray<size_t> arr(list)" << endl;
    
    try {
        LinkedList<size_t> list(mas, 3);
        DynamicArray<size_t> arr(list);
        cout << "  Результат: arr создан" << endl;
        
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(3, arr.GetSize(), "GetSize()", "из списка " + input, "конструктор из списка");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, arr.Get(0), "Get(0)", "индекс 0", "конструктор из списка");
        check_size_t(20, arr.Get(1), "Get(1)", "индекс 1", "конструктор из списка");
        check_size_t(30, arr.Get(2), "Get(2)", "индекс 2", "конструктор из списка");
        
        cout << "  [OK] Конструктор из LinkedList работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_DynamicArray_set()
{
    print_test_header("DynamicArray: Set");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: arr.Set(2, 99)" << endl;
    cout << "  Ожидаем: arr[2] = 99" << endl;
    
    try {
        DynamicArray<size_t> arr(mas, 3);
        arr.Set(2, 99);
        cout << "  Результат: элемент изменён" << endl;
        
        cout << "  Проверка: arr.Get(2)" << endl;
        check_size_t(99, arr.Get(2), "Get(2)", "индекс 2 (изменён с 30 на 99)", "проверка Set");
        
        cout << "  Проверка, что остальные элементы не изменились:" << endl;
        check_size_t(10, arr.Get(0), "Get(0)", "индекс 0", "проверка Set");
        check_size_t(20, arr.Get(1), "Get(1)", "индекс 1", "проверка Set");
        
        cout << "  [OK] Set работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 3;
    }
}

void test_DynamicArray_append()
{
    print_test_header("DynamicArray: Append");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: arr.Append(40)" << endl;
    cout << "  Ожидаем: массив {10, 20, 30, 40}" << endl;
    
    try {
        DynamicArray<size_t> arr(mas, 3);
        arr.Append(40);
        cout << "  Результат: элемент добавлен" << endl;
        
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(4, arr.GetSize(), "GetSize()", "после Append", "проверка Append");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, arr.Get(0), "Get(0)", "индекс 0", "проверка Append");
        check_size_t(20, arr.Get(1), "Get(1)", "индекс 1", "проверка Append");
        check_size_t(30, arr.Get(2), "Get(2)", "индекс 2", "проверка Append");
        check_size_t(40, arr.Get(3), "Get(3)", "индекс 3 (добавлен)", "проверка Append");
        
        cout << "  [OK] Append работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 5;
    }
}

void test_DynamicArray_prepend()
{
    print_test_header("DynamicArray: Prepend");
    
    size_t mas[3] = {20, 30, 40};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: arr.Prepend(10)" << endl;
    cout << "  Ожидаем: массив {10, 20, 30, 40}" << endl;
    
    try {
        DynamicArray<size_t> arr(mas, 3);
        arr.Prepend(10);
        cout << "  Результат: элемент добавлен" << endl;
        
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(4, arr.GetSize(), "GetSize()", "после Prepend", "проверка Prepend");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, arr.Get(0), "Get(0)", "индекс 0 (добавлен)", "проверка Prepend");
        check_size_t(20, arr.Get(1), "Get(1)", "индекс 1", "проверка Prepend");
        check_size_t(30, arr.Get(2), "Get(2)", "индекс 2", "проверка Prepend");
        check_size_t(40, arr.Get(3), "Get(3)", "индекс 3", "проверка Prepend");
        
        cout << "  [OK] Prepend работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 5;
    }
}

void test_DynamicArray_insert_at()
{
    print_test_header("DynamicArray: InsertAt");
    
    size_t mas[3] = {10, 30, 40};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: arr.InsertAt(20, 1)" << endl;
    cout << "  Ожидаем: массив {10, 20, 30, 40}" << endl;
    
    try {
        DynamicArray<size_t> arr(mas, 3);
        arr.InsertAt(20, 1);
        cout << "  Результат: элемент вставлен" << endl;
        
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(4, arr.GetSize(), "GetSize()", "после InsertAt", "проверка InsertAt");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, arr.Get(0), "Get(0)", "индекс 0", "проверка InsertAt");
        check_size_t(20, arr.Get(1), "Get(1)", "индекс 1 (вставлен)", "проверка InsertAt");
        check_size_t(30, arr.Get(2), "Get(2)", "индекс 2", "проверка InsertAt");
        check_size_t(40, arr.Get(3), "Get(3)", "индекс 3", "проверка InsertAt");
        
        cout << "  [OK] InsertAt работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 5;
    }
}

void test_DynamicArray_resize()
{
    print_test_header("DynamicArray: Resize (увеличение)");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: arr.Resize(5)" << endl;
    cout << "  Ожидаем: размер 5, старые элементы сохранены" << endl;
    
    try {
        DynamicArray<size_t> arr(mas, 3);
        arr.Resize(5);
        cout << "  Результат: размер изменён" << endl;
        
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(5, arr.GetSize(), "GetSize()", "новый размер 5", "проверка Resize");
        
        cout << "  Проверка старых элементов:" << endl;
        check_size_t(10, arr.Get(0), "Get(0)", "индекс 0", "проверка Resize");
        check_size_t(20, arr.Get(1), "Get(1)", "индекс 1", "проверка Resize");
        check_size_t(30, arr.Get(2), "Get(2)", "индекс 2", "проверка Resize");
        
        cout << "  [OK] Resize (увеличение) работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_DynamicArray_resize_smaller()
{
    print_test_header("DynamicArray: Resize (уменьшение)");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: arr.Resize(2)" << endl;
    cout << "  Ожидаем: размер 2, первые 2 элемента сохранены" << endl;
    
    try {
        DynamicArray<size_t> arr(mas, 3);
        arr.Resize(2);
        cout << "  Результат: размер изменён" << endl;
        
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(2, arr.GetSize(), "GetSize()", "новый размер 2", "проверка Resize (уменьшение)");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, arr.Get(0), "Get(0)", "индекс 0", "проверка Resize (уменьшение)");
        check_size_t(20, arr.Get(1), "Get(1)", "индекс 1", "проверка Resize (уменьшение)");
        
        cout << "  [OK] Resize (уменьшение) работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 3;
    }
}


void test_LinkedList_empty_constructor_and_getters()
{
    print_test_header("LinkedList: пустой конструктор + GetLength");
    cout << "  Вход: нет параметров" << endl;
    cout << "  Вызов: LinkedList<size_t> list;" << endl;
    
    try {
        LinkedList<size_t> list;
        cout << "  Результат: объект создан" << endl;
        cout << "  Проверка: list.GetLength()" << endl;
        check_size_t(0, list.GetLength(), "GetLength()", "нет параметров", "пустой конструктор + геттеры (список)");
        cout << "  [OK] Пустой конструктор и GetLength работают корректно" << endl;
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests++;
    }
}

void test_LinkedList_constructor_and_getters()
{
    print_test_header("LinkedList: конструктор из массива + GetLength + Get + GetFirst + GetLast");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << ", размер 3" << endl;
    cout << "  Вызов: LinkedList<size_t> list(mas, 3)" << endl;
    
    try {
        LinkedList<size_t> list(mas, 3);
        cout << "  Результат: объект создан" << endl;
        
        cout << "  Проверка: list.GetLength()" << endl;
        check_size_t(3, list.GetLength(), "GetLength()", input, "конструктор из массива + геттеры (список)");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, list.Get(0), "Get(0)", "индекс 0", "конструктор из массива + геттеры (список)");
        check_size_t(20, list.Get(1), "Get(1)", "индекс 1", "конструктор из массива + геттеры (список)");
        check_size_t(30, list.Get(2), "Get(2)", "индекс 2", "конструктор из массива + геттеры (список)");
        
        cout << "  Проверка: list.GetFirst()" << endl;
        check_size_t(10, list.GetFirst(), "GetFirst()", input, "конструктор из массива + геттеры (список)");
        
        cout << "  Проверка: list.GetLast()" << endl;
        check_size_t(30, list.GetLast(), "GetLast()", input, "конструктор из массива + геттеры (список)");
        
        cout << "  [OK] Конструктор из массива и геттеры работают корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 6;
    }
}

void test_LinkedList_copy_constructor()
{
    print_test_header("LinkedList: копирующий конструктор");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: LinkedList<size_t> list1(mas, 3)" << endl;
    cout << "  Вызов: LinkedList<size_t> list2(list1)" << endl;
    
    try {
        LinkedList<size_t> list1(mas, 3);
        LinkedList<size_t> list2(list1);
        cout << "  Результат: list2 создан" << endl;
        
        cout << "  Проверка: list2.GetLength()" << endl;
        check_size_t(3, list2.GetLength(), "GetLength()", "копия list1", "копирующий конструктор (список)");
        
        cout << "  Проверка элементов list2:" << endl;
        check_size_t(10, list2.Get(0), "Get(0)", "индекс 0", "копирующий конструктор (список)");
        check_size_t(20, list2.Get(1), "Get(1)", "индекс 1", "копирующий конструктор (список)");
        check_size_t(30, list2.Get(2), "Get(2)", "индекс 2", "копирующий конструктор (список)");
        
        cout << "  [OK] Копирующий конструктор работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_LinkedList_append()
{
    print_test_header("LinkedList: Append");
    
    cout << "  Вход: последовательно 10, 20, 30" << endl;
    cout << "  Вызов: list.Append(10); list.Append(20); list.Append(30)" << endl;
    cout << "  Ожидаем: список {10, 20, 30}" << endl;
    
    try {
        LinkedList<size_t> list;
        list.Append(10);
        list.Append(20);
        list.Append(30);
        cout << "  Результат: элементы добавлены" << endl;
        
        cout << "  Проверка: list.GetLength()" << endl;
        check_size_t(3, list.GetLength(), "GetLength()", "добавлены 10,20,30", "проверка Append (список)");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, list.Get(0), "Get(0)", "индекс 0", "проверка Append (список)");
        check_size_t(20, list.Get(1), "Get(1)", "индекс 1", "проверка Append (список)");
        check_size_t(30, list.Get(2), "Get(2)", "индекс 2", "проверка Append (список)");
        
        cout << "  [OK] Append работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_LinkedList_prepend()
{
    print_test_header("LinkedList: Prepend");
    
    cout << "  Вход: последовательно 30, 20, 10 (в начало)" << endl;
    cout << "  Вызов: list.Prepend(30); list.Prepend(20); list.Prepend(10)" << endl;
    cout << "  Ожидаем: список {10, 20, 30}" << endl;
    
    try {
        LinkedList<size_t> list;
        list.Prepend(30);
        list.Prepend(20);
        list.Prepend(10);
        cout << "  Результат: элементы добавлены" << endl;
        
        cout << "  Проверка: list.GetLength()" << endl;
        check_size_t(3, list.GetLength(), "GetLength()", "добавлены 30,20,10 в начало", "проверка Prepend (список)");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, list.Get(0), "Get(0)", "индекс 0", "проверка Prepend (список)");
        check_size_t(20, list.Get(1), "Get(1)", "индекс 1", "проверка Prepend (список)");
        check_size_t(30, list.Get(2), "Get(2)", "индекс 2", "проверка Prepend (список)");
        
        cout << "  [OK] Prepend работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_LinkedList_insert_at()
{
    print_test_header("LinkedList: InsertAt");
    
    cout << "  Вход: массив {10, 30}, вставка 20 по индексу 1" << endl;
    cout << "  Вызов: list.InsertAt(20, 1)" << endl;
    cout << "  Ожидаем: список {10, 20, 30}" << endl;
    
    try {
        size_t mas[2] = {10, 30};
        LinkedList<size_t> list(mas, 2);
        list.InsertAt(20, 1);
        cout << "  Результат: элемент вставлен" << endl;
        
        cout << "  Проверка: list.GetLength()" << endl;
        check_size_t(3, list.GetLength(), "GetLength()", "вставка 20 по индексу 1", "проверка InsertAt (список)");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, list.Get(0), "Get(0)", "индекс 0", "проверка InsertAt (список)");
        check_size_t(20, list.Get(1), "Get(1)", "индекс 1 (вставлен)", "проверка InsertAt (список)");
        check_size_t(30, list.Get(2), "Get(2)", "индекс 2", "проверка InsertAt (список)");
        
        cout << "  [OK] InsertAt работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_LinkedList_concat()
{
    print_test_header("LinkedList: Concat");
    
    size_t mas1[2] = {10, 20};
    size_t mas2[2] = {30, 40};
    string input1 = array_to_string(mas1, 2);
    string input2 = array_to_string(mas2, 2);
    
    cout << "  Вход: list1 = " << input1 << ", list2 = " << input2 << endl;
    cout << "  Вызов: list1.Concat(&list2)" << endl;
    cout << "  Ожидаем: " << input1 << " + " << input2 << " -> {10, 20, 30, 40}" << endl;
    
    try {
        LinkedList<size_t> list1(mas1, 2);
        LinkedList<size_t> list2(mas2, 2);
        LinkedList<size_t>* result = list1.Concat(&list2);
        cout << "  Результат: список объединён" << endl;
        
        cout << "  Проверка: result->GetLength()" << endl;
        check_size_t(4, result->GetLength(), "GetLength()", input1 + " + " + input2, "проверка Concat");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(10, result->Get(0), "Get(0)", "индекс 0", "проверка Concat");
        check_size_t(20, result->Get(1), "Get(1)", "индекс 1", "проверка Concat");
        check_size_t(30, result->Get(2), "Get(2)", "индекс 2", "проверка Concat");
        check_size_t(40, result->Get(3), "Get(3)", "индекс 3", "проверка Concat");
        
        cout << "  Проверка, что list1 не изменился:" << endl;
        check_size_t(2, list1.GetLength(), "list1.GetLength()", "оригинал", "проверка Concat");
        
        cout << "  [OK] Concat работает корректно" << endl;
        
        delete result;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 6;
    }
}

void test_LinkedList_get_sublist()
{
    print_test_header("LinkedList: GetSubList");
    
    size_t mas[5] = {10, 20, 30, 40, 50};
    string input = array_to_string(mas, 5);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: list.GetSubList(1, 3)" << endl;
    cout << "  Ожидаем: подсписок {20, 30, 40}" << endl;
    
    try {
        LinkedList<size_t> list(mas, 5);
        LinkedList<size_t>* sublist = list.GetSubList(1, 3);
        cout << "  Результат: подсписок создан" << endl;
        
        cout << "  Проверка: sublist->GetLength()" << endl;
        check_size_t(3, sublist->GetLength(), "GetLength()", "подсписок из " + input + " [1..3]", "проверка GetSubList");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(20, sublist->Get(0), "Get(0)", "индекс 0 из подсписка", "проверка GetSubList");
        check_size_t(30, sublist->Get(1), "Get(1)", "индекс 1 из подсписка", "проверка GetSubList");
        check_size_t(40, sublist->Get(2), "Get(2)", "индекс 2 из подсписка", "проверка GetSubList");
        
        cout << "  [OK] GetSubList работает корректно" << endl;
        
        delete sublist;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_LinkedList_clear()
{
    print_test_header("LinkedList: ClearList");
    
    size_t mas[3] = {10, 20, 30};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: list.ClearList()" << endl;
    cout << "  Ожидаем: размер 0" << endl;
    
    try {
        LinkedList<size_t> list(mas, 3);
        list.ClearList();
        cout << "  Результат: список очищен" << endl;
        
        cout << "  Проверка: list.GetLength()" << endl;
        check_size_t(0, list.GetLength(), "GetLength()", "после очистки (было 3)", "проверка ClearList");
        
        cout << "  [OK] ClearList работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests++;
    }
}


void test_BitSequence_empty_constructor_and_getters()
{
    print_test_header("BitSequence: пустой конструктор + GetLength");
    cout << "  Вход: нет параметров" << endl;
    cout << "  Вызов: BitSequence bits;" << endl;
    
    try {
        BitSequence bits;
        cout << "  Результат: объект создан" << endl;
        cout << "  Проверка: bits.GetLength()" << endl;
        check_size_t(0, bits.GetLength(), "GetLength()", "нет параметров", "пустой конструктор + геттеры BitSequence");
        cout << "  [OK] Пустой конструктор и GetLength работают корректно" << endl;
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests++;
    }
}

void test_BitSequence_constructor_and_getters()
{
    print_test_header("BitSequence: конструктор из массива + GetLength + Get + GetFirst + GetLast");
    
    size_t mas[3] = {1, 0, 1};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << ", размер 3" << endl;
    cout << "  Вызов: BitSequence bits(mas, 3)" << endl;
    
    try {
        BitSequence bits(mas, 3);
        cout << "  Результат: объект создан" << endl;
        
        cout << "  Проверка: bits.GetLength()" << endl;
        check_size_t(3, bits.GetLength(), "GetLength()", input, "конструктор из массива + геттеры BitSequence");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, bits.Get(0), "Get(0)", "индекс 0", "конструктор из массива + геттеры BitSequence");
        check_size_t(0, bits.Get(1), "Get(1)", "индекс 1", "конструктор из массива + геттеры BitSequence");
        check_size_t(1, bits.Get(2), "Get(2)", "индекс 2", "конструктор из массива + геттеры BitSequence");
        
        cout << "  Проверка: bits.GetFirst()" << endl;
        check_size_t(1, bits.GetFirst(), "GetFirst()", input, "конструктор из массива + геттеры BitSequence");
        
        cout << "  Проверка: bits.GetLast()" << endl;
        check_size_t(1, bits.GetLast(), "GetLast()", input, "конструктор из массива + геттеры BitSequence");
        
        cout << "  [OK] Конструктор из массива и геттеры работают корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 6;
    }
}

void test_BitSequence_copy_constructor()
{
    print_test_header("BitSequence: копирующий конструктор");
    
    size_t mas[3] = {1, 0, 1};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << endl;
    cout << "  Вызов: BitSequence bits1(mas, 3)" << endl;
    cout << "  Вызов: BitSequence bits2(bits1)" << endl;
    
    try {
        BitSequence bits1(mas, 3);
        BitSequence bits2(bits1);
        cout << "  Результат: bits2 создан" << endl;
        
        cout << "  Проверка: bits2.GetLength()" << endl;
        check_size_t(3, bits2.GetLength(), "GetLength()", "копия bits1", "копирующий конструктор BitSequence");
        
        cout << "  Проверка элементов bits2:" << endl;
        check_size_t(1, bits2.Get(0), "Get(0)", "индекс 0", "копирующий конструктор BitSequence");
        check_size_t(0, bits2.Get(1), "Get(1)", "индекс 1", "копирующий конструктор BitSequence");
        check_size_t(1, bits2.Get(2), "Get(2)", "индекс 2", "копирующий конструктор BitSequence");
        
        cout << "  [OK] Копирующий конструктор работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_BitSequence_from_unsigned_int()
{
    print_test_header("BitSequence: конструктор из числа");
    
    cout << "  Вход: число 5" << endl;
    cout << "  Вызов: BitSequence bits(5)" << endl;
    cout << "  Ожидаем: биты {1, 0, 1} (двоичное представление 5)" << endl;
    
    try {
        BitSequence bits(5);
        cout << "  Результат: объект создан" << endl;
        
        cout << "  Проверка: bits.GetLength()" << endl;
        check_size_t(3, bits.GetLength(), "GetLength()", "число 5", "BitSequence из числа");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, bits.Get(0), "Get(0)", "индекс 0", "BitSequence из числа 5");
        check_size_t(0, bits.Get(1), "Get(1)", "индекс 1", "BitSequence из числа 5");
        check_size_t(1, bits.Get(2), "Get(2)", "индекс 2", "BitSequence из числа 5");
        
        cout << "  [OK] Конструктор из числа работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_BitSequence_append()
{
    print_test_header("BitSequence: Append");
    
    cout << "  Вход: последовательно 1, 0, 1" << endl;
    cout << "  Вызов: bits.Append(1); bits.Append(0); bits.Append(1)" << endl;
    cout << "  Ожидаем: {1, 0, 1}" << endl;
    
    try {
        BitSequence bits;
        bits.Append(1);
        bits.Append(0);
        bits.Append(1);
        cout << "  Результат: элементы добавлены" << endl;
        
        cout << "  Проверка: bits.GetLength()" << endl;
        check_size_t(3, bits.GetLength(), "GetLength()", "добавлены 1,0,1", "проверка Append BitSequence");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, bits.Get(0), "Get(0)", "индекс 0", "проверка Append BitSequence");
        check_size_t(0, bits.Get(1), "Get(1)", "индекс 1", "проверка Append BitSequence");
        check_size_t(1, bits.Get(2), "Get(2)", "индекс 2", "проверка Append BitSequence");
        
        cout << "  [OK] Append работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_BitSequence_insert_at()
{
    print_test_header("BitSequence: InsertAt");
    
    cout << "  Вход: массив {1, 1}, вставка 0 по индексу 1" << endl;
    cout << "  Вызов: bits.InsertAt(0, 1)" << endl;
    cout << "  Ожидаем: {1, 0, 1}" << endl;
    
    try {
        size_t mas[2] = {1, 1};
        BitSequence bits(mas, 2);
        bits.InsertAt(0, 1);
        cout << "  Результат: элемент вставлен" << endl;
        
        cout << "  Проверка: bits.GetLength()" << endl;
        check_size_t(3, bits.GetLength(), "GetLength()", "вставка 0 по индексу 1", "проверка InsertAt BitSequence");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, bits.Get(0), "Get(0)", "индекс 0", "проверка InsertAt BitSequence");
        check_size_t(0, bits.Get(1), "Get(1)", "индекс 1 (вставлен)", "проверка InsertAt BitSequence");
        check_size_t(1, bits.Get(2), "Get(2)", "индекс 2", "проверка InsertAt BitSequence");
        
        cout << "  [OK] InsertAt работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_BitSequence_concat()
{
    print_test_header("BitSequence: Concat");
    
    size_t mas1[2] = {1, 0};
    size_t mas2[2] = {1, 1};
    string input1 = array_to_string(mas1, 2);
    string input2 = array_to_string(mas2, 2);
    
    cout << "  Вход: bits1 = " << input1 << ", bits2 = " << input2 << endl;
    cout << "  Вызов: bits1.Concat(&bits2)" << endl;
    cout << "  Ожидаем: " << input1 << " + " << input2 << " -> {1, 0, 1, 1}" << endl;
    
    try {
        BitSequence bits1(mas1, 2);
        BitSequence bits2(mas2, 2);
        Sequence<size_t>* result = bits1.Concat(&bits2);
        cout << "  Результат: последовательности объединены" << endl;
        
        cout << "  Проверка: result->GetLength()" << endl;
        check_size_t(4, result->GetLength(), "GetLength()", input1 + " + " + input2, "проверка Concat BitSequence");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, result->Get(0), "Get(0)", "индекс 0", "проверка Concat BitSequence");
        check_size_t(0, result->Get(1), "Get(1)", "индекс 1", "проверка Concat BitSequence");
        check_size_t(1, result->Get(2), "Get(2)", "индекс 2", "проверка Concat BitSequence");
        check_size_t(1, result->Get(3), "Get(3)", "индекс 3", "проверка Concat BitSequence");
        
        cout << "  [OK] Concat работает корректно" << endl;
        
        delete result;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 5;
    }
}

void test_BitSequence_bit_and()
{
    print_test_header("BitSequence: BitAnd");
    
    size_t mas1[3] = {1, 0, 1};
    size_t mas2[3] = {1, 1, 0};
    string input1 = array_to_string(mas1, 3);
    string input2 = array_to_string(mas2, 3);
    
    cout << "  Вход: bits1 = " << input1 << ", bits2 = " << input2 << endl;
    cout << "  Вызов: bits1.BitAnd(&bits2)" << endl;
    cout << "  Ожидаем: " << input1 << " & " << input2 << " = {1, 0, 0}" << endl;
    
    try {
        BitSequence bits1(mas1, 3);
        BitSequence bits2(mas2, 3);
        BitSequence* result = bits1.BitAnd(&bits2);
        cout << "  Результат: операция выполнена" << endl;
        
        cout << "  Проверка: result->GetLength()" << endl;
        check_size_t(3, result->GetLength(), "GetLength()", "BitAnd", "проверка BitAnd");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, result->Get(0), "Get(0)", "1 & 1 = 1", "проверка BitAnd");
        check_size_t(0, result->Get(1), "Get(1)", "0 & 1 = 0", "проверка BitAnd");
        check_size_t(0, result->Get(2), "Get(2)", "1 & 0 = 0", "проверка BitAnd");
        
        cout << "  [OK] BitAnd работает корректно" << endl;
        
        delete result;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_BitSequence_bit_or()
{
    print_test_header("BitSequence: BitOr");
    
    size_t mas1[3] = {1, 0, 1};
    size_t mas2[3] = {1, 1, 0};
    string input1 = array_to_string(mas1, 3);
    string input2 = array_to_string(mas2, 3);
    
    cout << "  Вход: bits1 = " << input1 << ", bits2 = " << input2 << endl;
    cout << "  Вызов: bits1.BitOr(&bits2)" << endl;
    cout << "  Ожидаем: " << input1 << " | " << input2 << " = {1, 1, 1}" << endl;
    
    try {
        BitSequence bits1(mas1, 3);
        BitSequence bits2(mas2, 3);
        BitSequence* result = bits1.BitOr(&bits2);
        cout << "  Результат: операция выполнена" << endl;
        
        cout << "  Проверка: result->GetLength()" << endl;
        check_size_t(3, result->GetLength(), "GetLength()", "BitOr", "проверка BitOr");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, result->Get(0), "Get(0)", "1 | 1 = 1", "проверка BitOr");
        check_size_t(1, result->Get(1), "Get(1)", "0 | 1 = 1", "проверка BitOr");
        check_size_t(1, result->Get(2), "Get(2)", "1 | 0 = 1", "проверка BitOr");
        
        cout << "  [OK] BitOr работает корректно" << endl;
        
        delete result;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_BitSequence_bit_xor()
{
    print_test_header("BitSequence: BitXor");
    
    size_t mas1[3] = {1, 0, 1};
    size_t mas2[3] = {1, 1, 0};
    string input1 = array_to_string(mas1, 3);
    string input2 = array_to_string(mas2, 3);
    
    cout << "  Вход: bits1 = " << input1 << ", bits2 = " << input2 << endl;
    cout << "  Вызов: bits1.BitXor(&bits2)" << endl;
    cout << "  Ожидаем: " << input1 << " ^ " << input2 << " = {0, 1, 1}" << endl;
    
    try {
        BitSequence bits1(mas1, 3);
        BitSequence bits2(mas2, 3);
        BitSequence* result = bits1.BitXor(&bits2);
        cout << "  Результат: операция выполнена" << endl;
        
        cout << "  Проверка: result->GetLength()" << endl;
        check_size_t(3, result->GetLength(), "GetLength()", "BitXor", "проверка BitXor");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(0, result->Get(0), "Get(0)", "1 ^ 1 = 0", "проверка BitXor");
        check_size_t(1, result->Get(1), "Get(1)", "0 ^ 1 = 1", "проверка BitXor");
        check_size_t(1, result->Get(2), "Get(2)", "1 ^ 0 = 1", "проверка BitXor");
        
        cout << "  [OK] BitXor работает корректно" << endl;
        
        delete result;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_BitSequence_bit_not()
{
    print_test_header("BitSequence: BitNot");
    
    size_t mas[3] = {1, 0, 1};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: bits = " << input << endl;
    cout << "  Вызов: bits.BitNot()" << endl;
    cout << "  Ожидаем: ~" << input << " = {0, 1, 0}" << endl;
    
    try {
        BitSequence bits(mas, 3);
        BitSequence* result = bits.BitNot();
        cout << "  Результат: операция выполнена" << endl;
        
        cout << "  Проверка: result->GetLength()" << endl;
        check_size_t(3, result->GetLength(), "GetLength()", "BitNot", "проверка BitNot");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(0, result->Get(0), "Get(0)", "~1 = 0", "проверка BitNot");
        check_size_t(1, result->Get(1), "Get(1)", "~0 = 1", "проверка BitNot");
        check_size_t(0, result->Get(2), "Get(2)", "~1 = 0", "проверка BitNot");
        
        cout << "  [OK] BitNot работает корректно" << endl;
        
        delete result;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 4;
    }
}

void test_BitSequence_invalid_value()
{
    print_test_header("BitSequence: недопустимое значение (2)");
    
    size_t mas[3] = {1, 2, 1};
    string input = array_to_string(mas, 3);
    
    cout << "  Вход: массив " << input << " (2 — недопустимый бит)" << endl;
    cout << "  Вызов: BitSequence bits(mas, 3)" << endl;
    cout << "  Ожидаем: InvalidBitException" << endl;
    
    try {
        BitSequence bits(mas, 3);
        cout << "  [ОШИБКА] Исключение не выброшено!" << endl;
        totalTests++;
    } catch (const InvalidBitException& e) {
        cout << "  [OK] Поймано InvalidBitException" << endl;
        cout << "  Сообщение: " << e.what() << endl;
        totalTests++;
        passedTests++;
    } catch (const std::exception& e) {
        cout << "  [ОШИБКА] Поймано другое исключение: " << e.what() << endl;
        totalTests++;
    }
}


void test_initializer_list_dynamic_array()
{
    print_test_header("initializer_list: DynamicArray");
    cout << "  Вход: {1, 2, 3, 4, 5}" << endl;
    cout << "  Вызов: DynamicArray<size_t> arr = {1, 2, 3, 4, 5}" << endl;
    
    try {
        DynamicArray<size_t> arr = {1, 2, 3, 4, 5};
        cout << "  Результат: объект создан" << endl;
        
        cout << "  Проверка: arr.GetSize()" << endl;
        check_size_t(5, arr.GetSize(), "GetSize()", "{1,2,3,4,5}", "initializer_list DynamicArray");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, arr.Get(0), "Get(0)", "индекс 0", "initializer_list DynamicArray");
        check_size_t(2, arr.Get(1), "Get(1)", "индекс 1", "initializer_list DynamicArray");
        check_size_t(3, arr.Get(2), "Get(2)", "индекс 2", "initializer_list DynamicArray");
        check_size_t(4, arr.Get(3), "Get(3)", "индекс 3", "initializer_list DynamicArray");
        check_size_t(5, arr.Get(4), "Get(4)", "индекс 4", "initializer_list DynamicArray");
        
        cout << "  [OK] initializer_list DynamicArray работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 6;
    }
}

void test_initializer_list_array_sequence()
{
    print_test_header("initializer_list: ArraySequence");
    cout << "  Вход: {1, 2, 3, 4, 5}" << endl;
    cout << "  Вызов: ArraySequence<size_t> seq = {1, 2, 3, 4, 5}" << endl;
    
    try {
        ArraySequence<size_t> seq = {1, 2, 3, 4, 5};
        cout << "  Результат: объект создан" << endl;
        
        cout << "  Проверка: seq.GetLength()" << endl;
        check_size_t(5, seq.GetLength(), "GetLength()", "{1,2,3,4,5}", "initializer_list ArraySequence");
        
        cout << "  Проверка: seq.GetFirst()" << endl;
        check_size_t(1, seq.GetFirst(), "GetFirst()", "первый элемент", "initializer_list ArraySequence");
        
        cout << "  Проверка: seq.GetLast()" << endl;
        check_size_t(5, seq.GetLast(), "GetLast()", "последний элемент", "initializer_list ArraySequence");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, seq.Get(0), "Get(0)", "индекс 0", "initializer_list ArraySequence");
        check_size_t(3, seq.Get(2), "Get(2)", "индекс 2", "initializer_list ArraySequence");
        check_size_t(5, seq.Get(4), "Get(4)", "индекс 4", "initializer_list ArraySequence");
        
        cout << "  [OK] initializer_list ArraySequence работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 6;
    }
}

void test_initializer_list_list_sequence()
{
    print_test_header("initializer_list: ListSequence");
    cout << "  Вход: {100, 200, 300, 400, 500}" << endl;
    cout << "  Вызов: ListSequence<size_t> seq = {100, 200, 300, 400, 500}" << endl;
    
    try {
        ListSequence<size_t> seq = {100, 200, 300, 400, 500};
        cout << "  Результат: объект создан" << endl;
        
        cout << "  Проверка: seq.GetLength()" << endl;
        check_size_t(5, seq.GetLength(), "GetLength()", "{100,200,300,400,500}", "initializer_list ListSequence");
        
        cout << "  Проверка: seq.GetFirst()" << endl;
        check_size_t(100, seq.GetFirst(), "GetFirst()", "первый элемент", "initializer_list ListSequence");
        
        cout << "  Проверка: seq.GetLast()" << endl;
        check_size_t(500, seq.GetLast(), "GetLast()", "последний элемент", "initializer_list ListSequence");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(100, seq.Get(0), "Get(0)", "индекс 0", "initializer_list ListSequence");
        check_size_t(300, seq.Get(2), "Get(2)", "индекс 2", "initializer_list ListSequence");
        check_size_t(500, seq.Get(4), "Get(4)", "индекс 4", "initializer_list ListSequence");
        
        cout << "  [OK] initializer_list ListSequence работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 6;
    }
}

void test_initializer_list_bit_sequence()
{
    print_test_header("initializer_list: BitSequence");
    cout << "  Вход: {1, 0, 1, 0, 1}" << endl;
    cout << "  Вызов: BitSequence seq = {1, 0, 1, 0, 1}" << endl;
    
    try {
        BitSequence seq = {1, 0, 1, 0, 1};
        cout << "  Результат: объект создан" << endl;
        
        cout << "  Проверка: seq.GetLength()" << endl;
        check_size_t(5, seq.GetLength(), "GetLength()", "{1,0,1,0,1}", "initializer_list BitSequence");
        
        cout << "  Проверка: seq.GetFirst()" << endl;
        check_size_t(1, seq.GetFirst(), "GetFirst()", "первый элемент", "initializer_list BitSequence");
        
        cout << "  Проверка: seq.GetLast()" << endl;
        check_size_t(1, seq.GetLast(), "GetLast()", "последний элемент", "initializer_list BitSequence");
        
        cout << "  Проверка элементов:" << endl;
        check_size_t(1, seq.Get(0), "Get(0)", "индекс 0", "initializer_list BitSequence");
        check_size_t(0, seq.Get(1), "Get(1)", "индекс 1", "initializer_list BitSequence");
        check_size_t(1, seq.Get(2), "Get(2)", "индекс 2", "initializer_list BitSequence");
        check_size_t(0, seq.Get(3), "Get(3)", "индекс 3", "initializer_list BitSequence");
        check_size_t(1, seq.Get(4), "Get(4)", "индекс 4", "initializer_list BitSequence");
        
        cout << "  [OK] initializer_list BitSequence работает корректно" << endl;
        
    } catch (const std::exception& e) {
        cout << "  [ИСКЛЮЧЕНИЕ] " << e.what() << endl;
        totalTests += 7;
    }
}

void test_initializer_list_bit_sequence_invalid()
{
    print_test_header("initializer_list: BitSequence с недопустимым значением");
    cout << "  Вход: {1, 0, 2, 0, 1} (2 — недопустимый бит)" << endl;
    cout << "  Вызов: BitSequence seq = {1, 0, 2, 0, 1}" << endl;
    cout << "  Ожидаем: InvalidBitException" << endl;
    
    try {
        BitSequence seq = {1, 0, 2, 0, 1};
        cout << "  [ОШИБКА] Исключение не выброшено!" << endl;
        totalTests++;
    } catch (const InvalidBitException& e) {
        cout << "  [OK] Поймано InvalidBitException" << endl;
        cout << "  Сообщение: " << e.what() << endl;
        totalTests++;
        passedTests++;
    } catch (const std::exception& e) {
        cout << "  [ОШИБКА] Поймано другое исключение: " << e.what() << endl;
        totalTests++;
    }
}

#endif