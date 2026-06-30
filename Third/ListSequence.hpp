#ifndef LIST_SEQUENCE_HPP
#define LIST_SEQUENCE_HPP

#include "Sequence.hpp"
#include "LinkedList.hpp"

template <class T> class ListSequence : public Sequence<T>
{
    private:
    LinkedList<T>* items;

    protected:
    Sequence<T>* instance() override;
    Sequence<T>* AppendImpl(T item) override;
    Sequence<T>* InsertAtImpl(T item, size_t index) override;
    Sequence<T>* PrependImpl(T item) override;

    public:
    ListSequence();
    ListSequence(T* arr, size_t count);
    ListSequence(LinkedList<T> list);
    ListSequence(const ListSequence<T>& other);
    ListSequence(std::initializer_list<T> list);
    ~ListSequence();

    T GetFirst() override;
    T GetLast() override;
    T Get(size_t index) override;
    Sequence<T>* GetSubsequence(size_t start, size_t end) override;
    size_t GetLength() override;
    Sequence<T>* Concat(Sequence<T>* list) override;
};

#include "ListSequence.tpp"

#endif