#ifndef ARRAY_SEQUENCE_TPP
#define ARRAY_SEQUENCE_TPP

template <class T>
Sequence<T>* ArraySequence<T>::instance()
{
    return new ArraySequence<T>(*this);
}

template <class T>
Sequence<T>* ArraySequence<T>::AppendImpl(const T& item)
{
    data->Append(item);
    return this;
}

template <class T>
Sequence<T>* ArraySequence<T>::InsertAtImpl(const T& item, size_t index)
{
    if (index > data->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в ArraySequence::InsertAtImpl");
    }
    data->InsertAt(item, index);
    return this;
}

template <class T>
Sequence<T>* ArraySequence<T>::PrependImpl(const T& item)
{
    data->Prepend(item);
    return this;
}

template <class T>
ArraySequence<T>::ArraySequence() : data(new DynamicArray<T>()) {}

template <class T>
ArraySequence<T>::ArraySequence(T* items, size_t count) : data(new DynamicArray<T>(items, count)) {}

template <class T>
ArraySequence<T>::ArraySequence(const DynamicArray<T>& array) : data(new DynamicArray<T>(array)) {}

template <class T>
ArraySequence<T>::ArraySequence(const LinkedList<T>& list) : data(new DynamicArray<T>(list)) {}

template <class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other) : data(new DynamicArray<T>(*other.data)) {}

template <class T>
ArraySequence<T>::ArraySequence(std::initializer_list<T> list) : data(new DynamicArray<T>())
{
    for (const T& val : list)
    {
        data->Append(val);
    }
}

template <class T>
ArraySequence<T>::~ArraySequence()
{
    delete data;
}

template <class T>
T ArraySequence<T>::GetFirst()
{
    if (data->GetSize() == 0)
    {
        throw SequenceEmptyException("ArraySequence пуста - невозможно получить первый элемент");
    }
    return data->Get(0);
}

template <class T>
T ArraySequence<T>::GetLast()
{
    if (data->GetSize() == 0)
    {
        throw SequenceEmptyException("ArraySequence пуста - невозможно получить последний элемент");
    }
    return data->Get(data->GetSize() - 1);
}

template <class T>
T ArraySequence<T>::Get(size_t index)
{
    if (index >= data->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в ArraySequence::Get");
    }
    return data->Get(index);
}

template <class T>
Sequence<T>* ArraySequence<T>::GetSubsequence(size_t start, size_t end)
{
    size_t size = data->GetSize();
    if (start > end || end >= size)
    {
        throw IndexOutOfRangeException("Неверные начальный или конечный индексы в ArraySequence::GetSubsequence");
    }
    
    size_t length = end - start + 1;
    T* subseq = new T[length];
    for (size_t i = 0; i < length; i++)
    {
        subseq[i] = data->Get(start + i);
    }
    
    ArraySequence<T>* result = new ArraySequence<T>(subseq, length);
    delete[] subseq;
    return result;
}

template <class T>
size_t ArraySequence<T>::GetLength()
{
    return data->GetSize();
}

template <class T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* list)
{
    if (list == nullptr)
    {
        throw NullPointerException("Невозможно объединить с нулевым указателем в ArraySequence::Concat");
    }
    
    ArraySequence<T>* result = new ArraySequence<T>(*this);
    for (size_t i = 0; i < list->GetLength(); i++)
    {
        result->data->Append(list->Get(i));
    }
    return result;
}

#endif