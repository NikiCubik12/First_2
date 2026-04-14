#include <iostream>
using namespace std;
using std::cin, std::cout, std::out_of_range, std::invalid_argument
#include <string>
#include "DynamicArray.hpp"
#include "LinkedList.hpp"

template <typename T> 
DynamicArray<T> :: DynamicArray ()
{
    items = NULL;
    size = 0;
}

template <typename T> 
DynamicArray<T> :: DynamicArray (T* items, int size)
{
    this->items = new T [size];
    this->size = size;

    for (int i = 0; i < size; i++)
        this->items[i] = items[i];
}

template <typename T> 
DynamicArray<T> :: DynamicArray (DynamicArray <T>& rhs const)
{
    this->items = new T [rhs.size];
    this->size = rhs.size;

    for (int i = 0; i < rhs.size; i++)
        this->items[i] = rhs.items[i];
}

template <typename T> 
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

template <typename T> 
T DynamicArray<T> :: Get(int index)
{
    if (index > size-1 || index < 0)
        throw invalid_argument("Индекс выходит за границы последовательности");
    else
        return items[index];
}

template <typename T> 
int DynamicArray<T> :: GetSize()
{
    return size;
}

template <typename T> 
void DynamicArray<T> :: Set (int index, T value)
{
    if (index > size-1 || index < 0)
        throw invalid_argument("Индекс выходит за границы последовательности");
    else
        items[index] = value;
}

template <typename T> 
void DynamicArray<T> :: Resize(int newsize)
{
    T* arr = new T [newsize];

    if (newsize > size)
        for (int i = 0; i < size; i++)
            arr[i] = items[i];
    else
        for (int i = 0; i < newsize; i++)
            arr[i] = items[i];

    delete [] items;
    size = newsize;
    items = arr;
}

template <typename T> 
DynamicArray<T> :: ~DynamicArray ()
{
    if (items)
        delete [] items;
}