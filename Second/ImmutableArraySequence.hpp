#ifndef IMMUTABLE_ARRAY_SEQUENCE_HPP
#define IMMUTABLE_ARRAY_SEQUENCE_HPP

#include "ArraySequence.hpp"

template <class T> class ImmutableArraySequence : public ArraySequence<T> 
{
    protected:
    Sequence<T>* instance() override
    {
        return new ImmutableArraySequence<T>(*this);
    }
    
    public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence(const DynamicArray<T>& list) : ArraySequence<T>(list) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(*(other.data)) {}
};

#endif 