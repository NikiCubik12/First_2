#include <iostream>
#include <clocale>
using namespace std;
using std::cin;
using std::cout;
using out_of_range = std::out_of_range;
using invalid_argument = std::invalid_argument;
#include "Tests.hpp"
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"

bool run_test(void (*test_function)())
{
    int old_passed = passedTests;
    int old_total = totalTests;
    
    test_function();
    
    int new_passed = passedTests - old_passed;
    int new_total = totalTests - old_total;
    
    if (new_total > 0 && new_passed == new_total) {
        return true;  // Все проверки успешны
    }
    
    return false;  // Есть проваленные проверки
}

void run_all_tests()
{
    cout << "\n========== RUNNING ALL TESTS ==========\n" << endl;
    
    cout << "DynamicArray Tests:" << endl;
    cout << "  check_DynamicArray: " << (run_test(check_DynamicArray) ? "PASSED" : "FAILED") << endl;
    cout << "  check_DynamicArray_user: " << (run_test(check_DynamicArray_user) ? "PASSED" : "FAILED") << endl;
    cout << "  check_DynamicArray_copy: " << (run_test(check_DynamicArray_copy) ? "PASSED" : "FAILED") << endl;
    cout << "  check_DynamicArray_copy_list: " << (run_test(check_DynamicArray_copy_list) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Get_DynamicArray: " << (run_test(check_Get_DynamicArray) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetSize_DynamicArray: " << (run_test(check_GetSize_DynamicArray) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Set_DynamicArray: " << (run_test(check_Set_DynamicArray) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Resize_DynamicArray: " << (run_test(check_Resize_DynamicArray) ? "PASSED" : "FAILED") << endl;
    
    cout << "\nLinkedList Tests:" << endl;
    cout << "  check_LinkedList: " << (run_test(check_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_LinkedList_user: " << (run_test(check_LinkedList_user) ? "PASSED" : "FAILED") << endl;
    cout << "  check_LinkedList_copy: " << (run_test(check_LinkedList_copy) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetFirst_LinkedList: " << (run_test(check_GetFirst_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetLast_LinkedList: " << (run_test(check_GetLast_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Get_LinkedList: " << (run_test(check_Get_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetSubList_LinkedList: " << (run_test(check_GetSubList_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetLength_LinkedList: " << (run_test(check_GetLength_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Append_LinkedList: " << (run_test(check_Append_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Prepend_LinkedList: " << (run_test(check_Prepend_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_InsertAt_LinkedList: " << (run_test(check_InsertAt_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Concat_LinkedList: " << (run_test(check_Concat_LinkedList) ? "PASSED" : "FAILED") << endl;
    cout << "  check_ClearList: " << (run_test(check_ClearList) ? "PASSED" : "FAILED") << endl;
    
    cout << "\nBitSequence Tests:" << endl;
    cout << "  check_BitSequence: " << (run_test(check_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_BitSequence_user: " << (run_test(check_BitSequence_user) ? "PASSED" : "FAILED") << endl;
    cout << "  check_BitSequence_copy: " << (run_test(check_BitSequence_copy) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetFirst_BitSequence: " << (run_test(check_GetFirst_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetLast_BitSequence: " << (run_test(check_GetLast_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Get_BitSequence: " << (run_test(check_Get_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetSubsequence_BitSequence: " << (run_test(check_GetSubsequence_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_GetLength_BitSequence: " << (run_test(check_GetLength_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_AppendImpl_BitSequence: " << (run_test(check_AppendImpl_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_InsertAtImpl_BitSequence: " << (run_test(check_InsertAtImpl_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_PrependImpl_BitSequence: " << (run_test(check_PrependImpl_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_Concat_BitSequence: " << (run_test(check_Concat_BitSequence) ? "PASSED" : "FAILED") << endl;
    cout << "  check_BitAnd: " << (run_test(check_BitAnd) ? "PASSED" : "FAILED") << endl;
    cout << "  check_BitOr: " << (run_test(check_BitOr) ? "PASSED" : "FAILED") << endl;
    cout << "  check_BitXor: " << (run_test(check_BitXor) ? "PASSED" : "FAILED") << endl;
    cout << "  check_BitNot: " << (run_test(check_BitNot) ? "PASSED" : "FAILED") << endl;
    
    cout << "\n========== TEST RESULTS ==========" << endl;
    cout << "Total tests: " << totalTests << endl;
    cout << "Passed: " << passedTests << endl;
    cout << "Failed: " << (totalTests - passedTests) << endl;
    cout << "Success rate: " << (passedTests * 100.0 / totalTests) << "%" << endl;
}

int main()
{
    setlocale(LC_ALL, "Rus");
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