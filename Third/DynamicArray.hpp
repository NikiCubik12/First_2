#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <iostream>
#include <string>
#include <initializer_list>
#include <cstddef>
#include "Sequence.hpp"

template <class T> class LinkedList;

template <class T> class DynamicArray
{
    private:
    T* items;
    size_t size;

    public:
    DynamicArray();
    DynamicArray(T* items, size_t size);
    DynamicArray(DynamicArray<T>& rhs);
    DynamicArray(LinkedList<T>& list);
    DynamicArray(std::initializer_list<T> list);
    ~DynamicArray();

    void Append(T item);
    void InsertAt(T item, size_t index);
    void Prepend(T item);
    void RemoveAt(size_t index);
    T Get(size_t index) const;
    size_t GetSize();
    void Set(size_t index, T value);
    void Resize(size_t newSize);
};

#include "DynamicArray.tpp"

#endif