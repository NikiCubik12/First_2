#ifndef LAZY_SEQUENCE_TPP
#define LAZY_SEQUENCE_TPP

#include "LazySequence.hpp"
#include "MutableArraySequence.hpp"
#include "SimpleVector.hpp"

template <typename T>
LazySequence<T>::LazySequence()
    : producer_(),
      cache_(),
      exhausted_(true),
      isInfinite_(false)
{}

template <typename T>
LazySequence<T>::LazySequence(Sequence<T>& src)
    : producer_(),
      cache_(),
      exhausted_(true),
      isInfinite_(false)
{
    for (size_t i = 0; i < src.GetLength(); ++i)
        cache_.push_back(src.Get(i));
}

template <typename T>
LazySequence<T>::LazySequence(Generator<T>& gen)
    : producer_(),
      cache_(),
      exhausted_(false),
      isInfinite_(!gen.HasBound())
{
    auto shared = std::make_shared<Generator<T>>(gen);
    producer_ = [shared]() { return shared->TryGetNext(); };
}

template <typename T>
LazySequence<T>::LazySequence(const LazySequence<T>& other)
    : producer_(other.producer_),
      cache_(other.cache_),
      exhausted_(other.exhausted_),
      isInfinite_(other.isInfinite_)
{}

template <typename T>
LazySequence<T>& LazySequence<T>::operator=(const LazySequence<T>& other)
{
    if (this == &other) return *this;
    producer_   = other.producer_;
    cache_      = other.cache_;
    exhausted_  = other.exhausted_;
    isInfinite_ = other.isInfinite_;
    return *this;
}

template <typename T>
LazySequence<T>::~LazySequence() = default;

template <typename T>
void LazySequence<T>::PullOne()
{
    if (exhausted_ || !producer_) return;
    Optional<T> next = producer_();
    if (next.HasValue())
        cache_.push_back(next.GetValue());
    else
        exhausted_ = true;
}

template <typename T>
void LazySequence<T>::MaterializeUpTo(size_t n)
{
    while (!exhausted_ && cache_.size() < n)
        PullOne();
}

template <typename T>
void LazySequence<T>::MaterializeAll()
{
    if (isInfinite_)
        throw std::runtime_error("LazySequence::MaterializeAll: бесконечная последовательность");
    while (!exhausted_)
        PullOne();
}

template <typename T>
T LazySequence<T>::GetFirst()
{
    MaterializeUpTo(1);
    if (cache_.empty())
        throw SequenceEmptyException("LazySequence: пустая последовательность");
    return cache_.front();
}

template <typename T>
T LazySequence<T>::GetLast()
{
    MaterializeAll();
    if (cache_.empty())
        throw SequenceEmptyException("LazySequence: пустая последовательность");
    return cache_.back();
}

template <typename T>
T LazySequence<T>::Get(size_t index)
{
    MaterializeUpTo(index + 1);
    if (index >= cache_.size())
        throw IndexOutOfRangeException("LazySequence: индекс вне диапазона");
    return cache_[index];
}

template <typename T>
size_t LazySequence<T>::GetLength()
{
    MaterializeAll();
    return cache_.size();
}

template <typename T>
Cardinal LazySequence<T>::GetCardinality()
{
    if (isInfinite_) return Cardinal::Infinity();
    MaterializeAll();
    return Cardinal::Finite(cache_.size());
}

template <typename T>
Sequence<T>* LazySequence<T>::GetSubsequence(size_t start, size_t end)
{
    if (end < start)
        throw IndexOutOfRangeException("LazySequence::GetSubsequence: end < start");
    MaterializeUpTo(end + 1);
    if (end >= cache_.size())
        throw IndexOutOfRangeException("LazySequence::GetSubsequence: end вне диапазона");

    LazySequence<T>* result = new LazySequence<T>();
    for (size_t i = start; i <= end; ++i)
        result->cache_.push_back(cache_[i]);
    return result;
}

template <typename T>
Sequence<T>* LazySequence<T>::instance()
{
    return new LazySequence<T>(*this);
}

template <typename T>
Sequence<T>* LazySequence<T>::AppendImpl(const T& item)
{
    MaterializeAll();
    cache_.push_back(item);
    return this;
}

template <typename T>
Sequence<T>* LazySequence<T>::PrependImpl(const T& item)
{
    MaterializeAll();
    cache_.prepend(item);
    return this;
}

template <typename T>
Sequence<T>* LazySequence<T>::InsertAtImpl(const T& item, size_t index)
{
    MaterializeAll();
    if (index > cache_.size())
        throw IndexOutOfRangeException("LazySequence::InsertAtImpl: индекс вне диапазона");
    cache_.insert_at(item, index);
    return this;
}

template <typename T>
Sequence<T>* LazySequence<T>::Concat(Sequence<T>* list)
{
    if (!list)
        throw NullPointerException("LazySequence::Concat: nullptr");
    MaterializeAll();
    LazySequence<T>* result = new LazySequence<T>(*this);
    for (size_t i = 0; i < list->GetLength(); ++i)
        result->cache_.push_back(list->Get(i));
    return result;
}

template <typename T>
template <typename U>
LazySequence<U>* LazySequence<T>::Map(std::function<U(T)> f)
{
    LazySequence<U>* result = new LazySequence<U>();
    result->cache_.clear();
    result->isInfinite_ = isInfinite_;

    auto srcCache = std::make_shared<SimpleVector<T>>();
    *srcCache = cache_;
    auto srcProducer = producer_;
    auto srcExhausted = std::make_shared<bool>(exhausted_);
    auto idx = std::make_shared<size_t>(0);

    result->producer_ = [srcCache, srcProducer, srcExhausted, idx, f]() -> Optional<U>
    {
        if (*idx < srcCache->size())
        {
            U v = f((*srcCache)[*idx]);
            ++(*idx);
            return Optional<U>::Some(v);
        }
        if (*srcExhausted || !srcProducer)
            return Optional<U>::None();
        Optional<T> nxt = srcProducer();
        if (!nxt.HasValue())
        {
            *srcExhausted = true;
            return Optional<U>::None();
        }
        srcCache->push_back(nxt.GetValue());
        U v = f(nxt.GetValue());
        ++(*idx);
        return Optional<U>::Some(v);
    };
    result->exhausted_ = false;
    if (!isInfinite_ && exhausted_ && cache_.empty())
        result->exhausted_ = true;
    return result;
}

template <typename T>
LazySequence<T>* LazySequence<T>::Where(std::function<bool(T)> pred)
{
    LazySequence<T>* result = new LazySequence<T>();
    result->isInfinite_ = isInfinite_;

    auto srcCache = std::make_shared<SimpleVector<T>>();
    *srcCache = cache_;
    auto srcProducer = producer_;
    auto srcExhausted = std::make_shared<bool>(exhausted_);
    auto idx = std::make_shared<size_t>(0);

    result->producer_ = [srcCache, srcProducer, srcExhausted, idx, pred]() -> Optional<T>
    {
        while (true)
        {
            if (*idx < srcCache->size())
            {
                T v = (*srcCache)[*idx];
                ++(*idx);
                if (pred(v)) return Optional<T>::Some(v);
                continue;
            }
            if (*srcExhausted || !srcProducer)
                return Optional<T>::None();
            Optional<T> nxt = srcProducer();
            if (!nxt.HasValue())
            {
                *srcExhausted = true;
                return Optional<T>::None();
            }
            srcCache->push_back(nxt.GetValue());
            ++(*idx);
            if (pred(nxt.GetValue())) return Optional<T>::Some(nxt.GetValue());
        }
    };
    result->exhausted_ = false;
    if (!isInfinite_ && exhausted_ && cache_.empty())
        result->exhausted_ = true;
    return result;
}

template <typename T>
template <typename U>
LazySequence<std::pair<T, U>>* LazySequence<T>::Zip(LazySequence<U>& other)
{
    LazySequence<std::pair<T, U>>* result = new LazySequence<std::pair<T, U>>();
    result->isInfinite_ = isInfinite_ && other.IsInfinite();

    auto aCache = std::make_shared<SimpleVector<T>>();   *aCache = cache_;
    auto bCache = std::make_shared<SimpleVector<U>>();   *bCache = other.cache_;
    auto aProducer = producer_;
    auto bProducer = other.producer_;
    auto aExhausted = std::make_shared<bool>(exhausted_);
    auto bExhausted = std::make_shared<bool>(other.exhausted_);
    auto idx = std::make_shared<size_t>(0);

    auto pullA = [aCache, aProducer, aExhausted](size_t need) -> bool
    {
        while (aCache->size() < need && !*aExhausted && aProducer)
        {
            Optional<T> n = aProducer();
            if (!n.HasValue()) { *aExhausted = true; return false; }
            aCache->push_back(n.GetValue());
        }
        return aCache->size() >= need;
    };
    auto pullB = [bCache, bProducer, bExhausted](size_t need) -> bool
    {
        while (bCache->size() < need && !*bExhausted && bProducer)
        {
            Optional<U> n = bProducer();
            if (!n.HasValue()) { *bExhausted = true; return false; }
            bCache->push_back(n.GetValue());
        }
        return bCache->size() >= need;
    };

    result->producer_ = [aCache, bCache, idx, pullA, pullB]() -> Optional<std::pair<T, U>>
    {
        if (!pullA(*idx + 1)) return Optional<std::pair<T, U>>::None();
        if (!pullB(*idx + 1)) return Optional<std::pair<T, U>>::None();
        std::pair<T, U> p((*aCache)[*idx], (*bCache)[*idx]);
        ++(*idx);
        return Optional<std::pair<T, U>>::Some(p);
    };
    result->exhausted_ = false;
    return result;
}

template <typename T>
T LazySequence<T>::Reduce(std::function<T(T, T)> f, T init)
{
    MaterializeAll();
    T acc = init;
    for (size_t i = 0; i < cache_.size(); ++i)
        acc = f(acc, cache_[i]);
    return acc;
}

#endif