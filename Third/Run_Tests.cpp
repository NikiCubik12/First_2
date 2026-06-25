#include <iostream>
#include "Tests.hpp"
#include <clocale>

using namespace std;

void run_test(void (*test_function)(), const string& name)
{
    setlocale(LC_ALL, "Rus");
    cout << "========================================" << endl;
    cout << "Running: " << name << endl;
    cout << "========================================" << endl;
    
    int old_passed = passedTests;
    int old_total = totalTests;
    
    test_function();
    
    int new_passed = passedTests - old_passed;
    int new_total = totalTests - old_total;
    
    print_separator();
    if (new_total > 0 && new_passed == new_total) {
        cout << "[PASS] " << name << " (" << new_passed << "/" << new_total << ")" << endl;
    } else {
        cout << "[FAIL] " << name << " (" << new_passed << "/" << new_total << ")" << endl;
    }
    cout << "========================================" << endl << endl;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    
    cout << "\n============================================================" << endl;
    cout << "                 RUNNING ALL TESTS" << endl;
    cout << "============================================================" << endl;
    
    run_test(check_DynamicArray, "check_DynamicArray");
    run_test(check_DynamicArray_user, "check_DynamicArray_user");
    run_test(check_DynamicArray_copy, "check_DynamicArray_copy");
    run_test(check_DynamicArray_copy_list, "check_DynamicArray_copy_list");
    run_test(check_Get_DynamicArray, "check_Get_DynamicArray");
    run_test(check_GetSize_DynamicArray, "check_GetSize_DynamicArray");
    run_test(check_Set_DynamicArray, "check_Set_DynamicArray");
    run_test(check_Resize_DynamicArray, "check_Resize_DynamicArray");

    run_test(check_LinkedList, "check_LinkedList");
    run_test(check_LinkedList_user, "check_LinkedList_user");
    run_test(check_LinkedList_copy, "check_LinkedList_copy");
    run_test(check_GetFirst_LinkedList, "check_GetFirst_LinkedList");
    run_test(check_GetLast_LinkedList, "check_GetLast_LinkedList");
    run_test(check_Get_LinkedList, "check_Get_LinkedList");
    run_test(check_GetSubList_LinkedList, "check_GetSubList_LinkedList");
    run_test(check_GetLength_LinkedList, "check_GetLength_LinkedList");
    run_test(check_Append_LinkedList, "check_Append_LinkedList");
    run_test(check_Prepend_LinkedList, "check_Prepend_LinkedList");
    run_test(check_InsertAt_LinkedList, "check_InsertAt_LinkedList");
    run_test(check_Concat_LinkedList, "check_Concat_LinkedList");
    run_test(check_ClearList, "check_ClearList");

    run_test(check_BitSequence, "check_BitSequence");
    run_test(check_BitSequence_user, "check_BitSequence_user");
    run_test(check_BitSequence_copy, "check_BitSequence_copy");
    run_test(check_GetFirst_BitSequence, "check_GetFirst_BitSequence");
    run_test(check_GetLast_BitSequence, "check_GetLast_BitSequence");
    run_test(check_Get_BitSequence, "check_Get_BitSequence");
    run_test(check_GetSubsequence_BitSequence, "check_GetSubsequence_BitSequence");
    run_test(check_GetLength_BitSequence, "check_GetLength_BitSequence");
    run_test(check_AppendImpl_BitSequence, "check_AppendImpl_BitSequence");
    run_test(check_InsertAtImpl_BitSequence, "check_InsertAtImpl_BitSequence");
    run_test(check_PrependImpl_BitSequence, "check_PrependImpl_BitSequence");
    run_test(check_Concat_BitSequence, "check_Concat_BitSequence");
    run_test(check_BitAnd, "check_BitAnd");
    run_test(check_BitOr, "check_BitOr");
    run_test(check_BitXor, "check_BitXor");
    run_test(check_BitNot, "check_BitNot");

    run_test(check_initializer_list_dynamic_array, "check_initializer_list_dynamic_array");
    run_test(check_initializer_list_array_sequence, "check_initializer_list_array_sequence");
    run_test(check_initializer_list_list_sequence, "check_initializer_list_list_sequence");
    run_test(check_initializer_list_bit_sequence, "check_initializer_list_bit_sequence");
    run_test(check_initializer_list_bit_sequence_invalid, "check_initializer_list_bit_sequence_invalid");
    
    cout << "\n============================================================" << endl;
    cout << "                 FINAL RESULTS" << endl;
    cout << "============================================================" << endl;
    cout << "  Total tests: " << totalTests << endl;
    cout << "  Passed:      " << passedTests << endl;
    cout << "  Failed:      " << (totalTests - passedTests) << endl;
    cout << "  Success rate: " << (totalTests > 0 ? (passedTests * 100.0 / totalTests) : 0) << "%" << endl;
    
    return 0;
}