#ifndef TESTS
#define TESTS

#include <iostream>
#include <climits>
#include <string>
#include <exception>
#include <clocale>

using std::cin;
using std::cout;
using std::endl;
using std::string;
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

void print_separator()
{
    setlocale(LC_ALL, "Rus");
    cout << "------------------------------------------------------------" << endl;
}

void print_test_header(const string& name)
{
    cout << "\n============================================================" << endl;
    cout << "  " << name << endl;
    cout << "============================================================" << endl;
}

void check_int(int expected, int got, const string& testName, const string& input = "", const string& context = "")
{
    totalTests++;
    if (expected == got) {
        passedTests++;
        cout << "  [PASS] " << testName;
        if (!input.empty()) {
            cout << " [input: " << input << "]";
        }
        if (!context.empty()) {
            cout << " (" << context << ")";
        }
        cout << " -> expected " << expected << ", got " << got << endl;
    } else {
        cout << "  [FAIL] " << testName;
        if (!input.empty()) {
            cout << " [input: " << input << "]";
        }
        if (!context.empty()) {
            cout << " (" << context << ")";
        }
        cout << " -> expected " << expected << ", got " << got << endl;
    }
}

void check_size_t(size_t expected, size_t got, const string& testName, const string& input = "", const string& context = "")
{
    totalTests++;
    if (expected == got) {
        passedTests++;
        cout << "  [PASS] " << testName;
        if (!input.empty()) {
            cout << " [input: " << input << "]";
        }
        if (!context.empty()) {
            cout << " (" << context << ")";
        }
        cout << " -> expected " << expected << ", got " << got << endl;
    } else {
        cout << "  [FAIL] " << testName;
        if (!input.empty()) {
            cout << " [input: " << input << "]";
        }
        if (!context.empty()) {
            cout << " (" << context << ")";
        }
        cout << " -> expected " << expected << ", got " << got << endl;
    }
}

void check_DynamicArray()
{
    print_test_header("check_DynamicArray");
    cout << "  Input: no parameters" << endl;
    cout << "  Call: DynamicArray<int> arr;" << endl;
    try {
        DynamicArray<int> arr;
        cout << "  Result: object created" << endl;
        cout << "  Check: arr.GetSize()" << endl;
        check_size_t(0, arr.GetSize(), "GetSize()", "no parameters", "empty constructor");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_DynamicArray_user()
{
    print_test_header("check_DynamicArray_user");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}, size 3" << endl;
    cout << "  Call: DynamicArray<int> arr(mas, 3)" << endl;
    try {
        DynamicArray<int> arr(mas, 3);
        cout << "  Result: object created" << endl;
        cout << "  Check: arr.GetSize()" << endl;
        check_size_t(3, arr.GetSize(), "GetSize()", "array {1,2,3}", "constructor from array");
        cout << "  Check: arr.Get(0)" << endl;
        check_int(1, arr.Get(0), "Get(0)", "index 0", "constructor from array");
        cout << "  Check: arr.Get(1)" << endl;
        check_int(2, arr.Get(1), "Get(1)", "index 1", "constructor from array");
        cout << "  Check: arr.Get(2)" << endl;
        check_int(3, arr.Get(2), "Get(2)", "index 2", "constructor from array");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_DynamicArray_copy()
{
    print_test_header("check_DynamicArray_copy");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: DynamicArray<int> arr1(mas, 3)" << endl;
    cout << "  Call: DynamicArray<int> arr2(arr1)" << endl;
    try {
        DynamicArray<int> arr1(mas, 3);
        DynamicArray<int> arr2(arr1);
        cout << "  Result: arr2 created" << endl;
        cout << "  Check: arr2.GetSize()" << endl;
        check_size_t(3, arr2.GetSize(), "GetSize()", "copy of arr1", "copy constructor");
        cout << "  Check: arr2.Get(0)" << endl;
        check_int(1, arr2.Get(0), "Get(0)", "index 0", "copy constructor");
        cout << "  Check: arr2.Get(1)" << endl;
        check_int(2, arr2.Get(1), "Get(1)", "index 1", "copy constructor");
        cout << "  Check: arr2.Get(2)" << endl;
        check_int(3, arr2.Get(2), "Get(2)", "index 2", "copy constructor");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_DynamicArray_copy_list()
{
    print_test_header("check_DynamicArray_copy_list");
    int mas[3] = {1, 2, 3};
    cout << "  Input: list {1, 2, 3}" << endl;
    cout << "  Call: LinkedList<int> list(mas, 3)" << endl;
    cout << "  Call: DynamicArray<int> arr(list)" << endl;
    try {
        LinkedList<int> list(mas, 3);
        DynamicArray<int> arr(list);
        cout << "  Result: arr created" << endl;
        cout << "  Check: arr.GetSize()" << endl;
        check_size_t(3, arr.GetSize(), "GetSize()", "from list", "constructor from list");
        cout << "  Check: arr.Get(0)" << endl;
        check_int(1, arr.Get(0), "Get(0)", "index 0", "constructor from list");
        cout << "  Check: arr.Get(1)" << endl;
        check_int(2, arr.Get(1), "Get(1)", "index 1", "constructor from list");
        cout << "  Check: arr.Get(2)" << endl;
        check_int(3, arr.Get(2), "Get(2)", "index 2", "constructor from list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_Get_DynamicArray()
{
    print_test_header("check_Get_DynamicArray");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: DynamicArray<int> arr(mas, 3)" << endl;
    cout << "  Check: arr.Get(1)" << endl;
    try {
        DynamicArray<int> arr(mas, 3);
        check_int(2, arr.Get(1), "Get(1)", "index 1", "check Get");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_GetSize_DynamicArray()
{
    print_test_header("check_GetSize_DynamicArray");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: DynamicArray<int> arr(mas, 3)" << endl;
    cout << "  Check: arr.GetSize()" << endl;
    try {
        DynamicArray<int> arr(mas, 3);
        check_size_t(3, arr.GetSize(), "GetSize()", "array {1,2,3}", "check GetSize");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_Set_DynamicArray()
{
    print_test_header("check_Set_DynamicArray");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: arr.Set(2, 4)" << endl;
    cout << "  Expected: arr[2] = 4" << endl;
    try {
        DynamicArray<int> arr(mas, 3);
        arr.Set(2, 4);
        cout << "  Check: arr.Get(2)" << endl;
        check_int(4, arr.Get(2), "Get(2)", "index 2 (modified)", "check Set");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_Resize_DynamicArray()
{
    print_test_header("check_Resize_DynamicArray");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: arr.Resize(2)" << endl;
    cout << "  Expected: size 2" << endl;
    try {
        DynamicArray<int> arr(mas, 3);
        arr.Resize(2);
        cout << "  Check: arr.GetSize()" << endl;
        check_size_t(2, arr.GetSize(), "GetSize()", "new size 2", "check Resize");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_LinkedList()
{
    print_test_header("check_LinkedList");
    cout << "  Input: no parameters" << endl;
    cout << "  Call: LinkedList<int> list;" << endl;
    try {
        LinkedList<int> list;
        cout << "  Result: object created" << endl;
        cout << "  Check: list.GetLength()" << endl;
        check_size_t(0, list.GetLength(), "GetLength()", "no parameters", "empty constructor list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_LinkedList_user()
{
    print_test_header("check_LinkedList_user");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}, size 3" << endl;
    cout << "  Call: LinkedList<int> list(mas, 3)" << endl;
    try {
        LinkedList<int> list(mas, 3);
        cout << "  Result: object created" << endl;
        cout << "  Check: list.GetLength()" << endl;
        check_size_t(3, list.GetLength(), "GetLength()", "array {1,2,3}", "constructor from array list");
        cout << "  Check: list.Get(0)" << endl;
        check_int(1, list.Get(0), "Get(0)", "index 0", "constructor from array list");
        cout << "  Check: list.Get(1)" << endl;
        check_int(2, list.Get(1), "Get(1)", "index 1", "constructor from array list");
        cout << "  Check: list.Get(2)" << endl;
        check_int(3, list.Get(2), "Get(2)", "index 2", "constructor from array list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_LinkedList_copy()
{
    print_test_header("check_LinkedList_copy");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: LinkedList<int> list1(mas, 3)" << endl;
    cout << "  Call: LinkedList<int> list2(list1)" << endl;
    try {
        LinkedList<int> list1(mas, 3);
        LinkedList<int> list2(list1);
        cout << "  Result: list2 created" << endl;
        cout << "  Check: list2.GetLength()" << endl;
        check_size_t(3, list2.GetLength(), "GetLength()", "copy of list1", "copy constructor list");
        cout << "  Check: list2.Get(0)" << endl;
        check_int(1, list2.Get(0), "Get(0)", "index 0", "copy constructor list");
        cout << "  Check: list2.Get(1)" << endl;
        check_int(2, list2.Get(1), "Get(1)", "index 1", "copy constructor list");
        cout << "  Check: list2.Get(2)" << endl;
        check_int(3, list2.Get(2), "Get(2)", "index 2", "copy constructor list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_GetFirst_LinkedList()
{
    print_test_header("check_GetFirst_LinkedList");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: LinkedList<int> list(mas, 3)" << endl;
    cout << "  Check: list.GetFirst()" << endl;
    try {
        LinkedList<int> list(mas, 3);
        check_int(1, list.GetFirst(), "GetFirst()", "array {1,2,3}", "check GetFirst");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_GetLast_LinkedList()
{
    print_test_header("check_GetLast_LinkedList");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: LinkedList<int> list(mas, 3)" << endl;
    cout << "  Check: list.GetLast()" << endl;
    try {
        LinkedList<int> list(mas, 3);
        check_int(3, list.GetLast(), "GetLast()", "array {1,2,3}", "check GetLast");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_Get_LinkedList()
{
    print_test_header("check_Get_LinkedList");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: LinkedList<int> list(mas, 3)" << endl;
    cout << "  Check: list.Get(1)" << endl;
    try {
        LinkedList<int> list(mas, 3);
        check_int(2, list.Get(1), "Get(1)", "index 1", "check Get list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_GetSubList_LinkedList()
{
    print_test_header("check_GetSubList_LinkedList");
    int mas[5] = {1, 2, 3, 4, 5};
    cout << "  Input: array {1, 2, 3, 4, 5}" << endl;
    cout << "  Call: LinkedList<int> list(mas, 5)" << endl;
    cout << "  Call: list.GetSubList(1, 3)" << endl;
    cout << "  Expected: sublist {2, 3, 4}" << endl;
    try {
        LinkedList<int> list(mas, 5);
        LinkedList<int>* sublist = list.GetSubList(1, 3);
        cout << "  Result: sublist created" << endl;
        cout << "  Check: sublist->GetLength()" << endl;
        check_size_t(3, sublist->GetLength(), "GetLength()", "sublist", "check GetSubList");
        cout << "  Check: sublist->Get(0)" << endl;
        check_int(2, sublist->Get(0), "Get(0)", "index 0", "check GetSubList");
        cout << "  Check: sublist->Get(1)" << endl;
        check_int(3, sublist->Get(1), "Get(1)", "index 1", "check GetSubList");
        cout << "  Check: sublist->Get(2)" << endl;
        check_int(4, sublist->Get(2), "Get(2)", "index 2", "check GetSubList");
        delete sublist;
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_GetLength_LinkedList()
{
    print_test_header("check_GetLength_LinkedList");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: LinkedList<int> list(mas, 3)" << endl;
    cout << "  Check: list.GetLength()" << endl;
    try {
        LinkedList<int> list(mas, 3);
        check_size_t(3, list.GetLength(), "GetLength()", "array {1,2,3}", "check GetLength list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_Append_LinkedList()
{
    print_test_header("check_Append_LinkedList");
    cout << "  Input: 1, 2, 3 sequentially" << endl;
    cout << "  Call: list.Append(1); list.Append(2); list.Append(3)" << endl;
    cout << "  Expected: list {1, 2, 3}" << endl;
    try {
        LinkedList<int> list;
        list.Append(1);
        list.Append(2);
        list.Append(3);
        cout << "  Result: items added" << endl;
        cout << "  Check: list.GetLength()" << endl;
        check_size_t(3, list.GetLength(), "GetLength()", "added 1,2,3", "check Append list");
        cout << "  Check: list.Get(0)" << endl;
        check_int(1, list.Get(0), "Get(0)", "index 0", "check Append list");
        cout << "  Check: list.Get(1)" << endl;
        check_int(2, list.Get(1), "Get(1)", "index 1", "check Append list");
        cout << "  Check: list.Get(2)" << endl;
        check_int(3, list.Get(2), "Get(2)", "index 2", "check Append list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_Prepend_LinkedList()
{
    print_test_header("check_Prepend_LinkedList");
    cout << "  Input: 3, 2, 1 to front" << endl;
    cout << "  Call: list.Prepend(3); list.Prepend(2); list.Prepend(1)" << endl;
    cout << "  Expected: list {1, 2, 3}" << endl;
    try {
        LinkedList<int> list;
        list.Prepend(3);
        list.Prepend(2);
        list.Prepend(1);
        cout << "  Result: items added" << endl;
        cout << "  Check: list.GetLength()" << endl;
        check_size_t(3, list.GetLength(), "GetLength()", "added 3,2,1 to front", "check Prepend list");
        cout << "  Check: list.Get(0)" << endl;
        check_int(1, list.Get(0), "Get(0)", "index 0", "check Prepend list");
        cout << "  Check: list.Get(1)" << endl;
        check_int(2, list.Get(1), "Get(1)", "index 1", "check Prepend list");
        cout << "  Check: list.Get(2)" << endl;
        check_int(3, list.Get(2), "Get(2)", "index 2", "check Prepend list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_InsertAt_LinkedList()
{
    print_test_header("check_InsertAt_LinkedList");
    cout << "  Input: array {1, 3}, insert 2 at index 1" << endl;
    cout << "  Call: list.InsertAt(2, 1)" << endl;
    cout << "  Expected: list {1, 2, 3}" << endl;
    try {
        int mas[2] = {1, 3};
        LinkedList<int> list(mas, 2);
        list.InsertAt(2, 1);
        cout << "  Result: item inserted" << endl;
        cout << "  Check: list.GetLength()" << endl;
        check_size_t(3, list.GetLength(), "GetLength()", "insert 2 at index 1", "check InsertAt list");
        cout << "  Check: list.Get(0)" << endl;
        check_int(1, list.Get(0), "Get(0)", "index 0", "check InsertAt list");
        cout << "  Check: list.Get(1)" << endl;
        check_int(2, list.Get(1), "Get(1)", "index 1 (inserted 2)", "check InsertAt list");
        cout << "  Check: list.Get(2)" << endl;
        check_int(3, list.Get(2), "Get(2)", "index 2", "check InsertAt list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_Concat_LinkedList()
{
    print_test_header("check_Concat_LinkedList");
    int mas1[2] = {1, 2};
    int mas2[2] = {3, 4};
    cout << "  Input: list1 = {1, 2}, list2 = {3, 4}" << endl;
    cout << "  Call: list1.Concat(&list2)" << endl;
    cout << "  Expected: {1, 2, 3, 4}" << endl;
    try {
        LinkedList<int> list1(mas1, 2);
        LinkedList<int> list2(mas2, 2);
        LinkedList<int>* result = list1.Concat(&list2);
        cout << "  Result: list concatenated" << endl;
        cout << "  Check: result->GetLength()" << endl;
        check_size_t(4, result->GetLength(), "GetLength()", "concatenation", "check Concat");
        cout << "  Check: result->Get(0)" << endl;
        check_int(1, result->Get(0), "Get(0)", "index 0", "check Concat");
        cout << "  Check: result->Get(1)" << endl;
        check_int(2, result->Get(1), "Get(1)", "index 1", "check Concat");
        cout << "  Check: result->Get(2)" << endl;
        check_int(3, result->Get(2), "Get(2)", "index 2", "check Concat");
        cout << "  Check: result->Get(3)" << endl;
        check_int(4, result->Get(3), "Get(3)", "index 3", "check Concat");
        delete result;
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 5;
    }
}

void check_ClearList()
{
    print_test_header("check_ClearList");
    int mas[3] = {1, 2, 3};
    cout << "  Input: array {1, 2, 3}" << endl;
    cout << "  Call: list.ClearList()" << endl;
    cout << "  Expected: size 0" << endl;
    try {
        LinkedList<int> list(mas, 3);
        list.ClearList();
        cout << "  Result: list cleared" << endl;
        cout << "  Check: list.GetLength()" << endl;
        check_size_t(0, list.GetLength(), "GetLength()", "after clear", "check ClearList");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_BitSequence()
{
    print_test_header("check_BitSequence");
    cout << "  Input: no parameters" << endl;
    cout << "  Call: BitSequence bits;" << endl;
    try {
        BitSequence bits;
        cout << "  Result: object created" << endl;
        cout << "  Check: bits.GetLength()" << endl;
        check_size_t(0, bits.GetLength(), "GetLength()", "no parameters", "empty constructor BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_BitSequence_user()
{
    print_test_header("check_BitSequence_user");
    int mas[3] = {1, 0, 1};
    cout << "  Input: array {1, 0, 1}, size 3" << endl;
    cout << "  Call: BitSequence bits(mas, 3)" << endl;
    try {
        BitSequence bits(mas, 3);
        cout << "  Result: object created" << endl;
        cout << "  Check: bits.GetLength()" << endl;
        check_size_t(3, bits.GetLength(), "GetLength()", "array {1,0,1}", "constructor BitSequence from array");
        cout << "  Check: bits.Get(0)" << endl;
        check_int(1, bits.Get(0), "Get(0)", "index 0", "constructor BitSequence from array");
        cout << "  Check: bits.Get(1)" << endl;
        check_int(0, bits.Get(1), "Get(1)", "index 1", "constructor BitSequence from array");
        cout << "  Check: bits.Get(2)" << endl;
        check_int(1, bits.Get(2), "Get(2)", "index 2", "constructor BitSequence from array");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_BitSequence_copy()
{
    print_test_header("check_BitSequence_copy");
    int mas[3] = {1, 0, 1};
    cout << "  Input: array {1, 0, 1}" << endl;
    cout << "  Call: BitSequence bits1(mas, 3)" << endl;
    cout << "  Call: BitSequence bits2(bits1)" << endl;
    try {
        BitSequence bits1(mas, 3);
        BitSequence bits2(bits1);
        cout << "  Result: bits2 created" << endl;
        cout << "  Check: bits2.GetLength()" << endl;
        check_size_t(3, bits2.GetLength(), "GetLength()", "copy of bits1", "copy constructor BitSequence");
        cout << "  Check: bits2.Get(0)" << endl;
        check_int(1, bits2.Get(0), "Get(0)", "index 0", "copy constructor BitSequence");
        cout << "  Check: bits2.Get(1)" << endl;
        check_int(0, bits2.Get(1), "Get(1)", "index 1", "copy constructor BitSequence");
        cout << "  Check: bits2.Get(2)" << endl;
        check_int(1, bits2.Get(2), "Get(2)", "index 2", "copy constructor BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_GetFirst_BitSequence()
{
    print_test_header("check_GetFirst_BitSequence");
    int mas[3] = {1, 0, 1};
    cout << "  Input: array {1, 0, 1}" << endl;
    cout << "  Call: BitSequence bits(mas, 3)" << endl;
    cout << "  Check: bits.GetFirst()" << endl;
    try {
        BitSequence bits(mas, 3);
        check_int(1, bits.GetFirst(), "GetFirst()", "array {1,0,1}", "check GetFirst BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_GetLast_BitSequence()
{
    print_test_header("check_GetLast_BitSequence");
    int mas[3] = {1, 0, 1};
    cout << "  Input: array {1, 0, 1}" << endl;
    cout << "  Call: BitSequence bits(mas, 3)" << endl;
    cout << "  Check: bits.GetLast()" << endl;
    try {
        BitSequence bits(mas, 3);
        check_int(1, bits.GetLast(), "GetLast()", "array {1,0,1}", "check GetLast BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_Get_BitSequence()
{
    print_test_header("check_Get_BitSequence");
    int mas[3] = {1, 0, 1};
    cout << "  Input: array {1, 0, 1}" << endl;
    cout << "  Call: BitSequence bits(mas, 3)" << endl;
    cout << "  Check: bits.Get(1)" << endl;
    try {
        BitSequence bits(mas, 3);
        check_int(0, bits.Get(1), "Get(1)", "index 1", "check Get BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_GetSubsequence_BitSequence()
{
    print_test_header("check_GetSubsequence_BitSequence");
    int mas[5] = {1, 0, 1, 0, 1};
    cout << "  Input: array {1, 0, 1, 0, 1}" << endl;
    cout << "  Call: BitSequence bits(mas, 5)" << endl;
    cout << "  Call: bits.GetSubsequence(1, 3)" << endl;
    cout << "  Expected: {0, 1, 0}" << endl;
    try {
        BitSequence bits(mas, 5);
        BitSequence* sub = bits.GetSubsequence(1, 3);
        cout << "  Result: subsequence created" << endl;
        cout << "  Check: sub->GetLength()" << endl;
        check_size_t(3, sub->GetLength(), "GetLength()", "subsequence", "check GetSubsequence BitSequence");
        cout << "  Check: sub->Get(0)" << endl;
        check_int(0, sub->Get(0), "Get(0)", "index 0", "check GetSubsequence BitSequence");
        cout << "  Check: sub->Get(1)" << endl;
        check_int(1, sub->Get(1), "Get(1)", "index 1", "check GetSubsequence BitSequence");
        cout << "  Check: sub->Get(2)" << endl;
        check_int(0, sub->Get(2), "Get(2)", "index 2", "check GetSubsequence BitSequence");
        delete sub;
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_GetLength_BitSequence()
{
    print_test_header("check_GetLength_BitSequence");
    int mas[3] = {1, 0, 1};
    cout << "  Input: array {1, 0, 1}" << endl;
    cout << "  Call: BitSequence bits(mas, 3)" << endl;
    cout << "  Check: bits.GetLength()" << endl;
    try {
        BitSequence bits(mas, 3);
        check_size_t(3, bits.GetLength(), "GetLength()", "array {1,0,1}", "check GetLength BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests++;
    }
}

void check_AppendImpl_BitSequence()
{
    print_test_header("check_AppendImpl_BitSequence");
    cout << "  Input: 1, 0, 1 sequentially" << endl;
    cout << "  Call: bits.Append(1); bits.Append(0); bits.Append(1)" << endl;
    cout << "  Expected: {1, 0, 1}" << endl;
    try {
        BitSequence bits;
        bits.Append(1);
        bits.Append(0);
        bits.Append(1);
        cout << "  Result: items added" << endl;
        cout << "  Check: bits.GetLength()" << endl;
        check_size_t(3, bits.GetLength(), "GetLength()", "added 1,0,1", "check Append BitSequence");
        cout << "  Check: bits.Get(0)" << endl;
        check_int(1, bits.Get(0), "Get(0)", "index 0", "check Append BitSequence");
        cout << "  Check: bits.Get(1)" << endl;
        check_int(0, bits.Get(1), "Get(1)", "index 1", "check Append BitSequence");
        cout << "  Check: bits.Get(2)" << endl;
        check_int(1, bits.Get(2), "Get(2)", "index 2", "check Append BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_InsertAtImpl_BitSequence()
{
    print_test_header("check_InsertAtImpl_BitSequence");
    cout << "  Input: array {1, 1}, insert 0 at index 1" << endl;
    cout << "  Call: bits.InsertAt(0, 1)" << endl;
    cout << "  Expected: {1, 0, 1}" << endl;
    try {
        int mas[2] = {1, 1};
        BitSequence bits(mas, 2);
        bits.InsertAt(0, 1);
        cout << "  Result: item inserted" << endl;
        cout << "  Check: bits.GetLength()" << endl;
        check_size_t(3, bits.GetLength(), "GetLength()", "insert 0 at index 1", "check InsertAt BitSequence");
        cout << "  Check: bits.Get(0)" << endl;
        check_int(1, bits.Get(0), "Get(0)", "index 0", "check InsertAt BitSequence");
        cout << "  Check: bits.Get(1)" << endl;
        check_int(0, bits.Get(1), "Get(1)", "index 1 (inserted 0)", "check InsertAt BitSequence");
        cout << "  Check: bits.Get(2)" << endl;
        check_int(1, bits.Get(2), "Get(2)", "index 2", "check InsertAt BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_PrependImpl_BitSequence()
{
    print_test_header("check_PrependImpl_BitSequence");
    cout << "  Input: 1, 0, 1 to front" << endl;
    cout << "  Call: bits.Prepend(1); bits.Prepend(0); bits.Prepend(1)" << endl;
    cout << "  Expected: {1, 0, 1}" << endl;
    try {
        BitSequence bits;
        bits.Prepend(1);
        bits.Prepend(0);
        bits.Prepend(1);
        cout << "  Result: items added" << endl;
        cout << "  Check: bits.GetLength()" << endl;
        check_size_t(3, bits.GetLength(), "GetLength()", "added 1,0,1 to front", "check Prepend BitSequence");
        cout << "  Check: bits.Get(0)" << endl;
        check_int(1, bits.Get(0), "Get(0)", "index 0", "check Prepend BitSequence");
        cout << "  Check: bits.Get(1)" << endl;
        check_int(0, bits.Get(1), "Get(1)", "index 1", "check Prepend BitSequence");
        cout << "  Check: bits.Get(2)" << endl;
        check_int(1, bits.Get(2), "Get(2)", "index 2", "check Prepend BitSequence");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_Concat_BitSequence()
{
    print_test_header("check_Concat_BitSequence");
    int mas1[2] = {1, 0};
    int mas2[2] = {1, 1};
    cout << "  Input: bits1 = {1, 0}, bits2 = {1, 1}" << endl;
    cout << "  Call: bits1.Concat(&bits2)" << endl;
    cout << "  Expected: {1, 0, 1, 1}" << endl;
    try {
        BitSequence bits1(mas1, 2);
        BitSequence bits2(mas2, 2);
        Sequence<int>* result = bits1.Concat(&bits2);
        cout << "  Result: sequences concatenated" << endl;
        cout << "  Check: result->GetLength()" << endl;
        check_size_t(4, result->GetLength(), "GetLength()", "concatenation", "check Concat BitSequence");
        cout << "  Check: result->Get(0)" << endl;
        check_int(1, result->Get(0), "Get(0)", "index 0", "check Concat BitSequence");
        cout << "  Check: result->Get(1)" << endl;
        check_int(0, result->Get(1), "Get(1)", "index 1", "check Concat BitSequence");
        cout << "  Check: result->Get(2)" << endl;
        check_int(1, result->Get(2), "Get(2)", "index 2", "check Concat BitSequence");
        cout << "  Check: result->Get(3)" << endl;
        check_int(1, result->Get(3), "Get(3)", "index 3", "check Concat BitSequence");
        delete result;
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 5;
    }
}

void check_BitAnd()
{
    print_test_header("check_BitAnd");
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    cout << "  Input: bits1 = {1, 0, 1}, bits2 = {1, 1, 0}" << endl;
    cout << "  Call: bits1.BitAnd(&bits2)" << endl;
    cout << "  Expected: {1, 0, 0}" << endl;
    try {
        BitSequence bits1(mas1, 3);
        BitSequence bits2(mas2, 3);
        BitSequence* result = bits1.BitAnd(&bits2);
        cout << "  Result: operation complete" << endl;
        cout << "  Check: result->GetLength()" << endl;
        check_size_t(3, result->GetLength(), "GetLength()", "BitAnd", "check BitAnd");
        cout << "  Check: result->Get(0)" << endl;
        check_int(1, result->Get(0), "Get(0)", "1 & 1 = 1", "check BitAnd");
        cout << "  Check: result->Get(1)" << endl;
        check_int(0, result->Get(1), "Get(1)", "0 & 1 = 0", "check BitAnd");
        cout << "  Check: result->Get(2)" << endl;
        check_int(0, result->Get(2), "Get(2)", "1 & 0 = 0", "check BitAnd");
        delete result;
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_BitOr()
{
    print_test_header("check_BitOr");
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    cout << "  Input: bits1 = {1, 0, 1}, bits2 = {1, 1, 0}" << endl;
    cout << "  Call: bits1.BitOr(&bits2)" << endl;
    cout << "  Expected: {1, 1, 1}" << endl;
    try {
        BitSequence bits1(mas1, 3);
        BitSequence bits2(mas2, 3);
        BitSequence* result = bits1.BitOr(&bits2);
        cout << "  Result: operation complete" << endl;
        cout << "  Check: result->GetLength()" << endl;
        check_size_t(3, result->GetLength(), "GetLength()", "BitOr", "check BitOr");
        cout << "  Check: result->Get(0)" << endl;
        check_int(1, result->Get(0), "Get(0)", "1 | 1 = 1", "check BitOr");
        cout << "  Check: result->Get(1)" << endl;
        check_int(1, result->Get(1), "Get(1)", "0 | 1 = 1", "check BitOr");
        cout << "  Check: result->Get(2)" << endl;
        check_int(1, result->Get(2), "Get(2)", "1 | 0 = 1", "check BitOr");
        delete result;
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_BitXor()
{
    print_test_header("check_BitXor");
    int mas1[3] = {1, 0, 1};
    int mas2[3] = {1, 1, 0};
    cout << "  Input: bits1 = {1, 0, 1}, bits2 = {1, 1, 0}" << endl;
    cout << "  Call: bits1.BitXor(&bits2)" << endl;
    cout << "  Expected: {0, 1, 1}" << endl;
    try {
        BitSequence bits1(mas1, 3);
        BitSequence bits2(mas2, 3);
        BitSequence* result = bits1.BitXor(&bits2);
        cout << "  Result: operation complete" << endl;
        cout << "  Check: result->GetLength()" << endl;
        check_size_t(3, result->GetLength(), "GetLength()", "BitXor", "check BitXor");
        cout << "  Check: result->Get(0)" << endl;
        check_int(0, result->Get(0), "Get(0)", "1 ^ 1 = 0", "check BitXor");
        cout << "  Check: result->Get(1)" << endl;
        check_int(1, result->Get(1), "Get(1)", "0 ^ 1 = 1", "check BitXor");
        cout << "  Check: result->Get(2)" << endl;
        check_int(1, result->Get(2), "Get(2)", "1 ^ 0 = 1", "check BitXor");
        delete result;
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_BitNot()
{
    print_test_header("check_BitNot");
    int mas[3] = {1, 0, 1};
    cout << "  Input: bits = {1, 0, 1}" << endl;
    cout << "  Call: bits.BitNot()" << endl;
    cout << "  Expected: {0, 1, 0}" << endl;
    try {
        BitSequence bits(mas, 3);
        BitSequence* result = bits.BitNot();
        cout << "  Result: operation complete" << endl;
        cout << "  Check: result->GetLength()" << endl;
        check_size_t(3, result->GetLength(), "GetLength()", "BitNot", "check BitNot");
        cout << "  Check: result->Get(0)" << endl;
        check_int(0, result->Get(0), "Get(0)", "~1 = 0", "check BitNot");
        cout << "  Check: result->Get(1)" << endl;
        check_int(1, result->Get(1), "Get(1)", "~0 = 1", "check BitNot");
        cout << "  Check: result->Get(2)" << endl;
        check_int(0, result->Get(2), "Get(2)", "~1 = 0", "check BitNot");
        delete result;
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 4;
    }
}

void check_initializer_list_dynamic_array()
{
    print_test_header("check_initializer_list_dynamic_array");
    cout << "  Input: {1, 2, 3, 4, 5}" << endl;
    cout << "  Call: DynamicArray<int> arr = {1, 2, 3, 4, 5}" << endl;
    try {
        DynamicArray<int> arr = {1, 2, 3, 4, 5};
        cout << "  Result: object created" << endl;
        cout << "  Check: arr.GetSize()" << endl;
        check_size_t(5, arr.GetSize(), "GetSize()", "{1,2,3,4,5}", "constructor from initializer_list");
        cout << "  Check: arr.Get(0)" << endl;
        check_int(1, arr.Get(0), "Get(0)", "index 0", "constructor from initializer_list");
        cout << "  Check: arr.Get(1)" << endl;
        check_int(2, arr.Get(1), "Get(1)", "index 1", "constructor from initializer_list");
        cout << "  Check: arr.Get(2)" << endl;
        check_int(3, arr.Get(2), "Get(2)", "index 2", "constructor from initializer_list");
        cout << "  Check: arr.Get(3)" << endl;
        check_int(4, arr.Get(3), "Get(3)", "index 3", "constructor from initializer_list");
        cout << "  Check: arr.Get(4)" << endl;
        check_int(5, arr.Get(4), "Get(4)", "index 4", "constructor from initializer_list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 6;
    }
}

void check_initializer_list_array_sequence()
{
    print_test_header("check_initializer_list_array_sequence");
    cout << "  Input: {1, 2, 3, 4, 5}" << endl;
    cout << "  Call: ArraySequence<int> seq = {1, 2, 3, 4, 5}" << endl;
    try {
        ArraySequence<int> seq = {1, 2, 3, 4, 5};
        cout << "  Result: object created" << endl;
        cout << "  Check: seq.GetLength()" << endl;
        check_size_t(5, seq.GetLength(), "GetLength()", "{1,2,3,4,5}", "constructor ArraySequence from initializer_list");
        cout << "  Check: seq.GetFirst()" << endl;
        check_int(1, seq.GetFirst(), "GetFirst()", "first element", "constructor ArraySequence from initializer_list");
        cout << "  Check: seq.GetLast()" << endl;
        check_int(5, seq.GetLast(), "GetLast()", "last element", "constructor ArraySequence from initializer_list");
        cout << "  Check: seq.Get(0)" << endl;
        check_int(1, seq.Get(0), "Get(0)", "index 0", "constructor ArraySequence from initializer_list");
        cout << "  Check: seq.Get(2)" << endl;
        check_int(3, seq.Get(2), "Get(2)", "index 2", "constructor ArraySequence from initializer_list");
        cout << "  Check: seq.Get(4)" << endl;
        check_int(5, seq.Get(4), "Get(4)", "index 4", "constructor ArraySequence from initializer_list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 6;
    }
}

void check_initializer_list_list_sequence()
{
    print_test_header("check_initializer_list_list_sequence");
    cout << "  Input: {100, 200, 300, 400, 500}" << endl;
    cout << "  Call: ListSequence<int> seq = {100, 200, 300, 400, 500}" << endl;
    try {
        ListSequence<int> seq = {100, 200, 300, 400, 500};
        cout << "  Result: object created" << endl;
        cout << "  Check: seq.GetLength()" << endl;
        check_size_t(5, seq.GetLength(), "GetLength()", "{100,200,300,400,500}", "constructor ListSequence from initializer_list");
        cout << "  Check: seq.GetFirst()" << endl;
        check_int(100, seq.GetFirst(), "GetFirst()", "first element", "constructor ListSequence from initializer_list");
        cout << "  Check: seq.GetLast()" << endl;
        check_int(500, seq.GetLast(), "GetLast()", "last element", "constructor ListSequence from initializer_list");
        cout << "  Check: seq.Get(0)" << endl;
        check_int(100, seq.Get(0), "Get(0)", "index 0", "constructor ListSequence from initializer_list");
        cout << "  Check: seq.Get(2)" << endl;
        check_int(300, seq.Get(2), "Get(2)", "index 2", "constructor ListSequence from initializer_list");
        cout << "  Check: seq.Get(4)" << endl;
        check_int(500, seq.Get(4), "Get(4)", "index 4", "constructor ListSequence from initializer_list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 6;
    }
}

void check_initializer_list_bit_sequence()
{
    print_test_header("check_initializer_list_bit_sequence");
    cout << "  Input: {1, 0, 1, 0, 1}" << endl;
    cout << "  Call: BitSequence seq = {1, 0, 1, 0, 1}" << endl;
    try {
        BitSequence seq = {1, 0, 1, 0, 1};
        cout << "  Result: object created" << endl;
        cout << "  Check: seq.GetLength()" << endl;
        check_size_t(5, seq.GetLength(), "GetLength()", "{1,0,1,0,1}", "constructor BitSequence from initializer_list");
        cout << "  Check: seq.GetFirst()" << endl;
        check_int(1, seq.GetFirst(), "GetFirst()", "first element", "constructor BitSequence from initializer_list");
        cout << "  Check: seq.GetLast()" << endl;
        check_int(1, seq.GetLast(), "GetLast()", "last element", "constructor BitSequence from initializer_list");
        cout << "  Check: seq.Get(0)" << endl;
        check_int(1, seq.Get(0), "Get(0)", "index 0", "constructor BitSequence from initializer_list");
        cout << "  Check: seq.Get(1)" << endl;
        check_int(0, seq.Get(1), "Get(1)", "index 1", "constructor BitSequence from initializer_list");
        cout << "  Check: seq.Get(2)" << endl;
        check_int(1, seq.Get(2), "Get(2)", "index 2", "constructor BitSequence from initializer_list");
        cout << "  Check: seq.Get(3)" << endl;
        check_int(0, seq.Get(3), "Get(3)", "index 3", "constructor BitSequence from initializer_list");
        cout << "  Check: seq.Get(4)" << endl;
        check_int(1, seq.Get(4), "Get(4)", "index 4", "constructor BitSequence from initializer_list");
    } catch (const std::exception& e) {
        cout << "  [EXCEPTION] " << e.what() << endl;
        totalTests += 7;
    }
}

void check_initializer_list_bit_sequence_invalid()
{
    print_test_header("check_initializer_list_bit_sequence_invalid");
    cout << "  Input: {1, 0, 2, 0, 1} (2 is invalid bit)" << endl;
    cout << "  Call: BitSequence seq = {1, 0, 2, 0, 1}" << endl;
    cout << "  Expected: InvalidBitException" << endl;
    bool exception_caught = false;
    try {
        BitSequence seq = {1, 0, 2, 0, 1};
        cout << "  [FAIL] Exception not thrown!" << endl;
        totalTests++;
    } catch (const InvalidBitException& e) {
        exception_caught = true;
        cout << "  [PASS] Caught InvalidBitException" << endl;
        cout << "  Message: " << e.what() << endl;
        totalTests++;
        passedTests++;
    } catch (const std::exception& e) {
        cout << "  [FAIL] Caught different exception: " << e.what() << endl;
        totalTests++;
    }
    (void)exception_caught;
}

#endif