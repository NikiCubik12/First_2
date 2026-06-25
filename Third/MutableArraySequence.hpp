#ifndef MUTABLE_ARRAY_SEQUENCE_HPP
#define MUTABLE_ARRAY_SEQUENCE_HPP

#include "ArraySequence.hpp"

template <class T> class MutableArraySequence : public ArraySequence<T> 
{
    public:
    MutableArraySequence();
    MutableArraySequence(T* items, size_t count);
    MutableArraySequence(const DynamicArray<T>& list);
    MutableArraySequence(std::initializer_list<T> list);
    
    void Set(size_t index, T value);
};

#include "MutableArraySequence.tpp"

#endif