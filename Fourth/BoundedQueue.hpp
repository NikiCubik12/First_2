#ifndef BOUNDED_QUEUE_HPP
#define BOUNDED_QUEUE_HPP

#include <cstddef>
#include <stdexcept>

template <typename T>
class BoundedQueue
{
private:
    T*     data_;
    size_t capacity_;
    size_t size_;
    size_t head_;

    size_t TailIndex() const { return (head_ + size_) % capacity_; }

public:
    explicit BoundedQueue(size_t capacity);
    BoundedQueue(const BoundedQueue<T>& other);
    BoundedQueue<T>& operator=(const BoundedQueue<T>& other);
    ~BoundedQueue();

    size_t Size()     const { return size_; }
    size_t Capacity() const { return capacity_; }
    bool   IsFull()   const { return size_ == capacity_; }
    bool   IsEmpty()  const { return size_ == 0; }

    void Push(const T& value);

    const T& Front() const;
    const T& Back()  const;
    const T& At(size_t i) const;
};

#include "BoundedQueue.tpp"

#endif
