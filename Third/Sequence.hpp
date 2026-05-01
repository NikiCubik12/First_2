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

template <class T> class ArraySequence : public Sequence<T>
{
    protected:
    DynamicArray <T>* data;

    public:
    ArraySequence () 
    {
        data = new DynamicArray<T> ();
    }

    ArraySequence (T* items, int count)
    {
        data = new DynamicArray<T> (items, count);
    }

    ArraySequence (LinkedList <T>& list)
    {
        data = new DynamicArray<T> (list);
    }

    T GetFirst () override
    {
        if (data->GetSize() == 0)
        {
            throw out_of_range("ArraySequence is empty - cannot get first element");
        }
            
        else 
        {
            return data->Get(0);
        }
    };

    T GetLast () override
    {
        if (data->GetSize() == 0)
        {
            throw out_of_range("ArraySequence is empty - cannot get last element");
        }
            
        else 
        {
            return data->Get(data->GetSize()-1);
        }
    };

    T Get (int index) override
    {
        if ((index < 0) || (index > data->GetSize()-1))
        {
            throw out_of_range("Index out of range in ArraySequence::Get");
        }
           
        else 
        {
            return data->Get(index);
        }
    };

    Sequence <T>* GetSubsequence (int start, int end) override
    {
        if ((start < 0) || (end > data->GetSize()-1) || (start > end))
        {
            throw out_of_range("Invalid start or end indices in ArraySequence::GetSubsequence");
        }
            
        else 
        {
            T* subseq = new T[end - start + 1];
            for (int i = 0; i < (end - start + 1); i++)
                subseq[i] = data->Get(start + i);
            
            ArraySequence <T>* result = new ArraySequence <T> (subseq, (end - start + 1));
            return result;
        }
    };

    int GetLength() override
    {
        return data->GetSize();
    };

    Sequence <T>* AppendImpl (const T& item) override
    {
        T* arr = new T[data->GetSize() + 1];
        for (int i = 0; i < data->GetSize(); i++)
            arr[i] = data->Get(i);
        arr[data->GetSize()] = item;

        delete [] data->items;
        data->size++;

        data->items = arr;
        return this;
    };

    Sequence <T>* InsertAtImpl(const T& item, int index) override
    {
        if ((index < 0) || (index > data->GetSize()-1))
        {
            throw out_of_range("Index out of range in ArraySequence::InsertAtImpl");
        }
            
        else 
        {
            T* arr = new T[data->GetSize() + 1];
            for (int i = 0; i < index; i++)
                arr[i] = data->Get(i);

            arr[index] = item;

            for (int i = index; i < data->GetSize(); i++)
                arr[i + 1] = data->Get(i);

            delete [] data->items;
            data->items = arr;
            return this;
        }
    };

    Sequence <T>* PrependImpl(const T& item) override
    {
        T* arr = new T[data->GetSize()+1];

        arr[0] = item;
        
        for (int i = 0; i < data->GetSize(); i++)
            arr[i] = data->Get(i);
        
        delete [] data->items;
        data->size++;
        data->items = arr;
        return this;
    };


    Sequence <T>* Concat (Sequence <T>* list) override
    {
        if (list == nullptr)
        {
            throw invalid_argument("Cannot concatenate with null pointer in ArraySequence::Concat");
        }    
        else 
        {
            int new_size = data->GetSize() + list->GetLength();
            T* arr = new T[new_size];
            for (int i = 0; i < data->GetSize(); i++)
                arr[i] = data->Get(i);
            
            for (int i = 0; i < list->GetLength(); i++)
                arr[data-> size + i] = list->Get(i);
            
            delete [] data->items;
            data->items = arr;
            data->size = new_size;
            return this;
        }
    };

    Sequence<T>* instance() override
    {
        return new ArraySequence<T>(*this);
    }
};

template <class T> class ListSequence : public Sequence <T>
{
    protected:
    LinkedList <T>* items;
    public:

    ListSequence ()
    {
        items = new LinkedList<T> ();
    }

    ListSequence (T* items, int count)
    {
        items = new LinkedList<T> (items, count);
    }

    ListSequence(const LinkedList<T>& list)
    {
        items = new LinkedList<T> (list);
    }

    T GetFirst()
    {
        if (items->GetLength() == 0)
            throw out_of_range("ListSequence is empty - cannot get first element");
        return items->Get(0);
    }

    
    T GetLast()
    {
        if (items->GetLength() == 0)
            throw out_of_range("ListSequence is empty - cannot get last element");
        return items->Get(items->GetLength() - 1);
    }

    T Get(int index)
    {
        if (index < 0 || index >= items->GetLength())
            throw out_of_range("Выход за границы диапазона в ListSequence::Get");
        return items->Get(index);
    }

    ListSequence<T>* GetSubsequence(int start, int end)
    {
        if (start < 0 || end >= items->GetLength() || start > end)
            throw out_of_range("Неправильные индексы для начала и конца в ListSequence::GetSubsequence");
        
        LinkedList<T>* sublist = new LinkedList<T>();
        for (int i = start; i <= end; i++)
            sublist->Append(items->Get(i));
        
        ListSequence<T>* result = new ListSequence<T>(*sublist);
        delete sublist;
        return result;
    }

    int GetLength()
    {
        return items->GetLength();
    }

    ListSequence<T>* AppendImpl(const T& item)
    {
        items->Append(item);
        return this;
    }

    ListSequence<T>* InsertAtImpl(const T& item, int index)
    {
        if (index < 0 || index > items->GetLength())
            throw out_of_range("Выход за границы диапазона в ListSequence::InsertAtImpl");
        
        items->InsertAt(item, index);
        return this;
    }

    ListSequence<T>* PrependImpl(const T& item)
    {
        items->Prepend(item);
        return this;
    }

    ListSequence<T>* Concat(ListSequence<T>* list)
    {
        if (list == nullptr)
            throw invalid_argument("Не может объединиться с пустым указателем в ListSequence::Concat");
        
        for (int i = 0; i < list->GetLength(); i++)
            items->Append(list->Get(i));
        
        return this;
    }

    Sequence<T>* instance()
    {
        return new ListSequence<T>(*this);
    }
};

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

template <class T> class ImmutableArraySequence : public ArraySequence<T> 
{
    protected:
    Sequence<T>* instance()
    {
        return new ImmutableArraySequence<T>(*this);
    }

    public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence(const DynamicArray<T>& list) : ArraySequence<T>(list) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(*(other.data)) {} // Копируем данные из other
};


class BitSequence: public Sequence <bool>
{
    private:
    DynamicArray <bool>* bits;

    public:
    Sequence<bool>* instance()
    {
        return new BitSequence(*this);
    }

    BitSequence ()
    {
        bits = new DynamicArray<bool> ();
    }

    BitSequence (unsigned a)
    {
        int size = 0;
        int t = a;

        while (t > 0)
        {
            size++;
            t = t / 2;
        }

        bool* b = new bool[size];
        for (int i = size-1; i >= 0; i--)
        {
            b[i] = a % 2;
            a /= 2;
        }

        bits = new DynamicArray<bool> (b, size);
    }

    BitSequence (bool* items, int size)
    {
        bits = new DynamicArray<bool> (items, size);
    }

    BitSequence (const BitSequence & rhs)
    {
        bits = new DynamicArray<bool> (*rhs.bits);
    }

    bool GetFirst () 
    {
        if (bits->GetSize() == 0)
        {      
            throw invalid_argument("Последовательность пуста");
        }
        else 
        {
            return bits->Get(0);
        }
    }

    bool GetLast () 
    {
        if (bits->GetSize() == 0)
        {      
            throw invalid_argument("Последовательность пуста");
        }
        else 
        {
            return bits->Get(bits->GetSize()-1);
        }
    }

    bool Get (int index) 
    {
        if (index >= 0 && index < bits->GetSize())
        {
            return bits->Get(index);
        }
        else
        {
            throw invalid_argument("Индекс выходит за границы последовательности");
        }
    }

    BitSequence* GetSubsequence (int start, int end) 
    {
        if ((start < 0) || (end > bits->GetSize()-1) || (start > end))
        {
            throw invalid_argument("Неверные границы подпоследовательности");
        }
        else 
        {
            bool* subseq = new bool[end - start + 1];
            for (int i = 0; i < (end - start + 1); i++)
                subseq[i] = bits->Get(start + i);

            BitSequence* result = new BitSequence (subseq, (end - start + 1));
            delete [] subseq;
            return result;
        }
    }

    int GetLength() 
    {
        return bits->GetSize();
    }

    BitSequence* AppendImpl (const bool& item) 
    {
        bits->Append(item);
        return this;
    }

    BitSequence* InsertAtImpl(const bool& item, int index) 
    {
        bits->InsertAt(item, index);
        return this;
    }

    BitSequence* PrependImpl(const bool& item) 
    {
        bits->Prepend(item);
        return this;
    }

    BitSequence* Concat (Sequence <bool>* rhs)
    {
        if (rhs == nullptr)
        {      
            throw invalid_argument("Последовательность не может быть пустой");
        } 
        else 
        {
            BitSequence* result = new BitSequence (*this);
            for (int i = 0; i < rhs->GetLength(); i++)
                result->AppendImpl(rhs->Get(i));

            return result;
        }
    }

    BitSequence* BitAnd (const BitSequence* rhs)
    {
        if (bits->GetSize() != rhs->bits->GetSize()) 
            throw invalid_argument("Размеры последовательностей не совпадают");
        else
        {
            BitSequence* result = new BitSequence();
            for (int i = 0; i < bits->GetSize(); i++)
                result = result->AppendImpl(bits->Get(i) & rhs->bits->Get(i));
            return result;
        }
    }

    BitSequence* BitOr (const BitSequence* rhs)
    {
        if (bits->GetSize() != rhs->bits->GetSize()) 
            throw invalid_argument("Размеры последовательностей не совпадают");
        else
        {
            BitSequence* result = new BitSequence();
            for (int i = 0; i < bits->GetSize(); i++)
                result->AppendImpl(bits->Get(i) || rhs->bits->Get(i));
            return result;
        }
    }

    BitSequence* BitXor (const BitSequence* rhs)
    {
        if (bits->GetSize() != rhs->bits->GetSize()) 
            throw invalid_argument("Размеры последовательностей не совпадают");
        else
        {
            BitSequence* result = new BitSequence();
            for (int i = 0; i < bits->GetSize(); i++)
                result->AppendImpl(bits->Get(i) ^ rhs->bits->Get(i));
            return result;
        }
    }

    BitSequence* BitNot ()
    {
        BitSequence* result = new BitSequence();
        for (int i = 0; i < bits->GetSize(); i++)
            result->AppendImpl(bits->Get(i) ^ 1);
        return result;
    }

    void Print ()
    {
        if (bits->GetSize() > 0)
        {
            for (int i=0; i<bits->GetSize(); i++)
                cout << (int) bits->Get(i);
            cout << endl; 
        }
        else
        {
            throw invalid_argument("Последовательность пуста");
        }
    }
};

#endif