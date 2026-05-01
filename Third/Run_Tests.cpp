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



void run_all_tests()
{
    cout << "\n========== RUNNING ALL TESTS ==========\n" << endl;
    
    cout << "DynamicArray Tests:" << endl;
    check_DynamicArray();
    check_DynamicArray_user();
    check_DynamicArray_copy();
    check_DynamicArray_copy_list();
    check_Get_DynamicArray();
    check_GetSize_DynamicArray();
    check_Set_DynamicArray();
    check_Resize_DynamicArray();
    
    cout << "\nLinkedList Tests:" << endl;
    check_LinkedList();
    check_LinkedList_user();
    check_LinkedList_copy();
    check_GetFirst_LinkedList();
    check_GetLast_LinkedList();
    check_Get_LinkedList();
    check_GetSubList_LinkedList();
    check_GetLength_LinkedList();
    check_Append_LinkedList();
    check_Prepend_LinkedList();
    check_InsertAt_LinkedList();
    check_Concat_LinkedList();
    check_ClearList();

    cout << "\nArraySequence Tests:" << endl;
    check_ArraySequence();
    check_ArraySequence_user();
    check_ArraySequence_copy();
    check_ArraySequence_from_LinkedList();

    
    cout << "\nBitSequence Tests:" << endl;
    check_BitSequence();
    check_BitSequence_user();
    check_BitSequence_copy();
    check_GetFirst_BitSequence();
    check_GetLast_BitSequence();
    check_Get_BitSequence();
    check_GetSubsequence_BitSequence();
    check_GetLength_BitSequence();
    check_AppendImpl_BitSequence();
    check_InsertAtImpl_BitSequence();
    check_PrependImpl_BitSequence();
    check_Concat_BitSequence();
    check_BitAnd();
    check_BitOr();
    check_BitXor();
    check_BitNot();


    
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
    catch (invalid_argument except)
    {
        cout << except.what() << endl;
    }
    catch (out_of_range except)
    {
        cout << except.what() << endl;
    }
    
    return 0;
}