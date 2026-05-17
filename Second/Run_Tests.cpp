#include <iostream>
#include "Tests.hpp"

using namespace std;

void run_test(void (*test_function)(), const string& name)
{
    cout << "  " << name << ": ";
    
    int old_passed = passedTests;
    int old_total = totalTests;
    
    test_function();
    
    int new_passed = passedTests - old_passed;
    int new_total = totalTests - old_total;
    
    if (new_total > 0 && new_passed == new_total)
        cout << "PASSED" << endl;
    else
        cout << "FAILED" << endl;
}

void run_all_tests()
{
    cout << "\n========== RUNNING ALL TESTS ==========\n" << endl;
    
    cout << "DynamicArray Tests:" << endl;
    run_test(check_DynamicArray, "check_DynamicArray");
    run_test(check_DynamicArray_user, "check_DynamicArray_user");
    run_test(check_DynamicArray_copy, "check_DynamicArray_copy");
    run_test(check_DynamicArray_copy_list, "check_DynamicArray_copy_list");
    run_test(check_Get_DynamicArray, "check_Get_DynamicArray");
    run_test(check_GetSize_DynamicArray, "check_GetSize_DynamicArray");
    run_test(check_Set_DynamicArray, "check_Set_DynamicArray");
    run_test(check_Resize_DynamicArray, "check_Resize_DynamicArray");
    
    cout << "\nLinkedList Tests:" << endl;
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
    
    cout << "\nBitSequence Tests:" << endl;
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
    
    cout << "\nInitializer List Tests:" << endl;
    run_test(check_initializer_list_dynamic_array, "check_initializer_list_dynamic_array");
    run_test(check_initializer_list_array_sequence, "check_initializer_list_array_sequence");
    run_test(check_initializer_list_list_sequence, "check_initializer_list_list_sequence");
    run_test(check_initializer_list_bit_sequence, "check_initializer_list_bit_sequence");
    run_test(check_initializer_list_bit_sequence_invalid, "check_initializer_list_bit_sequence_invalid");
    
    cout << "\n========== TEST RESULTS ==========" << endl;
    cout << "Total tests: " << totalTests << endl;
    cout << "Passed: " << passedTests << endl;
    cout << "Failed: " << (totalTests - passedTests) << endl;
    cout << "Success rate: " << (passedTests * 100.0 / totalTests) << "%" << endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    try 
    {
        run_all_tests();
    } 
    catch (const invalid_argument& except)
    {
        cout << except.what() << endl;
    }
    catch (const out_of_range& except)
    {
        cout << except.what() << endl;
    }
    
    return 0;
}