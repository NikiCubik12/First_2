#ifndef MUTABLE_ARRAY_SEQUENCE_HPP
#define MUTABLE_ARRAY_SEQUENCE_HPP

#include "ArraySequence.hpp"

template <class T> class MutableArraySequence : public ArraySequence<T> 
{
    protected:
    Sequence<T>* instance()
    {
        return this;
    }
    public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    MutableArraySequence(const DynamicArray<T>& list) : ArraySequence<T>(list) {}
};

#endif