#include "BitSequence.hpp"
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <cmath>
#include <iostream>
#include <clocale>

Sequence<size_t>* BitSequence::instance()
{
    setlocale(LC_ALL, "Rus");
    return new BitSequence(*this);
}

BitSequence* BitSequence::AppendImpl(size_t item)
{
    setlocale(LC_ALL, "Rus");
    if (item != 0 && item != 1)
    {
        throw InvalidBitException("Бит может быть только 0 или 1");
    }
    bits->Append(item);
    return this;
}

BitSequence* BitSequence::InsertAtImpl(size_t item, size_t index)
{
    setlocale(LC_ALL, "Rus");
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

BitSequence* BitSequence::PrependImpl(size_t item)
{
    setlocale(LC_ALL, "Rus");
    if (item != 0 && item != 1)
    {
        throw InvalidBitException("Бит может быть только 0 или 1");
    }
    bits->Prepend(item);
    return this;
}

BitSequence::BitSequence() : bits(new DynamicArray<size_t>()) {}

BitSequence::BitSequence(size_t a) : bits(new DynamicArray<size_t>())
{
    if (a == 0)
    {
        bits->Append(0);
        return;
    }
    
    size_t temp = a;
    DynamicArray<size_t> reverseBits;
    
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

BitSequence::BitSequence(size_t* items, size_t size) : bits(new DynamicArray<size_t>())
{
    setlocale(LC_ALL, "Rus");
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

BitSequence::BitSequence(BitSequence& rhs) : bits(new DynamicArray<size_t>(*rhs.bits)) {}

BitSequence::BitSequence(std::initializer_list<size_t> list) : bits(new DynamicArray<size_t>())
{
    setlocale(LC_ALL, "Rus");
    for (size_t val : list)
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

size_t BitSequence::GetFirst()
{
    setlocale(LC_ALL, "Rus");
    if (bits->GetSize() == 0)
    {
        throw SequenceEmptyException("BitSequence пуста - невозможно получить первый элемент");
    }
    return bits->Get(0);
}

size_t BitSequence::GetLast()
{
    setlocale(LC_ALL, "Rus");
    if (bits->GetSize() == 0)
    {
        throw SequenceEmptyException("BitSequence пуста - невозможно получить последний элемент");
    }
    return bits->Get(bits->GetSize() - 1);
}

size_t BitSequence::Get(size_t index)
{
    setlocale(LC_ALL, "Rus");
    if (index >= bits->GetSize())
    {
        throw IndexOutOfRangeException("Индекс выходит за границы в BitSequence::Get");
    }
    return bits->Get(index);
}

BitSequence* BitSequence::GetSubsequence(size_t start, size_t end)
{
    setlocale(LC_ALL, "Rus");
    size_t size = bits->GetSize();
    if (start > end || end >= size)
    {
        throw IndexOutOfRangeException("Неверные начальный или конечный индексы в BitSequence::GetSubsequence");
    }
    
    size_t length = end - start + 1;
    size_t* subseq = new size_t[length];
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

Sequence<size_t>* BitSequence::Concat(Sequence<size_t>* list)
{
    setlocale(LC_ALL, "Rus");
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

BitSequence* BitSequence::BitAnd(BitSequence* rhs)
{
    setlocale(LC_ALL, "Rus");
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

BitSequence* BitSequence::BitOr(BitSequence* rhs)
{
    setlocale(LC_ALL, "Rus");
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

BitSequence* BitSequence::BitXor(BitSequence* rhs)
{
    setlocale(LC_ALL, "Rus");
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
    setlocale(LC_ALL, "Rus");
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