#ifndef BOUNDED_QUEUE_HPP
#define BOUNDED_QUEUE_HPP

#include <cstddef>
#include <stdexcept>
#include "DynamicArray.hpp"

template <typename T>
class BoundedQueue
{
private:
    DynamicArray<T> data_;

public:
    BoundedQueue() : data_() {}
    explicit BoundedQueue(size_t /*capacity*/) : data_() {}

    size_t Size()     const { return data_.GetSize(); }
    bool   IsEmpty()  const { return data_.GetSize() == 0; }
    bool   IsFull()   const { return false; }

    void Push(const T& value)
    {
        data_.Append(value);
    }

    const T& Front() const
    {
        if (data_.GetSize() == 0)
            throw std::out_of_range("BoundedQueue::Front: очередь пуста");
        return data_.GetRef(0);
    }

    const T& Back() const
    {
        if (data_.GetSize() == 0)
            throw std::out_of_range("BoundedQueue::Back: очередь пуста");
        return data_.GetRef(data_.GetSize() - 1);
    }

    const T& At(size_t i) const
    {
        if (i >= data_.GetSize())
            throw std::out_of_range("BoundedQueue::At: индекс вне диапазона");
        return data_.GetRef(i);
    }
};

#endif