#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstddef>

class SequenceException : public std::exception
{
    protected:
    std::string message;
    
    public:
    explicit SequenceException(const std::string& msg);
    const char* what() const noexcept override;
};

class IndexOutOfRangeException : public SequenceException
{
    public:
    explicit IndexOutOfRangeException(const std::string& msg);
};

class SequenceEmptyException : public SequenceException
{
    public:
    explicit SequenceEmptyException(const std::string& msg);
};

class NullPointerException : public SequenceException
{
    public:
    explicit NullPointerException(const std::string& msg);
};

class InvalidBitException : public SequenceException
{
    public:
    explicit InvalidBitException(const std::string& msg);
};

class DifferentSizeException : public SequenceException
{
    public:
    explicit DifferentSizeException(const std::string& msg);
};

template <class T> class Sequence
{
    public:
    virtual ~Sequence() = default;
    
    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(size_t index) = 0;
    virtual Sequence<T>* GetSubsequence(size_t start, size_t end) = 0;
    virtual size_t GetLength() = 0;

    Sequence<T>* Append(T item);
    Sequence<T>* Prepend(T item);
    Sequence<T>* InsertAt(T item, size_t index);
    virtual Sequence<T>* Concat(Sequence<T>* list) = 0;
    
    protected:
    virtual Sequence<T>* AppendImpl(const T& item) = 0;
    virtual Sequence<T>* InsertAtImpl(const T& item, size_t index) = 0;
    virtual Sequence<T>* PrependImpl(const T& item) = 0;
    virtual Sequence<T>* instance() = 0;
};

#include "Sequence.tpp"

#endif