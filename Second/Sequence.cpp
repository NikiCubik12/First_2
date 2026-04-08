#include <iostream>
#include <string>
using namespace std;
#include "Sequence.h"


virtual Sequence <T>* Sequence :: Append (T item)
{
    return Instance()->AppendImpl(elem);
};

virtual Sequence <T>* Sequence :: Prepend (T item)
{
    return Instance()->PrependImpl(elem);
};

virtual Sequence <T>* Sequence :: InsertAt (T item, int index)
{
    return Instance()->InsertAtImpl(elem);
};



ArraySequence<T> :: ArraySequence ()
{
    data = new Dynamic_Array<T> ();
}
ArraySequence<T> :: ArraySequence (T* items, int count)
{
    data = new Dynamic_Array<T> (items, size);
}
ArraySequence<T> :: ArraySequence (LinkedList <T>& list const)
{
    data = new DynamicArray<T> (list);
}

T ArraySequence<T> :: GetFirst () override
{
    if (data->size == 0)
        // исключение
    else 
    {
        return data->Get(0);
    }
};

T ArraySequence<T> :: GetLast () override
{
    if (data->size == 0)
        // исключение
    else 
    {
        return data->Get(size-1);
    }
};

T ArraySequence<T> :: Get (int index) override
{
    if ((index < 0) || (index > data->size-1))
        // исключение
    else 
    {
        return data->Get(index)
    }
};

ArraySequence <T>* ArraySequence<T> :: GetSubsequence (int start, int end) override
{
    if ((start < 0) || (end > data->size-1) || (start > end))
    // исключение
    else 
    {
        T* subseq = new T[end - start + 1]
        for (int i = 0; i < (end - start + 1); i++)
            subseq[i] = data->Get(start + i);
        
        ArraySequence <T>* result = new ArraySequence <T> (subseq, (end - start + 1));
    }
    return result;
};

int ArraySequence :: GetLength()
{
    return data->size;
};

ArraySequence <T>* ArraySequence<T> :: AppendImpl (const T& item) override
{
    T* arr = new T[data->size + 1];
    for (int i = 0; i < data->size; i++)
        arr[i] = data->Get(i);
    arr[data->size] = item;

    delete [] data->items;
    data->size++;

    data->items = arr;
    return this;
};

ArraySequence <T>* ArraySequence<T> :: InsertAtImpl(const T& item, int index) override
{
    if ((index < 0) || (index > data->size-1))
        // исключение
    else 
    {
        T* arr = new T[data->size + 1];
        for (int i = 0; i < index; i++)
            arr[i] = data->Get(i);

        arr[index] = item;

        for (int i = index; i < data->size; i++)
            arr[i + 1] = data->Get(i)

        delete [] data->items;
        data->items = arr;
        return this;
    }
};

ArraySequence <T>* ArraySequence<T> :: PrependImpl(const T& item, int index) override
{
    if ((index < 0) || (index > data->size-1))
        // исключение
    else 
    {
        T* arr = new T[data->size+1];

        arr[0] = item;
        
        for (int i = 0; i < size; i++)
            arr[i] = data->Get(i)
        
        delete [] data->items;
        data->size++;
        data->items = arr;
        return this;
    }
};

ArraySequence <T>* ArraySequence<T> :: Concat (ArraySequence <T>* list)
{
    if (list == nullptr)
        // исключение
    else 
    {
        int new_size = data->size + list->data->size;
        T* arr = new T[new_size];
        for (int i = 0; i < data->size; i++)
            arr[i] = data->Get(i);
        
        for (int i = 0; i < list->data->size; i++)
            arr[data-> size + i] = list->data->Get(i);
        
        delete [] data->items;
        data->items = arr;
        data->size = new_size;
        return this;
    }
};



ListSequence <T> :: ListSequence ()
{
    data = new LinkedList<T> ();
}

ListSequence <T> :: ListSequence (T* items, int count)
{
    data = new LinkedList<T> (items, count);
}

ListSequence<T> :: ListSequence(const LinkedList<T>& list)
{
    data = new LinkedList<T> (list);
}

T ListSequence<T> :: GetFirst()
{
    if (data->GetLength() == 0)
        // исключение
    return data->Get(0);
}

T ListSequence<T> :: GetLast()
{
    if (data->GetLength() == 0)
        // исключение
    return data->Get(data->GetLength() - 1);
}

T ListSequence<T> :: Get(int index)
{
    if (index < 0 || index >= data->GetLength())
        // исключение
    return data->Get(index);
}

ListSequence<T>* ListSequence<T> :: GetSubsequence(int start, int end)
{
    if (start < 0 || end >= data->GetLength() || start > end)
        // исключение
    
    LinkedList<T>* sublist = new LinkedList<T>();
    for (int i = start; i <= end; i++)
        sublist->Append(data->Get(i));
    
    ListSequence<T>* result = new ListSequence<T>(*sublist);
    delete sublist;
    return result;
}

int ListSequence<T> :: GetLength()
{
    return data->GetLength();
}

ListSequence<T>* ListSequence<T> :: AppendImpl(const T& item)
{
    data->Append(item);
    return this;
}

ListSequence<T>* ListSequence<T> :: InsertAtImpl(const T& item, int index)
{
    if (index < 0 || index > data->GetLength())
        // исключение
    
    data->InsertAt(item, index);
    return this;
}

ListSequence<T>* ListSequence<T> :: PrependImpl(const T& item)
{
    data->Prepend(item);
    return this;
}

ListSequence<T>* ListSequence<T> :: Concat(Sequence<T>* list)
{
    if (list == nullptr)
        // исключение
    
    for (int i = 0; i < list->GetLength(); i++)
        data->Append(list->Get(i));
    
    return this;
}



template <class T> class MutableArraySequence : public ArraySequence<T> 
{
    protected:
    virtual Sequence<T> *instance() override 
    {
        return this;
    }
};

template <class T> class ImmutableArraySequence : public ArraySequence<T> 
{
    protected:
    virtual Sequence<T> *instance() override 
    {
        return new ImmutableArraySequence<T>(*this);
    }
    public:
    ImmutableArraySequence( const ImmutableArraySequence<T> &other );
};