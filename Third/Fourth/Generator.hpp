#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <vector>
#include "BoundedQueue.hpp"
#include "Sequence.hpp"
#include "Option.hpp"

template <typename T>
class Generator
{
public:
    using Rule = std::function<T(const BoundedQueue<T>&)>;

private:
    struct Modification
    {
        size_t pos;
        bool   isInsert;
        T      value;
    };

    Rule                       rule_;
    size_t                     window_;
    std::vector<T>             initials_;

    BoundedQueue<T>*           history_;
    size_t                     emitted_;
    size_t                     baseEmitted_;

    bool                       hasBound_;
    size_t                     maxCount_;

    std::vector<Modification>  mods_;
    size_t                     modIdx_;

    void AllocateHistory();
    void ResetState();
    T    ProduceBase();
    void InsertMod(const Modification& m);

    static std::vector<T> FromSequence(Sequence<T>& seq);

public:
    Generator(const Rule& rule,
              Sequence<T>& initials,
              size_t window);

    Generator(const Rule& rule,
              Sequence<T>& initials,
              size_t window,
              size_t maxCount);

    Generator(const Generator<T>& other);
    Generator<T>& operator=(const Generator<T>& other);
    ~Generator();

    T            GetNext();
    bool         HasNext() const;
    Optional<T>  TryGetNext();
    void         Reset();

    size_t       GetEmittedCount() const { return emitted_; }
    bool         HasBound() const { return hasBound_; }

    void InsertAt(size_t pos, const T& value);
    void RemoveAt(size_t pos);
    void InsertSubsequence(size_t pos, Sequence<T>& seq);
};

#include "Generator.tpp"

#endif
