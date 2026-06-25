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
    DynamicArray(const DynamicArray<T>& rhs);
    DynamicArray(LinkedList<T>& list);
    DynamicArray(std::initializer_list<T> list);
    ~DynamicArray();

    void Append(const T& item);
    void InsertAt(const T& item, size_t index);
    void Prepend(const T& item);
    T Get(size_t index) const;
    size_t GetSize() const;
    void Set(size_t index, T value);
    void Resize(size_t newSize);
    T* GetData();
    const T* GetData() const;
};

#include "DynamicArray.tpp"

#endif