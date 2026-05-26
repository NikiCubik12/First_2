#ifndef IMMUTABLE_ARRAY_SEQUENCE_TPP
#define IMMUTABLE_ARRAY_SEQUENCE_TPP

template <class T>
Sequence<T>* ImmutableArraySequence<T>::instance()
{
    return new ImmutableArraySequence<T>(*this);
}

template <class T>
Sequence<T>* ImmutableArraySequence<T>::AppendImpl(const T& item)
{
    DynamicArray<T>* newData = new DynamicArray<T>(*this->data);
    newData->Append(item);
    
    ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(*newData);
    delete newData;
    
    return result;
}

template <class T>
Sequence<T>* ImmutableArraySequence<T>::InsertAtImpl(const T& item, size_t index)
{
    if (index > this->data->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в ImmutableArraySequence::InsertAtImpl");
    }
    
    // Создаём НОВЫЙ массив с вставленным элементом
    DynamicArray<T>* newData = new DynamicArray<T>(*this->data);
    newData->InsertAt(item, index);
    
    // Создаём НОВУЮ последовательность
    ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(*newData);
    delete newData;
    
    return result;
}

template <class T>
Sequence<T>* ImmutableArraySequence<T>::PrependImpl(const T& item)
{
    // Создаём НОВЫЙ массив с добавленным элементом в начало
    DynamicArray<T>* newData = new DynamicArray<T>(*this->data);
    newData->Prepend(item);
    
    // Создаём НОВУЮ последовательность
    ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(*newData);
    delete newData;
    
    return result;
}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence() : ArraySequence<T>() {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(T* items, size_t count) : ArraySequence<T>(items, count) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const DynamicArray<T>& list) : ArraySequence<T>(list) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(*other.data) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(std::initializer_list<T> list) : ArraySequence<T>()
{
    for (const T& val : list)
    {
        this->data->Append(val);
    }
}

#endif