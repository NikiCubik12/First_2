#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include <iostream>
using namespace std;
#include <string>
#include "LinkedList.h"

template <class T> class DynamicArray
{
    private:
    T* items;
    int size;

    DynamicArray();
    DynamicArray (T* items, int size);
    DynamicArray (DynamicArray <T>& rhs const);
    DynamicArray (LinkedList <T>& list const);
    Get(int index)
    int GetSize()
    void Set (int index, T value);
    void Resize(int newsize);
    ~DynamicArray();
}

#endif