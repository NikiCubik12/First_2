#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include "Sequence.hpp"
#include "MutableArraySequence.hpp"

template <class T>
class Option 
{
    private:
    alignas(T) unsigned char value[sizeof(T)]; 
    bool HasValue;
    
    public:
    Option();
    Option(T value);
    Option(const Option& other);
    ~Option();
    Option& operator=(Option other);
    
    bool IsSome();
    bool IsNone();
    T GetValue();
    T GetValueOrDefault(T defaultValue) ;
    
    template <class R>
    Option<R> Map(R (*func)(T)) ;
    
    Option<T> Where(bool (*predicate)(T)) ;
    T OrElse(T defaultValue) ;
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