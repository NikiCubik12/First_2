#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"

template <class T> class ArraySequence : public Sequence<T>
{
    protected:
    DynamicArray<T>* data;
    
    Sequence<T>* instance() override;
    Sequence<T>* AppendImpl(const T& item) override;
    Sequence<T>* InsertAtImpl(const T& item, size_t index) override;
    Sequence<T>* PrependImpl(const T& item) override;

    public:
    ArraySequence();
    ArraySequence(T* items, size_t count);
    ArraySequence(const DynamicArray<T>& array);
    ArraySequence(const LinkedList<T>& list);
    ArraySequence(const ArraySequence<T>& other);
    ArraySequence<T>& operator=(const ArraySequence<T>& other);
    ArraySequence(std::initializer_list<T> list);
    ~ArraySequence();

    T GetFirst() override;
    T GetLast() override;
    T Get(size_t index) override;
    Sequence<T>* GetSubsequence(size_t start, size_t end) override;
    size_t GetLength() override;
    Sequence<T>* Concat(Sequence<T>* list) override;

    T& GetRef(size_t index);
    const T& GetRef(size_t index) const;
};

#include "ArraySequence.tpp"

#endif