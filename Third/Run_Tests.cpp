#include <iostream>
#include "Tests.hpp"

using namespace std;

void run_test(void (*test_function)(), const string& name)
{
    cout << "========================================" << endl;
    cout << "Запуск: " << name << endl;
    cout << "========================================" << endl;
    
    size_t old_passed = passedTests;
    size_t old_total = totalTests;
    
    test_function();
    
    size_t new_passed = passedTests - old_passed;
    size_t new_total = totalTests - old_total;
    
    print_separator();
    if (new_total > 0 && new_passed == new_total) 
    {
        cout << "[OK] " << name << " (" << new_passed << "/" << new_total << ")" << endl;
    } 
    else 
    {
        cout << "[ОШИБКА] " << name << " (" << new_passed << "/" << new_total << ")" << endl;
    }
    cout << "========================================" << endl << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    
    cout << "\n============================================================" << endl;
    cout << "                 ЗАПУСК ВСЕХ ТЕСТОВ" << endl;
    cout << "============================================================" << endl;
    
    run_test(test_DynamicArray_empty_constructor_and_getters, "DynamicArray: пустой конструктор + GetSize");
    run_test(test_DynamicArray_constructor_and_getters, "DynamicArray: конструктор из массива + GetSize + Get");
    run_test(test_DynamicArray_copy_constructor, "DynamicArray: копирующий конструктор");
    run_test(test_DynamicArray_copy_from_list, "DynamicArray: конструктор из LinkedList");
    run_test(test_DynamicArray_get, "DynamicArray: Get");
    run_test(test_DynamicArray_set, "DynamicArray: Set");
    run_test(test_DynamicArray_append, "DynamicArray: Append");
    run_test(test_DynamicArray_prepend, "DynamicArray: Prepend");
    run_test(test_DynamicArray_insert_at, "DynamicArray: InsertAt");
    run_test(test_DynamicArray_resize, "DynamicArray: Resize (увеличение)");
    run_test(test_DynamicArray_resize_smaller, "DynamicArray: Resize (уменьшение)");
    
    run_test(test_LinkedList_empty_constructor_and_getters, "LinkedList: пустой конструктор + GetLength");
    run_test(test_LinkedList_constructor_and_getters, "LinkedList: конструктор из массива + GetLength + Get + GetFirst + GetLast");
    run_test(test_LinkedList_copy_constructor, "LinkedList: копирующий конструктор");
    run_test(test_LinkedList_append, "LinkedList: Append");
    run_test(test_LinkedList_prepend, "LinkedList: Prepend");
    run_test(test_LinkedList_insert_at, "LinkedList: InsertAt");
    run_test(test_LinkedList_concat, "LinkedList: Concat");
    run_test(test_LinkedList_get_sublist, "LinkedList: GetSubList");
    run_test(test_LinkedList_clear, "LinkedList: ClearList");
    
    run_test(test_BitSequence_empty_constructor_and_getters, "BitSequence: пустой конструктор + GetLength");
    run_test(test_BitSequence_constructor_and_getters, "BitSequence: конструктор из массива + GetLength + Get + GetFirst + GetLast");
    run_test(test_BitSequence_copy_constructor, "BitSequence: копирующий конструктор");
    run_test(test_BitSequence_from_unsigned_int, "BitSequence: конструктор из числа");
    run_test(test_BitSequence_append, "BitSequence: Append");
    run_test(test_BitSequence_insert_at, "BitSequence: InsertAt");
    run_test(test_BitSequence_concat, "BitSequence: Concat");
    run_test(test_BitSequence_bit_and, "BitSequence: BitAnd");
    run_test(test_BitSequence_bit_or, "BitSequence: BitOr");
    run_test(test_BitSequence_bit_xor, "BitSequence: BitXor");
    run_test(test_BitSequence_bit_not, "BitSequence: BitNot");
    run_test(test_BitSequence_invalid_value, "BitSequence: недопустимое значение");
    
    run_test(test_initializer_list_dynamic_array, "initializer_list: DynamicArray");
    run_test(test_initializer_list_array_sequence, "initializer_list: ArraySequence");
    run_test(test_initializer_list_list_sequence, "initializer_list: ListSequence");
    run_test(test_initializer_list_bit_sequence, "initializer_list: BitSequence");
    run_test(test_initializer_list_bit_sequence_invalid, "initializer_list: BitSequence (недопустимое)");
    
    cout << "\n============================================================" << endl;
    cout << "                 ИТОГОВЫЙ РЕЗУЛЬТАТ" << endl;
    cout << "============================================================" << endl;
    cout << "  Всего тестов: " << totalTests << endl;
    cout << "  Пройдено:     " << passedTests << endl;
    cout << "  Провалено:    " << (totalTests - passedTests) << endl;
    cout << "  Успешность:   " << (totalTests > 0 ? (passedTests * 100.0 / totalTests) : 0) << "%" << endl;
    
    return 0;
}