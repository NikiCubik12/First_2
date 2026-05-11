#ifndef TESTS
#define TESTS

#include <iostream>
#include <climits>

using std::cin;
using std::cout;
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

int totalTests = 0;
int passedTests = 0;

void check_int(int expected, int got)
{
    totalTests++;
    if (expected == got)
        passedTests++;
    else 
        std::cerr << "FAILED: expected " << expected << ", got " << got << std::endl;
}


void check_DynamicArray()
{
    DynamicArray<int> arr;
    check_int(arr.GetSize(), 0);
}

void check_DynamicArray_user()
{
    int mas[3] = {1, 2, 3};
    DynamicArray<int> arr(mas, 3);
    
    check_int(arr.GetSize(), 3);
    check_int(arr.Get(0), 1);
    check_int(arr.Get(1), 2);
    check_int(arr.Get(2), 3);
}

void check_DynamicArray_copy()
{
    int mas[3] = {1, 2, 3};
    DynamicArray<int> arr1(mas, 3);
    DynamicArray<int> arr2(arr1);
    
    check_int(arr2.GetSize(), 3);
    check_int(arr2.Get(0), 1);
    check_int(arr2.Get(1), 2);
    check_int(arr2.Get(2), 3);
}

void check_DynamicArray_copy_list()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int> list(mas, 3);
    DynamicArray<int> arr(list);
    
    check_int(arr.GetSize(), 3);
    check_int(arr.Get(0), 1);
    check_int(arr.Get(1), 2);
    check_int(arr.Get(2), 3);
}

void check_Get_DynamicArray()
{
    int mas[3] = {1, 2, 3};
    DynamicArray<int> arr(mas, 3);
    
    check_int(arr.Get(1), 2);
}

void check_GetSize_DynamicArray()
{
    int mas[3] = {1, 2, 3};
    DynamicArray<int> arr(mas, 3);
    
    check_int(arr.GetSize(), 3);
}

void check_Set_DynamicArray()
{
    int mas[3] = {1, 2, 3};
    DynamicArray<int> arr(mas, 3);
    
    arr.Set(2, 4);
    check_int(arr.Get(2), 4);
}

void check_Resize_DynamicArray()
{
    int mas[3] = {1, 2, 3};
    DynamicArray<int> arr(mas, 3);
    
    arr.Resize(2);
    check_int(arr.GetSize(), 2);
}


void check_LinkedList()
{
    LinkedList<int> list;
    check_int(list.GetLength(), 0);
}

void check_LinkedList_user()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int> list(mas, 3);
    
    check_int(list.GetLength(), 3);
    check_int(list.Get(0), 1);
    check_int(list.Get(1), 2);
    check_int(list.Get(2), 3);
}

void check_LinkedList_copy()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int> list1(mas, 3);
    LinkedList<int> list2(list1);
    
    check_int(list2.GetLength(), 3);
    check_int(list2.Get(0), 1);
    check_int(list2.Get(1), 2);
    check_int(list2.Get(2), 3);
}

void check_GetFirst_LinkedList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int> list(mas, 3);
    
    check_int(list.GetFirst(), 1);
}

void check_GetLast_LinkedList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int> list(mas, 3);
    
    check_int(list.GetLast(), 3);
}

void check_Get_LinkedList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int> list(mas, 3);
    
    check_int(list.Get(1), 2);
}

void check_GetSubList_LinkedList()
{
    int mas[5] = {1, 2, 3, 4, 5};
    LinkedList<int> list(mas, 5);
    LinkedList<int>* sublist = list.GetSubList(1, 3);
    
    check_int(sublist->GetLength(), 3);
    check_int(sublist->Get(0), 2);
    check_int(sublist->Get(1), 3);
    check_int(sublist->Get(2), 4);
    delete sublist;
}

void check_GetLength_LinkedList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int> list(mas, 3);
    
    check_int(list.GetLength(), 3);
}

void check_Append_LinkedList()
{
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);
    
    check_int(list.GetLength(), 3);
    check_int(list.Get(0), 1);
    check_int(list.Get(1), 2);
    check_int(list.Get(2), 3);
}

void check_Prepend_LinkedList()
{
    LinkedList<int> list;
    list.Prepend(3);
    list.Prepend(2);
    list.Prepend(1);
    
    check_int(list.GetLength(), 3);
    check_int(list.Get(0), 1);
    check_int(list.Get(1), 2);
    check_int(list.Get(2), 3);
}

void check_InsertAt_LinkedList()
{
    int mas[2] = {1, 3};
    LinkedList<int> list(mas, 2);
    list.InsertAt(2, 1);
    
    check_int(list.GetLength(), 3);
    check_int(list.Get(0), 1);
    check_int(list.Get(1), 2);
    check_int(list.Get(2), 3);
}

void check_Concat_LinkedList()
{
    int mas1[2] = {1, 2};
    int mas2[2] = {3, 4};
    LinkedList<int> list1(mas1, 2);
    LinkedList<int> list2(mas2, 2);
    LinkedList<int>* result = list1.Concat(&list2);
    
    check_int(result->GetLength(), 4);
    check_int(result->Get(0), 1);
    check_int(result->Get(1), 2);
    check_int(result->Get(2), 3);
    check_int(result->Get(3), 4);
    delete result;
}

void check_ClearList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int> list(mas, 3);
    list.ClearList();
    
    check_int(list.GetLength(), 0);
}


void check_BitSequence()
{
    BitSequence bits;
    check_int(bits.GetLength(), 0);
}

void check_BitSequence_user()
{
    int mas[3] = {1, 0, 1};
    BitSequence bits(mas, 3);
    
    check_int(bits.GetLength(), 3);
    check_int(bits.Get(0), 1);
    check_int(bits.Get(1), 0);
    check_int(bits.Get(2), 1);
}

void check_BitSequence_copy()
{
    int mas[3] = {1, 0, 1};
    BitSequence bits1(mas, 3);
    BitSequence bits2(bits1);
    
    check_int(bits2.GetLength(), 3);
    check_int(bits2.Get(0), 1);
    check_int(bits2.Get(1), 0);
    check_int(bits2.Get(2), 1);
}

void check_GetFirst_BitSequence()
{
    int mas[3] = {1, 0, 1};
    BitSequence bits(mas, 3);
    
    check_int(bits.GetFirst(), 1);
}

void check_GetLast_BitSequence()
{
    int mas[3] = {1, 0, 1};
    BitSequence bits(mas, 3);
    
    check_int(bits.GetLast(), 1);
}

void check_Get_BitSequence()
{
    int mas[3] = {1, 0, 1};
    BitSequence bits(mas, 3);
    
    check_int(bits.Get(1), 0);
}

void check_GetSubsequence_BitSequence()
{
    int mas[5] = {1, 0, 1, 0, 1};
    BitSequence bits(mas, 5);
    BitSequence* sub = bits.GetSubsequence(1, 3);
    
    check_int(sub->GetLength(), 3);
    check_int(sub->Get(0), 0);
    check_int(sub->Get(1), 1);
    check_int(sub->Get(2), 0);
    delete sub;
}

void check_GetLength_BitSequence()
{
    int mas[3] = {1, 0, 1};
    BitSequence bits(mas, 3);
    
    check_int(bits.GetLength(), 3);
}

void check_AppendImpl_BitSequence()
{
    BitSequence bits;
    bits.Append(1);
    bits.Append(0);
    bits.Append(1);
    
    check_int(bits.GetLength(), 3);
    check_int(bits.Get(0), 1);
    check_int(bits.Get(1), 0);
    check_int(bits.Get(2), 1);
}

void check_InsertAtImpl_BitSequence()
{
    int mas[2] = {1, 1};
    BitSequence bits(mas, 2);
    bits.InsertAt(0, 1);
    
    check_int(bits.GetLength(), 3);
    check_int(bits.Get(0), 1);
    check_int(bits.Get(1), 0);
    check_int(bits.Get(2), 1);
}

void check_PrependImpl_BitSequence()
{
    BitSequence bits;
    bits.Prepend(1);
    bits.Prepend(0);
    bits.Prepend(1);
    
    check_int(bits.GetLength(), 3);
    check_int(bits.Get(0), 1);
    check_int(bits.Get(1), 0);
    check_int(bits.Get(2), 1);
}

void check_Concat_BitSequence()
{
    int mas1[2] = {1, 0};
    int mas2[2] = {1, 1};
    BitSequence bits1(mas1, 2);
    BitSequence bits2(mas2, 2);
    Sequence<int>* result = bits1.Concat(&bits2);
    
    check_int(result->GetLength(), 4);
    check_int(result->Get(0), 1);
    check_int(result->Get(1), 0);
    check_int(result->Get(2), 1);
    check_int(result->Get(3), 1);
    delete result;
}

void check_BitAnd()
{
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    BitSequence bits1(mas1, 3);
    BitSequence bits2(mas2, 3);
    BitSequence* result = bits1.BitAnd(&bits2);
    
    check_int(result->GetLength(), 3);
    check_int(result->Get(0), 1 & 1);
    check_int(result->Get(1), 0 & 1);
    check_int(result->Get(2), 1 & 0);
    delete result;
}

void check_BitOr()
{
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    BitSequence bits1(mas1, 3);
    BitSequence bits2(mas2, 3);
    BitSequence* result = bits1.BitOr(&bits2);
    
    check_int(result->GetLength(), 3);
    check_int(result->Get(0), 1 | 1);
    check_int(result->Get(1), 0 | 1);
    check_int(result->Get(2), 1 | 0);
    delete result;
}

void check_BitXor()
{
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    BitSequence bits1(mas1, 3);
    BitSequence bits2(mas2, 3);
    BitSequence* result = bits1.BitXor(&bits2);
    
    check_int(result->GetLength(), 3);
    check_int(result->Get(0), 1 ^ 1);
    check_int(result->Get(1), 0 ^ 1);
    check_int(result->Get(2), 1 ^ 0);
    delete result;
}

void check_BitNot()
{
    int mas[3] = {1, 0, 1};
    BitSequence bits(mas, 3);
    BitSequence* result = bits.BitNot();
    
    check_int(result->GetLength(), 3);
    check_int(result->Get(0), 0);
    check_int(result->Get(1), 1);
    check_int(result->Get(2), 0);
    delete result;
}

void check_initializer_list_dynamic_array()
{
    DynamicArray<int> arr = {1, 2, 3, 4, 5};
    
    check_int(arr.GetSize(), 5);
    check_int(arr.Get(0), 1);
    check_int(arr.Get(1), 2);
    check_int(arr.Get(2), 3);
    check_int(arr.Get(3), 4);
    check_int(arr.Get(4), 5);
}

void check_initializer_list_array_sequence()
{
    ArraySequence<int> seq = {1, 2, 3, 4, 5};
    
    check_int(seq.GetLength(), 5);
    check_int(seq.GetFirst(), 1);
    check_int(seq.GetLast(), 5);
    check_int(seq.Get(0), 1);
    check_int(seq.Get(2), 3);
    check_int(seq.Get(4), 5);
}

void check_initializer_list_mutable_array_sequence()
{
    MutableArraySequence<int> seq = {10, 20, 30, 40, 50};
    
    check_int(seq.GetLength(), 5);
    check_int(seq.GetFirst(), 10);
    check_int(seq.GetLast(), 50);
    check_int(seq.Get(0), 10);
    check_int(seq.Get(2), 30);
    check_int(seq.Get(4), 50);
    
    seq.Append(60);
    check_int(seq.GetLength(), 6);
    check_int(seq.Get(5), 60);
    
    seq.Prepend(5);
    check_int(seq.GetLength(), 7);
    check_int(seq.GetFirst(), 5);
    
    // Не вызываем Clear() и не создаем проблемы
}

void check_initializer_list_immutable_array_sequence()
{
    ImmutableArraySequence<int> seq = {1, 2, 3, 4, 5};
    
    check_int(seq.GetLength(), 5);
    check_int(seq.Get(0), 1);
    check_int(seq.Get(2), 3);
    check_int(seq.Get(4), 5);
    
    
    Sequence<int>* newSeq = seq.Append(6);
    check_int(seq.GetLength(), 5);  
    check_int(newSeq->GetLength(), 6);  
    
    delete newSeq;
}

void check_initializer_list_list_sequence()
{
    ListSequence<int> seq = {100, 200, 300, 400, 500};
    
    check_int(seq.GetLength(), 5);
    check_int(seq.GetFirst(), 100);
    check_int(seq.GetLast(), 500);
    check_int(seq.Get(0), 100);
    check_int(seq.Get(2), 300);
    check_int(seq.Get(4), 500);
}

void check_initializer_list_bit_sequence()
{
    BitSequence seq = {1, 0, 1, 0, 1};
    
    check_int(seq.GetLength(), 5);
    check_int(seq.GetFirst(), 1);
    check_int(seq.GetLast(), 1);
    check_int(seq.Get(0), 1);
    check_int(seq.Get(1), 0);
    check_int(seq.Get(2), 1);
    check_int(seq.Get(3), 0);
    check_int(seq.Get(4), 1);
}

void check_initializer_list_bit_sequence_invalid()
{
    bool exception_caught = false;
    try 
    {
        BitSequence seq = {1, 0, 2, 0, 1};
    }
    catch (const InvalidBitException& e) 
    {
        exception_caught = true;
    }
    
    totalTests++;
    if (exception_caught)
        passedTests++;
    else 
        std::cerr << "FAILED: expected InvalidBitException for bit sequence with value 2" << std::endl;
}


#endif