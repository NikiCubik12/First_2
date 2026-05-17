#ifndef BIT_SEQUENCE_HPP
#define BIT_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <cmath>
#include <cstddef>

class BitSequence : public Sequence<int>
{
    private:
    DynamicArray<int>* bits;

    protected:
    Sequence<int>* instance() override;
    BitSequence* AppendImpl(const int& item) override;
    BitSequence* InsertAtImpl(const int& item, size_t index) override;
    BitSequence* PrependImpl(const int& item) override;

    public:
    BitSequence();
    BitSequence(unsigned int a);
    BitSequence(int* items, size_t size);
    BitSequence(const BitSequence& rhs);
    BitSequence(std::initializer_list<int> list);
    ~BitSequence();

    int GetFirst() override;
    int GetLast() override;
    int Get(size_t index) override;
    BitSequence* GetSubsequence(size_t start, size_t end) override;
    size_t GetLength() override;
    Sequence<int>* Concat(Sequence<int>* list) override;

    BitSequence* BitAnd(const BitSequence* rhs);
    BitSequence* BitOr(const BitSequence* rhs);
    BitSequence* BitXor(const BitSequence* rhs);
    BitSequence* BitNot();
    void Print();
};

#endif