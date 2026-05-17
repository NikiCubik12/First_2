#ifndef MUTABLE_ARRAY_SEQUENCE_TPP
#define MUTABLE_ARRAY_SEQUENCE_TPP

template <class T>
MutableArraySequence<T>::MutableArraySequence() : ArraySequence<T>() {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(T* items, size_t count) : ArraySequence<T>(items, count) {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(const DynamicArray<T>& list) : ArraySequence<T>(list) {}

template <class T>
MutableArraySequence<T>::MutableArraySequence(std::initializer_list<T> list) : ArraySequence<T>()
{
    for (const T& val : list)
    {
        this->Append(val);
    }
}

template <class T>
void MutableArraySequence<T>::Set(size_t index, const T& value)
{
    if (index >= this->data->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в MutableArraySequence::Set");
    }
    this->data->Set(index, value);
}

template <class T>
void MutableArraySequence<T>::Set(size_t index, MutableArraySequence<T>* value)
{
    if (index >= this->data->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в MutableArraySequence::Set");
    }
    this->data->Set(index, value);
}

#endif