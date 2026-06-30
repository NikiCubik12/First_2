#ifndef IMMUTABLE_ARRAY_SEQUENCE_TPP
#define IMMUTABLE_ARRAY_SEQUENCE_TPP

template <class T>
Sequence<T>* ImmutableArraySequence<T>::instance()
{
    return new ImmutableArraySequence<T>(*this);
}

template <class T>
Sequence<T>* ImmutableArraySequence<T>::AppendImpl(T item)
{
    DynamicArray<T>* newData = new DynamicArray<T>(*this->data);
    newData->Append(item);
    ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(*newData);
    delete newData;
    return result;
}

template <class T>
Sequence<T>* ImmutableArraySequence<T>::InsertAtImpl(T item, size_t index)
{
    if (index > this->data->GetSize())
    {
        throw IndexOutOfRangeException("ImmutableArraySequence::InsertAtImpl index=" + std::to_string(index) + " size=" + std::to_string(this->data->GetSize()));
    }
    DynamicArray<T>* newData = new DynamicArray<T>(*this->data);
    newData->InsertAt(item, index);
    ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(*newData);
    delete newData;
    return result;
}

template <class T>
Sequence<T>* ImmutableArraySequence<T>::PrependImpl(T item)
{
    DynamicArray<T>* newData = new DynamicArray<T>(*this->data);
    newData->Prepend(item);
    ImmutableArraySequence<T>* result = new ImmutableArraySequence<T>(*newData);
    delete newData;
    return result;
}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence() : ArraySequence<T>() {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(T* items, size_t count) : ArraySequence<T>(items, count) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(DynamicArray<T> list) : ArraySequence<T>(list) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(*other.data) {}

template <class T>
ImmutableArraySequence<T>::ImmutableArraySequence(std::initializer_list<T> list) : ArraySequence<T>()
{
    for (T val : list)
    {
        this->data->Append(val);
    }
}

#endif