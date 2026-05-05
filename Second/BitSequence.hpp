#ifndef BIT_SEQUENCE_HPP
#define BIT_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"

class BitSequence: public Sequence <int>  
{
    private:
    DynamicArray <int>* bits;  

    public:
    Sequence<int>* instance()  
    {
        return this;
    }

    BitSequence ()
    {
        bits = new DynamicArray<int> ();
    }

    BitSequence (unsigned a)
    {
        int size = 0;
        int t = a;

        while (t > 0)
        {
            size++;
            t = t / 2;
        }

        if (size == 0) size = 1;  // Для случая a = 0

        int* b = new int[size];  // Вместо bool*
        for (int i = size-1; i >= 0; i--)
        {
            b[i] = a % 2;
            a /= 2;
        }

        bits = new DynamicArray<int> (b, size);
        delete[] b;  // Не забываем удалять!
    }

    BitSequence (int* items, int size)  // Вместо bool*
    {
        bits = new DynamicArray<int> (items, size);
    }

    BitSequence (const BitSequence & rhs)
    {
        bits = new DynamicArray<int> (*rhs.bits);
    }

    int GetFirst ()  // Вместо bool
    {
        if (bits->GetSize() == 0)
        {      
            throw invalid_argument("Последовательность пуста");
        }
        else 
        {
            return bits->Get(0);
        }
    }

    int GetLast ()  // Вместо bool
    {
        if (bits->GetSize() == 0)
        {      
            throw invalid_argument("Последовательность пуста");
        }
        else 
        {
            return bits->Get(bits->GetSize()-1);
        }
    }

    int Get (int index)  // Вместо bool
    {
        if (index >= 0 && index < bits->GetSize())
        {
            return bits->Get(index);
        }
        else
        {
            throw invalid_argument("Индекс выходит за границы последовательности");
        }
    }

    BitSequence* GetSubsequence (int start, int end) 
    {
        if ((start < 0) || (end > bits->GetSize()-1) || (start > end))
        {
            throw invalid_argument("Неверные границы подпоследовательности");
        }
        else 
        {
            int* subseq = new int[end - start + 1];  // Вместо bool*
            for (int i = 0; i < (end - start + 1); i++)
                subseq[i] = bits->Get(start + i);

            BitSequence* result = new BitSequence (subseq, (end - start + 1));
            delete [] subseq;
            return result;
        }
    }

    int GetLength() 
    {
        return bits->GetSize();
    }

    BitSequence* AppendImpl (const int& item)  
    {
        if (item != 0 && item != 1) 
        {
            throw invalid_argument("Бит может быть только 0 или 1");
        }
        bits->Append(item);
        return this;
    }

    BitSequence* InsertAtImpl(const int& item, int index)  // Вместо const bool&
    {
        if (item != 0 && item != 1) {
            throw invalid_argument("Бит может быть только 0 или 1");
        }
        bits->InsertAt(item, index);
        return this;
    }

    BitSequence* PrependImpl(const int& item)  // Вместо const bool&
    {
        if (item != 0 && item != 1) {
            throw invalid_argument("Бит может быть только 0 или 1");
        }
        bits->Prepend(item);
        return this;
    }

    BitSequence* Concat (Sequence <int>* rhs)  // Вместо Sequence<bool>
    {
        if (rhs == nullptr)
        {      
            throw invalid_argument("Последовательность не может быть пустой");
        } 
        else 
        {
            BitSequence* result = new BitSequence (*this);
            for (int i = 0; i < rhs->GetLength(); i++)
                result->AppendImpl(rhs->Get(i));

            return result;
        }
    }

    BitSequence* BitAnd (const BitSequence* rhs)
    {
        if (bits->GetSize() != rhs->bits->GetSize()) 
            throw invalid_argument("Размеры последовательностей не совпадают");
        
        BitSequence* result = new BitSequence();
        for (int i = 0; i < bits->GetSize(); i++)
            result->AppendImpl(bits->Get(i) & rhs->bits->Get(i));
        return result;
    }

    BitSequence* BitOr (const BitSequence* rhs)
    {
        if (bits->GetSize() != rhs->bits->GetSize()) 
            throw invalid_argument("Размеры последовательностей не совпадают");
        
        BitSequence* result = new BitSequence();
        for (int i = 0; i < bits->GetSize(); i++)
            result->AppendImpl(bits->Get(i) | rhs->bits->Get(i));  // | вместо ||
        return result;
    }

    BitSequence* BitXor (const BitSequence* rhs)
    {
        if (bits->GetSize() != rhs->bits->GetSize()) 
            throw invalid_argument("Размеры последовательностей не совпадают");
        
        BitSequence* result = new BitSequence();
        for (int i = 0; i < bits->GetSize(); i++)
            result->AppendImpl(bits->Get(i) ^ rhs->bits->Get(i));
        return result;
    }

    BitSequence* BitNot ()
    {
        BitSequence* result = new BitSequence();
        for (int i = 0; i < bits->GetSize(); i++)
            result->AppendImpl(bits->Get(i) ^ 1);  // Инверсия
        return result;
    }

    void Print ()
    {
        if (bits->GetSize() > 0)
        {
            for (int i = 0; i < bits->GetSize(); i++)
                cout << bits->Get(i);  // Убрал (int), т.к. теперь и так int
            cout << endl; 
        }
        else
        {
            throw invalid_argument("Последовательность пуста");
        }
    }
};

#endif