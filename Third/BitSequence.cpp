#include "BitSequence.hpp"
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <cmath>
#include <iostream>

Sequence<int>* BitSequence::instance()
{
    return new BitSequence(*this);
}

BitSequence* BitSequence::AppendImpl(int item)
{
    if (item != 0 && item != 1)
    {
        throw InvalidBitException("Бит может быть только 0 или 1");
    }
    bits->Append(item);
    return this;
}

BitSequence* BitSequence::InsertAtImpl(int item, size_t index)
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

BitSequence* BitSequence::PrependImpl(int item)
{
    if (item != 0 && item != 1)
    {
        throw InvalidBitException("Бит может быть только 0 или 1");
    }
    bits->Prepend(item);
    return this;
}

BitSequence::BitSequence() : bits(new DynamicArray<int>()) {}

BitSequence::BitSequence(unsigned int a) : bits(new DynamicArray<int>())
{
    if (a == 0)
    {
        bits->Append(0);
        return;
    }
    
    unsigned int temp = a;
    DynamicArray<int> reverseBits;
    
    while (temp > 0)
    {
        reverseBits.Prepend(temp % 2);  
        temp /= 2;
    }
    
    for (size_t i = 0; i < reverseBits.GetSize(); i++)
    {
        bits->Append(reverseBits.Get(i));
    }
}

BitSequence::BitSequence(int* items, size_t size) : bits(new DynamicArray<int>())
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

BitSequence::BitSequence(const BitSequence& rhs) : bits(new DynamicArray<int>(*rhs.bits)) {}

BitSequence::BitSequence(std::initializer_list<int> list) : bits(new DynamicArray<int>())
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

BitSequence::~BitSequence()
{
    delete bits;
}

int BitSequence::GetFirst()
{
    if (bits->GetSize() == 0)
    {
        throw SequenceEmptyException("BitSequence пуста - невозможно получить первый элемент");
    }
    return bits->Get(0);
}

int BitSequence::GetLast()
{
    if (bits->GetSize() == 0)
    {
        throw SequenceEmptyException("BitSequence пуста - невозможно получить последний элемент");
    }
    return bits->Get(bits->GetSize() - 1);
}

int BitSequence::Get(size_t index)
{
    if (index >= bits->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в BitSequence::Get");
    }
    return bits->Get(index);
}

BitSequence* BitSequence::GetSubsequence(size_t start, size_t end)
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

size_t BitSequence::GetLength()
{
    return bits->GetSize();
}

Sequence<int>* BitSequence::Concat(Sequence<int>* list)
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

BitSequence* BitSequence::BitAnd(const BitSequence* rhs)
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

BitSequence* BitSequence::BitOr(const BitSequence* rhs)
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

BitSequence* BitSequence::BitXor(const BitSequence* rhs)
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

BitSequence* BitSequence::BitNot()
{
    BitSequence* result = new BitSequence();
    for (size_t i = 0; i < bits->GetSize(); i++)
    {
        result->bits->Append(bits->Get(i) ^ 1);
    }
    return result;
}

void BitSequence::Print()
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