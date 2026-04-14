#ifndef TESTS
#define TESTS

#include <iostream>
using namespace std;
using std::cin, std::cout, std::out_of_range, std::invalid_argument
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"

extern int totalTests = 0;
extern int passedTests = 0;

void check_double(double expected, double got)
void check_int(int expected, int got);

void check_DynamicArray ();
void check_DynamicArray_user ();
void check_DynamicArray_copy ();
void check_DynamicArray_copy_list ();
void check_Get_DynamicArray ();
void check_GetSize_DynamicArray ();
void check_Set_DynamicArray ();
void check_Resize_DynamicArray ();

void check_LinkedList ();
void check_LinkedList_user ();
void check_LinkedList_copy ();
void check_GetFirst_copy_list ();
void check_GetLast_LinkedList ();
void check_Get_LinkedList ();
void check_GetSubList_LinkedList ();
void check_GetLength_LinkedList ();
void check_Append_LinkedList ();
void check_Prepend_LinkedList ();
void check_InsertAt_LinkedList ();
void check_Concat_LinkedList ();
void check_ClearList();

void check_ArraySequence ();
void check_ArraySequence_user ();
void check_ArraySequence_copy ();
void check_GetFirst_ArraySequence ();
void check_GetLast_ArraySequence ();
void check_Get_ArraySequence ();
void check_GetSubsequence_ArraySequence ();
void check_GetLength_ArraySequence ();
void check_AppendImpl_ArraySequence ();
void check_InsertAtImpl_ArraySequence ();
void check_PrependImpl_ArraySequence ();
void check_Concat_ArraySequence ();

void check_ListSequence ();
void check_ListSequence_user ();
void check_ListSequence_copy ();
void check_GetFirst_ListSequence ();
void check_GetLast_ListSequence ();
void check_Get_ListSequence ();
void check_GetSubsequence_ListSequence ();
void check_GetLength_ListSequence ();
void check_AppendImpl_ListSequence ();
void check_InsertAtImpl_ListSequence ();
void check_PrependImpl_ListSequence ();
void check_Concat_ListSequence ();

void check_BitSequence ();
void check_BitSequence_user ();
void check_BitSequence_copy ();
void check_GetFirst_BitSequence ();
void check_GetLast_BitSequence();
void check_Get_BitSequence ();
void check_GetSubsequence_BitSequence ();
void check_GetLength_BitSequence ();
void check_AppendImpl_BitSequence ();
void check_InsertAtImpl_BitSequence ();
void check_PrependImpl_BitSequence ();
void check_Concat_BitSequence ();
void check_BitAnd ();
void check_BitOr ();
void check_BitXor ();
void check_BitNot ();

#endif