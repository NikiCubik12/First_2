#ifndef BIT_SEQUENCE_HPP
#define BIT_SEQUENCE_HPP

#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <cmath>
#include <cstddef>

class BitSequence : public Sequence<size_t>
{
    private:
    DynamicArray<size_t>* bits;

    protected:
    Sequence<size_t>* instance() override;
    BitSequence* AppendImpl(size_t item) override;
    BitSequence* InsertAtImpl(size_t item, size_t index) override;
    BitSequence* PrependImpl(size_t item) override;

    public:
    BitSequence();
    BitSequence(size_t a);
    BitSequence(size_t* items, size_t size);
    BitSequence(const BitSequence& rhs);
    BitSequence(std::initializer_list<size_t> list);
    ~BitSequence();

    size_t GetFirst() override;
    size_t GetLast() override;
    size_t Get(size_t index) override;
    BitSequence* GetSubsequence(size_t start, size_t end) override;
    size_t GetLength() override;
    Sequence<size_t>* Concat(Sequence<size_t>* list) override;

    BitSequence* BitAnd(BitSequence* rhs);
    BitSequence* BitOr(BitSequence* rhs);
    BitSequence* BitXor(BitSequence* rhs);
    BitSequence* BitNot();
    void Print();
};

#endif