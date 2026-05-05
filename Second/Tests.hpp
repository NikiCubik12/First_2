#ifndef TESTS
#define TESTS

#include <iostream>
using namespace std;
using std::cin;
using std::cout;
using out_of_range = std::out_of_range;
using invalid_argument = std::invalid_argument;
using runtime_error = std::runtime_error;
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "BitSequence.hpp"

int totalTests = 0;
int passedTests = 0;

void check_int(int expected, int got)
{
    totalTests++;
    if (expected == got)
        passedTests++;
    else 
        cout << "FAILED: expected " << expected << ", got " << got << endl;
    
}

void check_DynamicArray ()
{
    DynamicArray <int>* arr = new DynamicArray<int>();

    check_int(arr->GetSize(), 0);
    delete arr;
}

void check_DynamicArray_user ()
{
    int mas[3] = {1, 2, 3};
    DynamicArray <int>* arr = new DynamicArray<int>(mas, 3);

    check_int(arr->GetSize(), 3);
    check_int(arr->Get(0), 2);
    check_int(arr->Get(1), 2);
    check_int(arr->Get(2), 3);
    delete arr;
}

void check_DynamicArray_copy ()
{
    int mas[3] = {1, 2, 3};
    DynamicArray <int>* arr1 = new DynamicArray<int>(mas, 3);
    DynamicArray <int>* arr2 = new DynamicArray<int>(*arr1);

    check_int(arr2->GetSize(), 3);
    check_int(arr2->Get(0), 1);
    check_int(arr2->Get(1), 2);
    check_int(arr2->Get(2), 3);
    delete arr1;
    delete arr2;
}

void check_DynamicArray_copy_list ()
{
    int mas[3] = {1, 2, 3};
    LinkedList <int>* list = new LinkedList<int>(mas, 3);
    DynamicArray <int>* arr = new DynamicArray<int>(*list);
    
    check_int(arr->GetSize(), 3);
    check_int(arr->Get(0), 1);
    check_int(arr->Get(1), 2);
    check_int(arr->Get(2), 3);
    delete arr;
    delete list;
}

void check_Get_DynamicArray ()
{
    int mas[3] = {1, 2, 3};
    DynamicArray <int>* arr = new DynamicArray<int>(mas, 3);
    
    check_int(arr->Get(1), 2);
    delete arr;
}

void check_GetSize_DynamicArray ()
{
    int mas[3] = {1, 2, 3};
    DynamicArray <int>* arr = new DynamicArray<int>(mas, 3);
    
    check_int(arr->GetSize(), 3);
    delete arr;
}

void check_Set_DynamicArray ()
{
    int mas[3] = {1, 2, 3};
    DynamicArray <int>* arr = new DynamicArray<int>(mas, 3);

    arr->Set(2, 4);
    check_int(arr->Get(2), 4);
    delete arr;
}

void check_Resize_DynamicArray ()
{
    int mas[3] = {1, 2, 3};
    DynamicArray <int>* arr = new DynamicArray<int>(mas, 3);

    arr->Resize(2);
    check_int(arr->GetSize(), 2);
    delete arr;
}



void check_LinkedList()
{
    LinkedList<int>* list = new LinkedList<int>();
    check_int(list->GetLength(), 0);
    delete list;
}

void check_LinkedList_user()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int>* list = new LinkedList<int>(mas, 3);
    
    check_int(list->GetLength(), 3);
    check_int(list->Get(0), 1);
    check_int(list->Get(1), 2);
    check_int(list->Get(2), 3);
    delete list;
}

void check_LinkedList_copy()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int>* list1 = new LinkedList<int>(mas, 3);
    LinkedList<int>* list2 = new LinkedList<int>(*list1);
    
    check_int(list2->GetLength(), 3);
    check_int(list2->Get(0), 1);
    check_int(list2->Get(1), 2);
    check_int(list2->Get(2), 3);
    delete list1;
    delete list2;
}

void check_GetFirst_LinkedList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int>* list = new LinkedList<int>(mas, 3);
    
    check_int(list->GetFirst(), 1);
    delete list;
}

void check_GetLast_LinkedList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int>* list = new LinkedList<int>(mas, 3);
    
    check_int(list->GetLast(), 3);
    delete list;
}

void check_Get_LinkedList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int>* list = new LinkedList<int>(mas, 3);
    
    check_int(list->Get(1), 2);
    delete list;
}

void check_GetSubList_LinkedList()
{
    int mas[5] = {1, 2, 3, 4, 5};
    LinkedList<int>* list = new LinkedList<int>(mas, 5);
    LinkedList<int>* sublist = list->GetSubList(1, 3);
    
    check_int(sublist->GetLength(), 3);
    check_int(sublist->Get(0), 2);
    check_int(sublist->Get(1), 3);
    check_int(sublist->Get(2), 4);
    delete list;
    delete sublist;
}

void check_GetLength_LinkedList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int>* list = new LinkedList<int>(mas, 3);
    
    check_int(list->GetLength(), 3);
    delete list;
}

void check_Append_LinkedList()
{
    LinkedList<int>* list = new LinkedList<int>();
    list->Append(1);
    list->Append(2);
    list->Append(3);
    
    check_int(list->GetLength(), 3);
    check_int(list->Get(0), 1);
    check_int(list->Get(1), 2);
    check_int(list->Get(2), 3);
    delete list;
}

void check_Prepend_LinkedList()
{
    LinkedList<int>* list = new LinkedList<int>();
    list->Prepend(3);
    list->Prepend(2);
    list->Prepend(1);
    
    check_int(list->GetLength(), 3);
    check_int(list->Get(0), 1);
    check_int(list->Get(1), 2);
    check_int(list->Get(2), 3);
    delete list;
}

void check_InsertAt_LinkedList()
{
    int mas[2] = {1, 3};
    LinkedList<int>* list = new LinkedList<int>(mas, 2);
    list->InsertAt(2, 1);
    
    check_int(list->GetLength(), 3);
    check_int(list->Get(0), 1);
    check_int(list->Get(1), 2);
    check_int(list->Get(2), 3);
    delete list;
}

void check_Concat_LinkedList()
{
    int mas1[2] = {1, 2};
    int mas2[2] = {3, 4};
    LinkedList<int>* list1 = new LinkedList<int>(mas1, 2);
    LinkedList<int>* list2 = new LinkedList<int>(mas2, 2);
    LinkedList<int>* result = list1->Concat(list2);
    
    check_int(result->GetLength(), 4);
    check_int(result->Get(0), 1);
    check_int(result->Get(1), 2);
    check_int(result->Get(2), 3);
    check_int(result->Get(3), 4);
    delete list1;
    delete list2;
    delete result;
}

void check_ClearList()
{
    int mas[3] = {1, 2, 3};
    LinkedList<int>* list = new LinkedList<int>(mas, 3);
    list->ClearList();
    
    check_int(list->GetLength(), 0);
    delete list;
}


void check_BitSequence()
{
    BitSequence* bits = new BitSequence();
    check_int(bits->GetLength(), 0);
    delete bits;
}

void check_BitSequence_user()
{
    int mas[3] = {1, 0, 1};
    BitSequence* bits = new BitSequence(mas, 3);
    
    check_int(bits->GetLength(), 3);
    check_int(bits->Get(0), 1);
    check_int(bits->Get(1), 0);
    check_int(bits->Get(2), 1);
    delete bits;
}

void check_BitSequence_copy()
{
    int mas[3] = {1, 0, 1};
    BitSequence* bits1 = new BitSequence(mas, 3);
    BitSequence* bits2 = new BitSequence(*bits1);
    
    check_int(bits2->GetLength(), 3);
    check_int(bits2->Get(0), 1);
    check_int(bits2->Get(1), 0);
    check_int(bits2->Get(2), 1);
    delete bits1;
    delete bits2;
}

void check_BitSequence_unsigned()
{
    BitSequence* bits = new BitSequence(5); // 101 в двоичной
    
    check_int(bits->GetLength(), 3);
    check_int(bits->Get(0), 1);
    check_int(bits->Get(1), 0);
    check_int(bits->Get(2), 1);
    delete bits;
}

void check_GetFirst_BitSequence()
{
    int mas[3] = {1, 0, 1};
    BitSequence* bits = new BitSequence(mas, 3);
    
    check_int(bits->GetFirst(), 1);
    delete bits;
}

void check_GetLast_BitSequence()
{
    int mas[3] = {1, 0, 1};
    BitSequence* bits = new BitSequence(mas, 3);
    
    check_int(bits->GetLast(), 1);
    delete bits;
}

void check_Get_BitSequence()
{
    int mas[3] = {1, 0, 1};
    BitSequence* bits = new BitSequence(mas, 3);
    
    check_int(bits->Get(1), 0);
    delete bits;
}

void check_GetSubsequence_BitSequence()
{
    int mas[5] = {1, 0, 1, 0, 1};
    BitSequence* bits = new BitSequence(mas, 5);
    BitSequence* sub = bits->GetSubsequence(1, 3);
    
    check_int(sub->GetLength(), 3);
    check_int(sub->Get(0), 0);
    check_int(sub->Get(1), 1);
    check_int(sub->Get(2), 0);
    delete bits;
    delete sub;
}

void check_GetLength_BitSequence()
{
    int mas[3] = {1, 0, 1};
    BitSequence* bits = new BitSequence(mas, 3);
    
    check_int(bits->GetLength(), 3);
    delete bits;
}

void check_AppendImpl_BitSequence()
{
    BitSequence* bits = new BitSequence();
    bits->AppendImpl(1);
    bits->AppendImpl(0);
    bits->AppendImpl(1);
    
    check_int(bits->GetLength(), 3);
    check_int(bits->Get(0), 1);
    check_int(bits->Get(1), 0);
    check_int(bits->Get(2), 1);
    delete bits;
}

void check_InsertAtImpl_BitSequence()
{
    int mas[2] = {1, 1};
    BitSequence* bits = new BitSequence(mas, 2);
    bits->InsertAtImpl(0, 1);  // Убрал присваивание, т.к. InsertAtImpl возвращает this

    check_int(bits->GetLength(), 3);
    check_int(bits->Get(0), 1);
    check_int(bits->Get(1), 0);
    check_int(bits->Get(2), 1);
    delete bits;
}

void check_PrependImpl_BitSequence()
{
    BitSequence* bits = new BitSequence();
    bits->PrependImpl(1);
    bits->PrependImpl(0);
    bits->PrependImpl(1);
    
    check_int(bits->GetLength(), 3);
    check_int(bits->Get(0), 1);
    check_int(bits->Get(1), 0);
    check_int(bits->Get(2), 1);
    delete bits;
}

void check_Concat_BitSequence()
{
    int mas1[2] = {1, 0};
    int mas2[2] = {1, 1};
    BitSequence* bits1 = new BitSequence(mas1, 2);
    BitSequence* bits2 = new BitSequence(mas2, 2);
    BitSequence* result = bits1->Concat(bits2);
    
    check_int(result->GetLength(), 4);
    check_int(result->Get(0), 1);
    check_int(result->Get(1), 0);
    check_int(result->Get(2), 1);
    check_int(result->Get(3), 1);
    delete bits1;
    delete result;
    delete bits2;
}

void check_BitAnd()
{
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    BitSequence* bits1 = new BitSequence(mas1, 3);
    BitSequence* bits2 = new BitSequence(mas2, 3);
    BitSequence* result = bits1->BitAnd(bits2);
    
    check_int(result->GetLength(), 3);
    check_int(result->Get(0), 1 & 1);  // 1
    check_int(result->Get(1), 0 & 1);  // 0
    check_int(result->Get(2), 1 & 0);  // 0
    delete bits1;
    delete bits2;
    delete result;
}

void check_BitOr()
{
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    BitSequence* bits1 = new BitSequence(mas1, 3);
    BitSequence* bits2 = new BitSequence(mas2, 3);
    BitSequence* result = bits1->BitOr(bits2);
    
    check_int(result->GetLength(), 3);
    check_int(result->Get(0), 1 | 1);  // 1
    check_int(result->Get(1), 0 | 1);  // 1
    check_int(result->Get(2), 1 | 0);  // 1
    delete bits1;
    delete bits2;
    delete result;
}

void check_BitXor()
{
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    BitSequence* bits1 = new BitSequence(mas1, 3);
    BitSequence* bits2 = new BitSequence(mas2, 3);
    BitSequence* result = bits1->BitXor(bits2);
    
    check_int(result->GetLength(), 3);
    check_int(result->Get(0), 1 ^ 1);  // 0
    check_int(result->Get(1), 0 ^ 1);  // 1
    check_int(result->Get(2), 1 ^ 0);  // 1
    delete bits1;
    delete bits2;
    delete result;
}

void check_BitNot()
{
    int mas[3] = {1, 0, 1};
    BitSequence* bits = new BitSequence(mas, 3);
    BitSequence* result = bits->BitNot();
    
    check_int(result->GetLength(), 3);
    check_int(result->Get(0), 0);
    check_int(result->Get(1), 1);
    check_int(result->Get(2), 0);
    delete bits;
    delete result;
}

#endif