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

#endif