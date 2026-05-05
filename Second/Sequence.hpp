#ifndef SEQUENCE
#define SEQUENCE

#include <iostream>
#include <string>
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
using namespace std;

template <class T> class DynamicArray;
template <class T> class LinkedList;

template <class T> class Sequence
{
    public:
    virtual ~Sequence() = default;
    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(int index) = 0;

    virtual Sequence <T>* GetSubsequence (int start, int end) = 0;
    virtual int GetLength() = 0;

    Sequence <T>* Append (T item)
    {
        return instance()->AppendImpl(item);
    };

    Sequence <T>* Prepend (T item)
    {
        return instance()->PrependImpl(item);
    };

    Sequence <T>* InsertAt (T item, int index)
    {
        return instance()->InsertAtImpl(item, index);
    };

    virtual Sequence <T>* Concat (Sequence <T>* list) = 0;
    virtual Sequence<T>* instance() = 0; 
    virtual Sequence <T>* AppendImpl (const T& item) = 0;
    virtual Sequence <T>* InsertAtImpl(const T& item, int index) = 0;
    virtual Sequence <T>* PrependImpl(const T& item) = 0;
};

#endif