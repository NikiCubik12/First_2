#ifndef BIT_SEQUENCE_TPP
#define BIT_SEQUENCE_TPP

#include <iostream>

inline Sequence<int>* BitSequence::instance()
{
    return new BitSequence(*this);
}

inline BitSequence* BitSequence::AppendImpl(const int& item)
{
    if (item != 0 && item != 1)
    {
        throw InvalidBitException("Бит может быть только 0 или 1");
    }
    bits->Append(item);
    return this;
}

inline BitSequence* BitSequence::InsertAtImpl(const int& item, size_t index)
{
    if (item != 0 && item != 1)
    {
        throw InvalidBitException("Бит может быть только 0 или 1");
    }
    if (index > bits->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в BitSequence::InsertAtImpl");
    }
    bits->InsertAt(item, index);
    return this;
}

inline BitSequence* BitSequence::PrependImpl(const int& item)
{
    if (item != 0 && item != 1)
    {
        throw InvalidBitException("Бит может быть только 0 или 1");
    }
    bits->Prepend(item);
    return this;
}

inline BitSequence::BitSequence() : bits(new DynamicArray<int>()) {}

inline BitSequence::BitSequence(unsigned int a) : bits(new DynamicArray<int>())
{
    if (a == 0)
    {
        bits->Append(0);
        return;
    }
    
    size_t size = static_cast<size_t>(log2(a)) + 1;
    unsigned int temp = a;
    
    int* tempBits = new int[size];
    for (size_t i = size; i-- > 0; )
    {
        tempBits[i] = temp % 2;
        temp /= 2;
    }
    
    for (size_t i = 0; i < size; i++)
    {
        bits->Append(tempBits[i]);
    }
    
    delete[] tempBits;
}

inline BitSequence::BitSequence(int* items, size_t size) : bits(new DynamicArray<int>())
{
    for (size_t i = 0; i < size; i++)
    {
        if (items[i] != 0 && items[i] != 1)
        {
            delete bits;
            throw InvalidBitException("Бит может быть только 0 или 1");
        }
        bits->Append(items[i]);
    }
}

inline BitSequence::BitSequence(const BitSequence& rhs) : bits(new DynamicArray<int>(*rhs.bits)) {}

inline BitSequence::BitSequence(std::initializer_list<int> list) : bits(new DynamicArray<int>())
{
    for (int val : list)
    {
        if (val != 0 && val != 1)
        {
            delete bits;
            throw InvalidBitException("Бит может быть только 0 или 1");
        }
        bits->Append(val);
    }
}

inline BitSequence::~BitSequence()
{
    delete bits;
}

inline int BitSequence::GetFirst()
{
    if (bits->GetSize() == 0)
    {
        throw SequenceEmptyException("BitSequence пуста - невозможно получить первый элемент");
    }
    return bits->Get(0);
}

inline int BitSequence::GetLast()
{
    if (bits->GetSize() == 0)
    {
        throw SequenceEmptyException("BitSequence пуста - невозможно получить последний элемент");
    }
    return bits->Get(bits->GetSize() - 1);
}

inline int BitSequence::Get(size_t index)
{
    if (index >= bits->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в BitSequence::Get");
    }
    return bits->Get(index);
}

inline BitSequence* BitSequence::GetSubsequence(size_t start, size_t end)
{
    size_t size = bits->GetSize();
    if (start > end || end >= size)
    {
        throw IndexOutOfRangeException("Неверные начальный или конечный индексы в BitSequence::GetSubsequence");
    }
    
    size_t length = end - start + 1;
    int* subseq = new int[length];
    for (size_t i = 0; i < length; i++)
    {
        subseq[i] = bits->Get(start + i);
    }
    
    BitSequence* result = new BitSequence(subseq, length);
    delete[] subseq;
    return result;
}

inline size_t BitSequence::GetLength()
{
    return bits->GetSize();
}

inline Sequence<int>* BitSequence::Concat(Sequence<int>* list)
{
    if (list == nullptr)
    {
        throw NullPointerException("Невозможно объединить с нулевым указателем в BitSequence::Concat");
    }
    
    BitSequence* result = new BitSequence(*this);
    for (size_t i = 0; i < list->GetLength(); i++)
    {
        int val = list->Get(i);
        if (val != 0 && val != 1)
        {
            delete result;
            throw InvalidBitException("Бит может быть только 0 или 1");
        }
        result->bits->Append(val);
    }
    return result;
}

inline BitSequence* BitSequence::BitAnd(const BitSequence* rhs)
{
    if (bits->GetSize() != rhs->bits->GetSize())
    {
        throw DifferentSizeException("Размеры последовательностей не совпадают для операции AND");
    }
    
    BitSequence* result = new BitSequence();
    for (size_t i = 0; i < bits->GetSize(); i++)
    {
        result->bits->Append(bits->Get(i) & rhs->bits->Get(i));
    }
    return result;
}

inline BitSequence* BitSequence::BitOr(const BitSequence* rhs)
{
    if (bits->GetSize() != rhs->bits->GetSize())
    {
        throw DifferentSizeException("Размеры последовательностей не совпадают для операции OR");
    }
    
    BitSequence* result = new BitSequence();
    for (size_t i = 0; i < bits->GetSize(); i++)
    {
        result->bits->Append(bits->Get(i) | rhs->bits->Get(i));
    }
    return result;
}

inline BitSequence* BitSequence::BitXor(const BitSequence* rhs)
{
    if (bits->GetSize() != rhs->bits->GetSize())
    {
        throw DifferentSizeException("Размеры последовательностей не совпадают для операции XOR");
    }
    
    BitSequence* result = new BitSequence();
    for (size_t i = 0; i < bits->GetSize(); i++)
    {
        result->bits->Append(bits->Get(i) ^ rhs->bits->Get(i));
    }
    return result;
}

inline BitSequence* BitSequence::BitNot()
{
    BitSequence* result = new BitSequence();
    for (size_t i = 0; i < bits->GetSize(); i++)
    {
        result->bits->Append(bits->Get(i) ^ 1);
    }
    return result;
}

inline void BitSequence::Print()
{
    if (bits->GetSize() == 0)
    {
        throw SequenceEmptyException("BitSequence пуста - невозможно вывести");
    }
    for (size_t i = 0; i < bits->GetSize(); i++)
    {
        std::cout << bits->Get(i);
    }
    std::cout << std::endl;
}

#endif