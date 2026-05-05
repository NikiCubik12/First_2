#ifndef ARRAY_SEQUENCE_HPP
#define ARRAY_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"


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

    ArraySequence (const DynamicArray<T>& array)
    {
        data = new DynamicArray<T> (array);
    }

    ArraySequence (const LinkedList <T>& list)
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

    ArraySequence <T>* GetSubsequence (int start, int end) override
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

    ArraySequence <T>* AppendImpl (const T& item) override
    {
        data->Append(item);
        return this;
    };

    ArraySequence <T>* InsertAtImpl(const T& item, int index) override
    {
        if ((index < 0) || (index > data->GetSize()))
        {
            throw out_of_range("Index out of range in ArraySequence::InsertAtImpl");
        }
            
        else 
        {
            data->InsertAt(item, index);
            return this;
        }
    };

    ArraySequence <T>* PrependImpl(const T& item) override
    {
        data->Prepend(item);
        return this;
    };

    Sequence<T>* Concat(Sequence<T>* list) override
    {
        if (list == nullptr)
        {
            throw invalid_argument("Cannot concatenate with null pointer in ArraySequence::Concat");
        }
        
        // Создаем НОВУЮ последовательность вместо изменения this
        ArraySequence<T>* result = new ArraySequence<T>(*this);
        for (int i = 0; i < list->GetLength(); i++)
            result->data->Append(list->Get(i));

        return result;
    }

    Sequence<T>* instance() override
    {
        return new ArraySequence<T>(*this);
    }
};

#endif