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
    for (T val : list)
    {
        this->Append(val);
    }
}

template <class T>
void MutableArraySequence<T>::Set(size_t index, T value)
{
    if (index >= this->data->GetSize())
    {
        throw IndexOutOfRangeException("MutableArraySequence::Set index=" + std::to_string(index) + " size=" + std::to_string(this->data->GetSize()));
    }
    this->data->Set(index, value);
}

#endif