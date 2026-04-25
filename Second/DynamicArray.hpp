#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include <iostream>
using namespace std;
#include <string>
#include "LinkedList.hpp"

template <class T> class LinkedList;

template <class T> class DynamicArray
{
    private:
    T* items;
    int size;

    public:
    DynamicArray()
    {
        items = NULL;
        size = 0;
    }

    DynamicArray(T* items, int size)
    {
        this->items = new T[size];
        this->size = size;

        for (int i = 0; i < size; i++)
            this->items[i] = items[i];
    }

    DynamicArray(const DynamicArray<T>& rhs)
    {
        this->items = new T[rhs.size];
        this->size = rhs.size;

        for (int i = 0; i < rhs.size; i++)
            this->items[i] = rhs.items[i];
    }

    DynamicArray(LinkedList<T>& list)
    {
        if (list.GetLength() == 0)  // Исправлено: было length, нужно list.GetLength()
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


    void Append (const T& item)
    {
        T* arr = new T[size + 1];
        for (int i = 0; i < size; i++)
            arr[i] = Get(i);
        arr[size] = item;

        delete [] items;
        size++;

        items = arr;
    }

    void InsertAt(const T& item, int index)
    {
        if ((index < 0) || (index > size-1))
        {
            throw out_of_range("Index out of range in ArraySequence::InsertAtImpl");
        }
            
        else 
        {
            T* arr = new T[size + 1];
            for (int i = 0; i < index; i++)
                arr[i] = Get(i);

            arr[index] = item;

            for (int i = index; i < size; i++)
                arr[i + 1] = Get(i);

            delete [] items;
            items = arr;
            size++;
        }
    }

    void Prepend(const T& item)
    {
        T* arr = new T[size+1];

        arr[0] = item;
        
        for (int i = 1; i < size + 1; i++)
            arr[i] = Get(i-1);
        
        delete [] items;
        size++;
        items = arr;
    }


    T Get(int index)
    {
        if (index > size-1 || index < 0)
        {
            throw std::out_of_range("Index out of range in DynamicArray::Get");
        }
        else
        {
            return items[index];
        }
    }

    int GetSize()
    {
        return size;
    }

    void Set(int index, T value)
    {
        if (index > size-1 || index < 0)
        {
            throw std::out_of_range("Index out of range in DynamicArray::Set");
        }
        else
            items[index] = value;
    }

    void Resize(int newsize)
    {
        T* arr = new T[newsize];

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

    ~DynamicArray()
    {
        if (items)
            delete [] items;
    }
};



#endif