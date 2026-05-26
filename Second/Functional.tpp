#ifndef FUNCTIONAL_TPP
#define FUNCTIONAL_TPP

#include "MutableArraySequence.hpp"

template <class T>
Option<T>::Option() : _value(nullptr), _hasValue(false) {}

template <class T>
Option<T>::Option(const T& value) : _value(new T(value)), _hasValue(true) {}

template <class T>
Option<T>::Option(const Option& other) : _value(nullptr), _hasValue(other._hasValue) 
{
    if (_hasValue) 
        _value = new T(*other._value);
}

template <class T>
Option<T>::~Option() 
{ 
    delete _value; 
}

template <class T>
Option<T>& Option<T>::operator=(const Option& other) 
{
    if (this != &other) 
    {
        delete _value;
        _hasValue = other._hasValue;
        if (_hasValue) 
            _value = new T(*other._value);
        else 
            _value = nullptr;
    }
    return *this;
}

template <class T>
bool Option<T>::IsSome() const 
{
    return _hasValue; 
}

template <class T>
bool Option<T>::IsNone() const 
{ 
    return !_hasValue; 
}

template <class T>
T Option<T>::GetValue() const 
{
    if (!_hasValue) 
        throw std::runtime_error("Ошибка: Option не содержит значения");
    return *_value;
}

template <class T>
T Option<T>::GetValueOrDefault(const T& defaultValue) const 
{
    return _hasValue ? *_value : defaultValue;
}

template <class T>
template <class R>
Option<R> Option<T>::Map(R (*func)(T)) const {
    if (!_hasValue || !func) return Option<R>();
    return Option<R>(func(*_value));
}

template <class T>
Option<T> Option<T>::Where(bool (*predicate)(T)) const {
    if (!_hasValue || !predicate) return Option<T>();
    return predicate(*_value) ? *this : Option<T>();
}

template <class T>
T Option<T>::OrElse(const T& defaultValue) const {
    return _hasValue ? *_value : defaultValue;
}


template <class T>
Option<T> TryHelpers<T>::TryGet(Sequence<T>* seq, size_t index) 
{
    if (!seq || index >= seq->GetLength()) return Option<T>();
    try 
    {
        return Option<T>(seq->Get(index));
    } 
    catch (...) 
    {
        return Option<T>();
    }
}

template <class T>
Option<T> TryHelpers<T>::TryGetFirst(Sequence<T>* seq) 
{
    if (!seq || seq->GetLength() == 0) 
        return Option<T>();
    return TryGet(seq, 0);
}

template <class T>
Option<T> TryHelpers<T>::TryGetLast(Sequence<T>* seq) 
{
    if (!seq || seq->GetLength() == 0) 
        return Option<T>();
    return TryGet(seq, seq->GetLength() - 1);
}

template <class T>
Option<T> TryHelpers<T>::TryFind(Sequence<T>* seq, bool (*predicate)(T)) 
{
    if (!seq || !predicate) return Option<T>();
    for (size_t i = 0; i < seq->GetLength(); i++) 
    {
        T val = seq->Get(i);
        if (predicate(val)) 
            return Option<T>(val);
    }
    return Option<T>();
}


template <class T, class R>
MutableArraySequence<R>* MapWithIndex(Sequence<T>* seq, R (*func)(T, size_t)) 
{
    if (!seq) 
        throw std::invalid_argument("Ошибка: MapWithIndex - последовательность не может быть пустой");
    if (!func) 
        throw std::invalid_argument("Ошибка: MapWithIndex - функция не может быть пустой");
    
    MutableArraySequence<R>* result = new MutableArraySequence<R>();
    for (size_t i = 0; i < seq->GetLength(); i++) 
    {
        result->Append(func(seq->Get(i), i));
    }
    return result;
}


template <class T, class R>
MutableArraySequence<R>* Map(Sequence<T>* seq, R (*func)(T)) 
{
    if (!seq) 
        throw std::invalid_argument("Ошибка: Map - последовательность не может быть пустой");
    if (!func) 
        throw std::invalid_argument("Ошибка: Map - функция не может быть пустой");
    
    MutableArraySequence<R>* result = new MutableArraySequence<R>();
    for (size_t i = 0; i < seq->GetLength(); i++) 
    {
        result->Append(func(seq->Get(i)));
    }
    return result;
}


template <class T>
MutableArraySequence<T>* Filter(Sequence<T>* seq, bool (*predicate)(T)) {
    if (!seq) 
        throw std::invalid_argument("Ошибка: Filter - последовательность не может быть пустой");
    if (!predicate) 
        throw std::invalid_argument("Ошибка: Filter - функция-предикат не может быть пустой");
    
    MutableArraySequence<T>* result = new MutableArraySequence<T>();
    for (size_t i = 0; i < seq->GetLength(); i++) 
    {
        T val = seq->Get(i);
        if (predicate(val)) 
        {
            result->Append(val);
        }
    }
    return result;
}


template <class T, class R>
R Reduce(Sequence<T>* seq, R initialValue, R (*func)(R, T)) 
{
    if (!seq) 
        throw std::invalid_argument("Ошибка: Reduce - последовательность не может быть пустой");
    if (!func) 
        throw std::invalid_argument("Ошибка: Reduce - функция не может быть пустой");
    
    R result = initialValue;
    for (size_t i = 0; i < seq->GetLength(); i++) 
    {
        result = func(result, seq->Get(i));
    }
    return result;
}


template <class T>
MutableArraySequence<MutableArraySequence<T>*>* GetAllPrefixes(Sequence<T>* seq) 
{
    if (!seq) 
        throw std::invalid_argument("Ошибка: GetAllPrefixes - последовательность не может быть пустой");
    
    MutableArraySequence<MutableArraySequence<T>*>* result = new MutableArraySequence<MutableArraySequence<T>*>();
    
    for (size_t i = 1; i <= seq->GetLength(); i++) 
    {
        MutableArraySequence<T>* prefix = new MutableArraySequence<T>();
        for (size_t j = 0; j < i; j++) 
            prefix->Append(seq->Get(j));
        result->Append(prefix);
    }
    
    return result;
}


template <class T>
MutableArraySequence<MutableArraySequence<T>*>* GetAllPostfixes(Sequence<T>* seq) {
    if (!seq) 
        throw std::invalid_argument("Ошибка: GetAllPostfixes - последовательность не может быть пустой");
    
    MutableArraySequence<MutableArraySequence<T>*>* result = new MutableArraySequence<MutableArraySequence<T>*>();
    
    for (size_t i = 0; i < seq->GetLength(); i++) 
    {
        MutableArraySequence<T>* postfix = new MutableArraySequence<T>();
        for (size_t j = i; j < seq->GetLength(); j++) 
            postfix->Append(seq->Get(j));
        result->Append(postfix);
    }
    
    return result;
}

template <class T, class R>
R ReduceWithIndex(Sequence<T>* seq, R initialValue, R (*func)(R, T, size_t))
{
    if (!seq) 
        throw std::invalid_argument("Ошибка: ReduceWithIndex - последовательность не может быть пустой");
    if (!func) 
        throw std::invalid_argument("Ошибка: ReduceWithIndex - функция не может быть пустой");
    
    R result = initialValue;
    for (size_t i = 0; i < seq->GetLength(); i++) {
        result = func(result, seq->Get(i), i);
    }
    return result;
}

#endif