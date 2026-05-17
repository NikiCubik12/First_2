#ifndef GENERATOR_TPP
#define GENERATOR_TPP

#include "Generator.hpp"

template <typename T>
std::vector<T> Generator<T>::FromSequence(Sequence<T>& seq)
{
    std::vector<T> v;
    v.reserve(seq.GetLength());
    for (size_t i = 0; i < seq.GetLength(); ++i)
        v.push_back(seq.Get(i));
    return v;
}

template <typename T>
void Generator<T>::AllocateHistory()
{
    delete history_;
    size_t cap = window_ > 0 ? window_ : 1;
    history_ = new BoundedQueue<T>(cap);
}

template <typename T>
void Generator<T>::ResetState()
{
    AllocateHistory();
    emitted_     = 0;
    baseEmitted_ = 0;
    modIdx_      = 0;
}

template <typename T>
Generator<T>::Generator(const Rule& rule,
                        Sequence<T>& initials,
                        size_t window)
    : rule_(rule),
      window_(window),
      initials_(FromSequence(initials)),
      history_(nullptr),
      emitted_(0),
      baseEmitted_(0),
      hasBound_(false),
      maxCount_(0),
      modIdx_(0)
{
    if (initials_.size() > window)
        throw std::invalid_argument("Generator: initials.size() > window");
    AllocateHistory();
}

template <typename T>
Generator<T>::Generator(const Rule& rule,
                        Sequence<T>& initials,
                        size_t window,
                        size_t maxCount)
    : rule_(rule),
      window_(window),
      initials_(FromSequence(initials)),
      history_(nullptr),
      emitted_(0),
      baseEmitted_(0),
      hasBound_(true),
      maxCount_(maxCount),
      modIdx_(0)
{
    if (initials_.size() > window)
        throw std::invalid_argument("Generator: initials.size() > window");
    AllocateHistory();
}

template <typename T>
Generator<T>::Generator(const Generator<T>& other)
    : rule_(other.rule_),
      window_(other.window_),
      initials_(other.initials_),
      history_(nullptr),
      emitted_(other.emitted_),
      baseEmitted_(other.baseEmitted_),
      hasBound_(other.hasBound_),
      maxCount_(other.maxCount_),
      mods_(other.mods_),
      modIdx_(other.modIdx_)
{
    if (other.history_)
        history_ = new BoundedQueue<T>(*other.history_);
    else
        AllocateHistory();
}

template <typename T>
Generator<T>& Generator<T>::operator=(const Generator<T>& other)
{
    if (this == &other) return *this;
    rule_        = other.rule_;
    window_      = other.window_;
    initials_    = other.initials_;
    emitted_     = other.emitted_;
    baseEmitted_ = other.baseEmitted_;
    hasBound_    = other.hasBound_;
    maxCount_    = other.maxCount_;
    mods_        = other.mods_;
    modIdx_      = other.modIdx_;
    delete history_;
    history_ = other.history_ ? new BoundedQueue<T>(*other.history_) : nullptr;
    if (!history_) AllocateHistory();
    return *this;
}

template <typename T>
Generator<T>::~Generator()
{
    delete history_;
}

template <typename T>
T Generator<T>::ProduceBase()
{
    T value;
    if (baseEmitted_ < initials_.size())
        value = initials_[baseEmitted_];
    else
        value = rule_(*history_);
    history_->Push(value);
    ++baseEmitted_;
    return value;
}

template <typename T>
T Generator<T>::GetNext()
{
    if (!HasNext())
        throw std::out_of_range("Generator::GetNext: больше нет элементов");

    while (true)
    {
        if (modIdx_ < mods_.size())
        {
            const Modification& m = mods_[modIdx_];
            if (m.pos == emitted_)
            {
                if (m.isInsert)
                {
                    T v = m.value;
                    ++modIdx_;
                    ++emitted_;
                    return v;
                }
                else
                {
                    ++modIdx_;
                    (void)ProduceBase();
                    continue;
                }
            }
        }
        T v = ProduceBase();
        ++emitted_;
        return v;
    }
}

template <typename T>
bool Generator<T>::HasNext() const
{
    if (!hasBound_) return true;
    return emitted_ < maxCount_;
}

template <typename T>
Optional<T> Generator<T>::TryGetNext()
{
    if (!HasNext()) return Optional<T>::None();
    return Optional<T>::Some(GetNext());
}

template <typename T>
void Generator<T>::Reset()
{
    ResetState();
}

template <typename T>
void Generator<T>::InsertMod(const Modification& m)
{
    size_t idx = 0;
    while (idx < mods_.size() && mods_[idx].pos <= m.pos) ++idx;
    mods_.insert(mods_.begin() + idx, m);
}

template <typename T>
void Generator<T>::InsertAt(size_t pos, const T& value)
{
    if (pos < emitted_)
        throw std::invalid_argument("Generator::InsertAt: позиция уже пройдена");
    Modification m{pos, true, value};
    InsertMod(m);
    if (hasBound_) ++maxCount_;
}

template <typename T>
void Generator<T>::RemoveAt(size_t pos)
{
    if (pos < emitted_)
        throw std::invalid_argument("Generator::RemoveAt: позиция уже пройдена");
    Modification m{pos, false, T()};
    InsertMod(m);
    if (hasBound_ && maxCount_ > 0) --maxCount_;
}

template <typename T>
void Generator<T>::InsertSubsequence(size_t pos, Sequence<T>& seq)
{
    for (size_t i = 0; i < seq.GetLength(); ++i)
        InsertAt(pos + i, seq.Get(i));
}

#endif
