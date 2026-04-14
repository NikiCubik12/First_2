#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;
using std::cin, std::cout, std::out_of_range, std::invalid_argument
#include "Sequence.hpp"

template <typename T> 
virtual Sequence <T>* Sequence :: Append (T item)
{
    return Instance()->AppendImpl(elem);
};

template <typename T> 
virtual Sequence <T>* Sequence :: Prepend (T item)
{
    return Instance()->PrependImpl(elem);
};

template <typename T> 
virtual Sequence <T>* Sequence :: InsertAt (T item, int index)
{
    return Instance()->InsertAtImpl(elem);
};

template <typename T> 
ArraySequence<T> :: ArraySequence ()
{
    data = new Dynamic_Array<T> ();
}

template <typename T> 
ArraySequence<T> :: ArraySequence (T* items, int count)
{
    data = new Dynamic_Array<T> (items, size);
}

template <typename T> 
ArraySequence<T> :: ArraySequence (LinkedList <T>& list const)
{
    data = new DynamicArray<T> (list);
}

template <typename T> 
T ArraySequence<T> :: GetFirst () override
{
    if (data->size == 0)
    {
        throw out_of_range("ArraySequence is empty - cannot get first element");
    }
        
    else 
    {
        return data->Get(0);
    }
};

template <typename T> 
T ArraySequence<T> :: GetLast () override
{
    if (data->size == 0)
    {
        throw out_of_range("ArraySequence is empty - cannot get last element");
    }
        
    else 
    {
        return data->Get(size-1);
    }
};

template <typename T> 
T ArraySequence<T> :: Get (int index) override
{
    if ((index < 0) || (index > data->size-1))
    {
        throw out_of_range("Index out of range in ArraySequence::Get");
    }
        
    else 
    {
        return data->Get(index)
    }
};

template <typename T> 
ArraySequence <T>* ArraySequence<T> :: GetSubsequence (int start, int end) override
{
    if ((start < 0) || (end > data->size-1) || (start > end))
    {
        throw out_of_range("Invalid start or end indices in ArraySequence::GetSubsequence");
    }
        
    else 
    {
        T* subseq = new T[end - start + 1]
        for (int i = 0; i < (end - start + 1); i++)
            subseq[i] = data->Get(start + i);
        
        ArraySequence <T>* result = new ArraySequence <T> (subseq, (end - start + 1));
    }
    return result;
};

template <typename T> 
int ArraySequence :: GetLength()
{
    return data->size;
};

template <typename T> 
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

template <typename T> 
ArraySequence <T>* ArraySequence<T> :: InsertAtImpl(const T& item, int index) override
{
    if ((index < 0) || (index > data->size-1))
    {
        throw out_of_range("Index out of range in ArraySequence::InsertAtImpl");
    }
        
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

template <typename T> 
ArraySequence <T>* ArraySequence<T> :: PrependImpl(const T& item, int index) override
{
    if ((index < 0) || (index > data->size-1))
    {
        throw out_of_range("Index out of range in ArraySequence::PrependImpl");
    }
        
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

template <typename T> 
ArraySequence <T>* ArraySequence<T> :: Concat (ArraySequence <T>* list)
{
    if (list == nullptr)
    {
        throw invalid_argument("Cannot concatenate with null pointer in ArraySequence::Concat");
    }    
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


template <typename T> 
ListSequence <T> :: ListSequence ()
{
    items = new LinkedList<T> ();
}

template <typename T> 
ListSequence <T> :: ListSequence (T* items, int count)
{
    items = new LinkedList<T> (items, count);
}

template <typename T> 
ListSequence<T> :: ListSequence(const LinkedList<T>& list)
{
    items = new LinkedList<T> (list);
}

template <typename T> 
T ListSequence<T> :: GetFirst()
{
    if (items->GetLength() == 0)
        throw out_of_range("ListSequence is empty - cannot get first element");
    return items->Get(0);
}

template <typename T> 
T ListSequence<T> :: GetLast()
{
    if (items->GetLength() == 0)
        throw out_of_range("ListSequence is empty - cannot get last element");
    return items->Get(items->GetLength() - 1);
}

template <typename T> 
T ListSequence<T> :: Get(int index)
{
    if (index < 0 || index >= items->GetLength())
        throw out_of_range("Выход за границы диапазона в ListSequence::Get");
    return items->Get(index);
}

template <typename T> 
ListSequence<T>* ListSequence<T> :: GetSubsequence(int start, int end)
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

template <typename T> 
int ListSequence<T> :: GetLength()
{
    return items->GetLength();
}

template <typename T> 
ListSequence<T>* ListSequence<T> :: AppendImpl(const T& item)
{
    items->Append(item);
    return this;
}

template <typename T> 
ListSequence<T>* ListSequence<T> :: InsertAtImpl(const T& item, int index)
{
    if (index < 0 || index > items->GetLength())
        throw out_of_range("Выход за границы диапазона в ListSequence::InsertAtImpl");
    
    items->InsertAt(item, index);
    return this;
}

template <typename T> 
ListSequence<T>* ListSequence<T> :: PrependImpl(const T& item)
{
    items->Prepend(item);
    return this;
}

template <typename T> 
ListSequence<T>* ListSequence<T> :: Concat(Sequence<T>* list)
{
    if (list == nullptr)
        throw invalid_argument("Не может объединиться с пустым указателем в ListSequence::Concat");
    
    for (int i = 0; i < list->GetLength(); i++)
        items->Append(list->Get(i));
    
    return this;
}

template <typename T> 
Sequence<T>* MutableArraySequence::instance() override 
{
    return this;  // возвращаем себя же (изменяем текущий объект)
}

template <typename T> 
virtual ImmutableArraySequence::Sequence<T>* instance() override 
{
    // возвращаем КОПИЮ текущего объекта
    return new ImmutableArraySequence<T>(*this);
}


// Оператор присваивания (если нужен)
template <typename T> 
ImmutableArraySequence<T>& ImmutableArraySequence::operator=(const ImmutableArraySequence<T>& other) 
{
        if (this != &other) {
            ArraySequence<T>::operator=(other);
        }
        return *this;
}



BitSequence::BitSequence ()
{
   bits = new DynamicArray ();
}

BitSequence::BitSequence (unsigned a)
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

    bits = new DynamicArray (b, size);
}

BitSequence::BitSequence (bool* items, int size)
{
    bits = new DynamicArray (items, size);
}

BitSequence::BitSequence (const BitSequence & rhs)
{
    bits = new DynamicArray (rhs.bits);
}

bool BitSequence::GetFirst () override
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

bool BitSequence::GetLast () override
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

bool BitSequence::Get (int index) override
{
    if (index > 0 && index < bits->GetSize())
    {
        return bits->Get(index);
    }
    else
    {
        throw invalid_argument("Индекс выходит за границы последовательности");
    }
}

BitSequence* BitSequence::GetSubsequence (int start, int end) override
{
    if ((start < 0) || (end > data->size-1) || (start > end))
    {
        throw invalid_argument("Неверные границы подпоследовательности");
    }
    else 
    {
            bool* subseq = new bool[end - start + 1]
            for (int i = 0; i < (end - start + 1); i++)
                subseq[i] = bits->Get(start + i);

            BitSequence* result = new BitSequence* (subseq, (end - start + 1));
            delete [] subseq;
            return result;
    }
}

int BitSequence::GetLength() override
{
    return bits->GetSize();
}

BitSequence* BitSequence::AppendImpl (const bool& item) override
{
    bool* arr = new bool[bits->GetSize() + 1];
    for (int i = 0; i < bits->GetSize(); i++)
        arr[i] = bits->Get(i);
    arr[bits->GetSize()] = item;

   delete [] bits->items;
   bits->size++;

   bits->items = arr;
   return this;
}

BitSequence* BitSequence::InsertAtImpl(const bool& item, int index) override
{
    if ((index < 0) || (index > bits->GetSize()-1))
    {
        throw invalid_argument("Индекс выходит за границы последовательности");
    }
    else 
    {
        bool* arr = new bool[bits->GetSize() + 1];
        for (int i = 0; i < index; i++)
            arr[i] = bits->Get(i);
    
        arr[index] = item;
    
        for (int i = index; i < bits->GetSize(); i++)
            arr[i + 1] = bits->Get(i);
    
       delete [] bits->items;
       bits->size++;
    
       bits->items = arr;
       return this;
    }

}

BitSequence* BitSequence::PrependImpl(const bool& item) override
{
    if ((index < 0) || (index > bits->GetSize()-1))
    {
        throw invalid_argument("Индекс выходит за границы последовательности");
    }
    else 
    {
        bool* arr = new bool[bits->GetSize() + 1];
        arr[0] = item;
    
        for (int i = 0; i < bits->GetSize(); i++)
            arr[i+1] = bits->Get(i);
    
        delete [] bits->items;
        bits->size++;
    
        bits->items = arr;
        return this;
    }
}

BitSequence* BitSequence::Concat (BitSequence* rhs)
{
    if (list == nullptr)
    {      
        throw invalid_argument("Последовательность не может быть пустой");
    } 
    else 
    {
        int new_size = bits->GetSize() + rhs->GetSize();
        bool* arr = new bool[new_size];
        for (int i = 0; i < bits->GetSize(); i++)
            arr[i] = bits->Get(i);
    
        for (int i = 0; i < list->data->size; i++)
            arr[bits->GetSize() + i] = rhs->bits->Get(i);
    
        delete [] bits->items;
        bits->items = arr;
        bits->size = new_size;
        return this;
    }
}

BitSequence* BitSequence::BitAnd (const BitSequence* rhs)
{
    if (bits->GetSize() != rhs->bits->GetSize()) 
        throw invalid_argument("Размеры последовательностей не совпадают");
    else
    {
        BitSequence* result = new BitSequence();
        for (int i = 0; i < bits->GetSize(); i++)
            result->Append(bits->Get(i) && rhs->bits->Get(i));
        return result;
    }
}

BitSequence* BitSequence::BitOr (const BitSequence* rhs)
{
    if (bits->GetSize() != rhs->bits->GetSize()) 
        throw invalid_argument("Размеры последовательностей не совпадают");
    else
    {
        BitSequence* result = new BitSequence();
        for (int i = 0; i < bits->GetSize(); i++)
            result->Append(bits->Get(i) || rhs->bits->Get(i));
        return result;
    }
}

BitSequence* BitSequence::BitXor (const BitSequence* rhs)
{
    if (bits->GetSize() != rhs->bits->GetSize()) 
        throw invalid_argument("Размеры последовательностей не совпадают");
    else
    {
        BitSequence* result = new BitSequence();
        for (int i = 0; i < bits->GetSize(); i++)
            result->Append(bits->Get(i) ^ rhs->bits->Get(i));
        return result;
    }
}

BitSequence* BitSequence::BitNot ()
{
    BitSequence* result = new BitSequence();
    for (int i = 0; i < bits->GetSize(); i++)
        result->Append(bits->Get(i) ^ 1)
    return result;
}

void BitSequence::Print ()
{
    if (bits->GetSize() > 0)
    {
        for (int i=0; i<bits->GetSize(); i++)
            cout << bits-> (int) Get (i);
        cout << endl; 
    }
    else
    {
        throw invalid_argument("Последовательность пуста");
    }
}