#ifndef IMMUTABLE_ARRAY_SEQUENCE_HPP
#define IMMUTABLE_ARRAY_SEQUENCE_HPP

#include "ArraySequence.hpp"

template <class T> class ImmutableArraySequence : public ArraySequence<T> 
{
    protected:
    Sequence<T>* instance() override;
    Sequence<T>* AppendImpl(T item) override;
    Sequence<T>* InsertAtImpl(T item, size_t index) override;
    Sequence<T>* PrependImpl(T item) override;

    public:
    ImmutableArraySequence();
    ImmutableArraySequence(T* items, size_t count);
    ImmutableArraySequence(DynamicArray<T> list);
    ImmutableArraySequence(const ImmutableArraySequence<T>& other);
    ImmutableArraySequence(std::initializer_list<T> list);
};

#include "ImmutableArraySequence.tpp"

#endif