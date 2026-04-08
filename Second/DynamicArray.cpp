#include <iostream>
using namespace std;
#include <string>
#include "DynamicArray.h"
#include "LinkedList.h"


DynamicArray<T> :: DynamicArray ()
{
    items = NULL;
    size = 0;
}

DynamicArray<T> :: DynamicArray (T* items, int size)
{
    this->items = new T [size];
    this->size = size;

    for (int i=0; i<size; i++)
        this->items[i] = items[i];
}

DynamicArray<T> :: DynamicArray (DynamicArray <T>& rhs const)
{
    this->items = new T [rhs.size];
    this->size = rhs.size;

    for (int i = 0; i < rhs.size; i++)
        this->items[i] = rhs.items[i];
}

DynamicArray<T> :: DynamicArray (LinkedList <T>& list const)
{
    if (length == 0)
    {
        items = nullptr;
        size = 0;
    }
    else 
    {
        size = list.GetLength();
        items = new T[size];

        for (int i = 0; i < size; i++)
            items[i] = list.Get(i);
    }
}

T DynamicArray<T> :: Get(int index)
{
    if (index > size-1 || index < 0)
    {
        // исключение бросаем через throw
    }
    else
        return items[index];
}

int DynamicArray<T> :: GetSize()
{
    return size;
}

void DynamicArray<T> :: Set (int index, T value)
{
    if (index > size-1 || index < 0)
    {
        // исключение бросаем через throw
    }
        else
        items[index] = value;
}

void DynamicArray<T> :: Resize(int newsize)
{
    T* arr = new T [newsize];

    if (newsize > size)
        for (int i=0; i < size; i++)
            arr[i] = items[i];
    else
        for (int i=0; i < newsize; i++)
            arr[i] = items[i];

    delete [] items;
    size = newsize;
    items = arr;
}

DynamicArray<T> :: ~DynamicArray ()
{
    if (items)
        delete [] items;
}

