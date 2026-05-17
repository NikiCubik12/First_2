#ifndef BOUNDED_QUEUE_TPP
#define BOUNDED_QUEUE_TPP

#include "BoundedQueue.hpp"

template <typename T>
BoundedQueue<T>::BoundedQueue(size_t capacity)
    : data_(nullptr), capacity_(capacity), size_(0), head_(0)
{
    if (capacity == 0)
        throw std::invalid_argument("BoundedQueue: ёмкость должна быть > 0");
    data_ = new T[capacity];
}

template <typename T>
BoundedQueue<T>::BoundedQueue(const BoundedQueue<T>& other)
    : data_(new T[other.capacity_]),
      capacity_(other.capacity_),
      size_(other.size_),
      head_(0)
{
    for (size_t i = 0; i < other.size_; ++i)
        data_[i] = other.At(i);
}

template <typename T>
BoundedQueue<T>& BoundedQueue<T>::operator=(const BoundedQueue<T>& other)
{
    if (this == &other) return *this;
    T* newData = new T[other.capacity_];
    for (size_t i = 0; i < other.size_; ++i)
        newData[i] = other.At(i);
    delete[] data_;
    data_     = newData;
    capacity_ = other.capacity_;
    size_     = other.size_;
    head_     = 0;
    return *this;
}

template <typename T>
BoundedQueue<T>::~BoundedQueue()
{
    delete[] data_;
}

template <typename T>
void BoundedQueue<T>::Push(const T& value)
{
    if (size_ < capacity_)
    {
        data_[TailIndex()] = value;
        ++size_;
    }
    else
    {
        data_[head_] = value;
        head_ = (head_ + 1) % capacity_;
    }
}

template <typename T>
const T& BoundedQueue<T>::Front() const
{
    if (size_ == 0)
        throw std::out_of_range("BoundedQueue::Front: очередь пуста");
    return data_[head_];
}

template <typename T>
const T& BoundedQueue<T>::Back() const
{
    if (size_ == 0)
        throw std::out_of_range("BoundedQueue::Back: очередь пуста");
    return data_[(head_ + size_ - 1) % capacity_];
}

template <typename T>
const T& BoundedQueue<T>::At(size_t i) const
{
    if (i >= size_)
        throw std::out_of_range("BoundedQueue::At: индекс вне диапазона");
    return data_[(head_ + i) % capacity_];
}

#endif
