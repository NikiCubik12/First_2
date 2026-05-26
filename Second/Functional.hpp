#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include "Sequence.hpp"
#include "MutableArraySequence.hpp"

template <class T>
class ICollection 
{
    public:
    virtual ~ICollection() = default;
    virtual T Get(size_t index) const = 0;
    virtual size_t GetSize() const = 0;
    virtual bool IsEmpty() const = 0;
    virtual void Clear() = 0;
};

template <class T>
class Option 
{
    private:
    T* _value;
    bool _hasValue;
    
    public:
    Option();
    Option(const T& value);
    Option(const Option& other);
    ~Option();
    Option& operator=(const Option& other);
    
    bool IsSome() const;
    bool IsNone() const;
    T GetValue() const;
    T GetValueOrDefault(const T& defaultValue) const;
    
    template <class R>
    Option<R> Map(R (*func)(T)) const;
    
    Option<T> Where(bool (*predicate)(T)) const;
    T OrElse(const T& defaultValue) const;
};

template <class T>
class TryHelpers 
{
    public:
    static Option<T> TryGet(Sequence<T>* seq, size_t index);
    static Option<T> TryGetFirst(Sequence<T>* seq);
    static Option<T> TryGetLast(Sequence<T>* seq);
    static Option<T> TryFind(Sequence<T>* seq, bool (*predicate)(T));
};

template <class T, class R>
MutableArraySequence<R>* MapWithIndex(Sequence<T>* seq, R (*func)(T, size_t));

template <class T, class R>
MutableArraySequence<R>* Map(Sequence<T>* seq, R (*func)(T));

template <class T>
MutableArraySequence<T>* Filter(Sequence<T>* seq, bool (*predicate)(T));

template <class T, class R>
R Reduce(Sequence<T>* seq, R initialValue, R (*func)(R, T));

template <class T>
MutableArraySequence<MutableArraySequence<T>*>* GetAllPrefixes(Sequence<T>* seq);

template <class T>
MutableArraySequence<MutableArraySequence<T>*>* GetAllPostfixes(Sequence<T>* seq);

template <class T, class R>
R ReduceWithIndex(Sequence<T>* seq, R initialValue, R (*func)(R, T, size_t));

#include "Functional.tpp"

#endif