#ifndef SIMPLE_VECTOR_HPP
#define SIMPLE_VECTOR_HPP

#include <cstddef>
#include "DynamicArray.hpp"

template <class T>
class SimpleVector
{
private:
    DynamicArray<T> data_;
public:
    SimpleVector() = default;
    SimpleVector(const SimpleVector&) = default;
    SimpleVector& operator=(const SimpleVector&) = default;

    void   push_back(const T& v) { data_.Append(v); }
    size_t size()  const         { return data_.GetSize(); }
    bool   empty() const         { return data_.GetSize() == 0; }
    void   clear()               { data_ = DynamicArray<T>(); }

    T&       operator[](size_t i)       { return data_.GetRef(i); }
    const T& operator[](size_t i) const { return data_.GetRef(i); }

    T&       front()       { return data_.GetRef(0); }
    const T& front() const { return data_.GetRef(0); }
    T&       back()        { return data_.GetRef(data_.GetSize() - 1); }
    const T& back()  const { return data_.GetRef(data_.GetSize() - 1); }

    void prepend(const T& v)             { data_.Prepend(v); }
    void insert_at(const T& v, size_t i) { data_.InsertAt(v, i); }
};

#endif
