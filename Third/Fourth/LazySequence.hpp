#ifndef LAZY_SEQUENCE_HPP
#define LAZY_SEQUENCE_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <vector>
#include <utility>
#include "Sequence.hpp"
#include "Option.hpp"
#include "Cardinal.hpp"
#include "Generator.hpp"

template <typename T>
class LazySequence : public Sequence<T>
{
    template <typename U> friend class LazySequence;

private:
    std::function<Optional<T>()>  producer_;
    std::vector<T>                cache_;
    bool                          exhausted_;
    bool                          isInfinite_;

    void PullOne();
    void MaterializeUpTo(size_t n);
    void MaterializeAll();

protected:
    Sequence<T>* instance()         override;
    Sequence<T>* AppendImpl (const T& item)  override;
    Sequence<T>* PrependImpl(const T& item)  override;
    Sequence<T>* InsertAtImpl(const T& item, size_t index) override;

public:
    LazySequence();
    explicit LazySequence(Sequence<T>& src);
    explicit LazySequence(Generator<T>& gen);
    LazySequence(const LazySequence<T>& other);
    LazySequence<T>& operator=(const LazySequence<T>& other);
    ~LazySequence();

    T            GetFirst()                       override;
    T            GetLast()                        override;
    T            Get(size_t index)                override;
    Sequence<T>* GetSubsequence(size_t start, size_t end) override;
    size_t       GetLength()                      override;
    Sequence<T>* Concat(Sequence<T>* list)        override;

    Cardinal     GetCardinality();
    size_t       GetMaterializedCount() const { return cache_.size(); }
    bool         IsInfinite()           const { return isInfinite_; }
    bool         IsExhausted()          const { return exhausted_; }

    template <typename U>
    LazySequence<U>* Map(std::function<U(T)> f);

    LazySequence<T>* Where(std::function<bool(T)> pred);

    template <typename U>
    LazySequence<std::pair<T, U>>* Zip(LazySequence<U>& other);

    T Reduce(std::function<T(T, T)> f, T init);
};

#include "LazySequence.tpp"

#endif
