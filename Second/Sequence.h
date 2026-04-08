#ifndef SEQUENCE
#define SEQUENCE

#include <iostream>
#include <string>
using namespace std;

template <class T> class Sequence
{
    public:
    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(int index) = 0;

    virtual Sequence <T>* GetSubsequence (int start, int end) = 0;
    virtual int GetLength() = 0;

    virtual Sequence <T>* Append (T item);
    virtual Sequence <T>* Prepend (T item);

    virtual Sequence <T>* InsertAt (T item, int index);

    virtual Sequence <T>* Concat (Sequence <T>* list) = 0;
    virtual Sequence <T>* Instance () = 0;
    virtual Sequence <T>* AppendImpl (const T& item) = 0;
    virtual Sequence <T>* InsertAtImpl(const T& item) = 0;
    virtual Sequence <T>* PrependImpl(const T& item) = 0;
};

template <class T> class ArraySequence : public Sequence<T>
{
    protected:
    DynamicArray <T>* data;

    public:
    ArraySequence ();
    ArraySequence (T* items, int count);
    ArraySequence (const LinkedList <T>& list);
    T GetFirst () override;
    T GetLast () override;
    T Get (int index) override;
    ArraySequence <T>* GetSubsequence (int start, int end) override;
    int GetLength() override;
    ArraySequence <T>* AppendImpl (const T& item) override;
    ArraySequence <T>* InsertAtImpl(const T& item, int index) override;
    ArraySequence <T>* PrependImpl(const T& item) override;
    ArraySequence <T>* Concat (ArraySequence <T>* list);
};

template <class T> class ListSequence : public Sequence <T>
{
    protected:
    LinkedList <T>* items;
    public:
    ListSequence ();
    ListSequence (T* items, int count);
    ListSequence (const LinkedList <T>& list);
    T GetFirst () override;
    T GetLast () override;
    T Get (int index) override;
    ListSequence <T>* GetSubsequence (int start, int end) override;
    int GetLength() override;
    ListSequence <T>* AppendImpl (const T& item) override;
    ListSequence <T>* InsertAtImpl(const T& item, int index) override;
    ListSequence <T>* PrependImpl(const T& item) override;
    ListSequence <T>* Concat (ListSequence <T>* list);
};

template <class T> class MutableArraySequence : public ArraySequence<T> 
{
    protected:
    virtual Sequence<T> *instance() override;
};

template <class T> class ImmutableArraySequence : public ArraySequence<T> 
{
    protected:
    virtual Sequence<T> *instance() override;
    public:
    ImmutableArraySequence( const ImmutableArraySequence<T> &other );
};

#endif