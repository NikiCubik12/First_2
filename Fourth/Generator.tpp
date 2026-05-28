#ifndef GENERATOR_TPP
#define GENERATOR_TPP

#include "Generator.hpp"

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
    ClearMods();
}

template <typename T>
void Generator<T>::ClearMods()
{
    Modification* curr = mods_;
    while (curr)
    {
        Modification* next = curr->next;
        delete curr;
        curr = next;
    }
    mods_ = nullptr;
    modCount_ = 0;
}

template <typename T>
Generator<T>::Generator(const Rule& rule,
                        Sequence<T>& initials,
                        size_t window)
    : rule_(rule),
      window_(window),
      initials_(),
      history_(nullptr),
      emitted_(0),
      baseEmitted_(0),
      hasBound_(false),
      maxCount_(0),
      mods_(nullptr),
      modCount_(0)
{
    for (size_t i = 0; i < initials.GetLength(); ++i)
        initials_.Append(initials.Get(i));
    
    if (initials_.GetLength() > window)
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
      initials_(),
      history_(nullptr),
      emitted_(0),
      baseEmitted_(0),
      hasBound_(true),
      maxCount_(maxCount),
      mods_(nullptr),
      modCount_(0)
{
    for (size_t i = 0; i < initials.GetLength(); ++i)
        initials_.Append(initials.Get(i));
    
    if (initials_.GetLength() > window)
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
      mods_(nullptr),
      modCount_(0)
{
    AllocateHistory();
    
    // Копируем модификации
    Modification* curr = other.mods_;
    while (curr)
    {
        InsertMod({curr->pos, curr->isInsert, curr->value, nullptr});
        curr = curr->next;
    }
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
    
    delete history_;
    AllocateHistory();
    ClearMods();
    
    Modification* curr = other.mods_;
    while (curr)
    {
        InsertMod({curr->pos, curr->isInsert, curr->value, nullptr});
        curr = curr->next;
    }
    
    return *this;
}

template <typename T>
Generator<T>::~Generator()
{
    delete history_;
    ClearMods();
}

template <typename T>
T Generator<T>::ProduceBase()
{
    T value;
    if (baseEmitted_ < initials_.GetLength())
        value = initials_.Get(baseEmitted_);
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
        Modification* curr = mods_;
        size_t idx = 0;
        
        while (curr && idx < emitted_)
        {
            curr = curr->next;
            ++idx;
        }
        
        if (curr && curr->pos == emitted_)
        {
            if (curr->isInsert)
            {
                T v = curr->value;
                ++emitted_;
                return v;
            }
            else
            {
                (void)ProduceBase();
                ++emitted_;
                continue;
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
    Modification* newMod = new Modification{m.pos, m.isInsert, m.value, nullptr};
    
    if (!mods_ || mods_->pos > m.pos)
    {
        newMod->next = mods_;
        mods_ = newMod;
    }
    else
    {
        Modification* curr = mods_;
        while (curr->next && curr->next->pos <= m.pos)
            curr = curr->next;
        newMod->next = curr->next;
        curr->next = newMod;
    }
    ++modCount_;
}

template <typename T>
void Generator<T>::InsertAt(size_t pos, const T& value)
{
    if (pos < emitted_)
        throw std::invalid_argument("Generator::InsertAt: позиция уже пройдена");
    InsertMod({pos, true, value, nullptr});
    if (hasBound_) ++maxCount_;
}

template <typename T>
void Generator<T>::RemoveAt(size_t pos)
{
    if (pos < emitted_)
        throw std::invalid_argument("Generator::RemoveAt: позиция уже пройдена");
    InsertMod({pos, false, T(), nullptr});
    if (hasBound_ && maxCount_ > 0) --maxCount_;
}

template <typename T>
void Generator<T>::InsertSubsequence(size_t pos, Sequence<T>& seq)
{
    for (size_t i = 0; i < seq.GetLength(); ++i)
        InsertAt(pos + i, seq.Get(i));
}

#endif