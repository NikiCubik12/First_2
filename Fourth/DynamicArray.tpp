#ifndef DYNAMIC_ARRAY_TPP
#define DYNAMIC_ARRAY_TPP

template <class T>
DynamicArray<T>::DynamicArray() : items(nullptr), size(0) {}

template <class T>
DynamicArray<T>::DynamicArray(T* items, size_t size) : items(new T[size]), size(size)
{
    for (size_t i = 0; i < size; i++)
    {
        this->items[i] = items[i];
    }
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& rhs) : items(new T[rhs.size]), size(rhs.size)
{
    for (size_t i = 0; i < rhs.size; i++)
    {
        this->items[i] = rhs.items[i];
    }
}

template <class T>
DynamicArray<T>::DynamicArray(LinkedList<T>& list) : items(nullptr), size(0)
{
    if (list.GetLength() > 0)
    {
        size = list.GetLength();
        items = new T[size];
        for (size_t i = 0; i < size; i++)
        {
            items[i] = list.Get(i);
        }
    }
}

template <class T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> list) : items(new T[list.size()]), size(list.size())
{
    size_t i = 0;
    for (const T& val : list)
    {
        items[i++] = val;
    }
}

template <class T>
DynamicArray<T>::~DynamicArray()
{
    if (items)
    {
        delete[] items;
    }
}

template <class T>
void DynamicArray<T>::Append(const T& item)
{
    T* arr = new T[size + 1];
    for (size_t i = 0; i < size; i++)
    {
        arr[i] = items[i];
    }
    arr[size] = item;
    delete[] items;
    size++;
    items = arr;
}

template <class T>
void DynamicArray<T>::InsertAt(const T& item, size_t index)
{
    if (index > size)
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в DynamicArray::InsertAt");
    }
    
    T* arr = new T[size + 1];
    for (size_t i = 0; i < index; i++)
    {
        arr[i] = items[i];
    }
    arr[index] = item;
    for (size_t i = index; i < size; i++)
    {
        arr[i + 1] = items[i];
    }
    delete[] items;
    items = arr;
    size++;
}

template <class T>
void DynamicArray<T>::Prepend(const T& item)
{
    T* arr = new T[size + 1];
    arr[0] = item;
    for (size_t i = 0; i < size; i++)
    {
        arr[i + 1] = items[i];
    }
    delete[] items;
    size++;
    items = arr;
}

template <class T>
T DynamicArray<T>::Get(size_t index) const
{
    if (index >= size)
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в DynamicArray::Get");
    }
    return items[index];
}

template <class T>
size_t DynamicArray<T>::GetSize() const
{
    return size;
}

template <class T>
void DynamicArray<T>::Set(size_t index, T value)
{
    if (index >= size)
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в DynamicArray::Set");
    }
    items[index] = value;
}

template <class T>
void DynamicArray<T>::Resize(size_t newSize)
{
    T* arr = new T[newSize];
    size_t copySize = (newSize < size) ? newSize : size;
    for (size_t i = 0; i < copySize; i++)
    {
        arr[i] = items[i];
    }
    delete[] items;
    size = newSize;
    items = arr;
}

template <class T>
T& DynamicArray<T>::GetRef(size_t index)
{
    if (index >= size)
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в DynamicArray::GetRef");
    }
    return items[index];
}

template <class T>
const T& DynamicArray<T>::GetRef(size_t index) const
{
    if (index >= size)
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в DynamicArray::GetRef");
    }
    return items[index];
}

#endif