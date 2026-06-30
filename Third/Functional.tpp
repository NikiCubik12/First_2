#ifndef FUNCTIONAL_TPP
#define FUNCTIONAL_TPP

#include "MutableArraySequence.hpp"

template <class T>
Option<T>::Option() : value(nullptr), HasValue(false) {}

template <class T>
Option<T>::Option(T value) : value(new T(value)), HasValue(true) {}

template <class T>
Option<T>::Option(const Option& other) : value(nullptr), HasValue(other.HasValue) 
{
    if (HasValue) 
        value = new T(*other.value);
}

template <class T>
Option<T>::~Option() 
{ 
    delete value; 
}

template <class T>
Option<T>& Option<T>::operator=(Option other) 
{
    if (this != &other) 
    {
        delete value;
        HasValue = other.HasValue;
        if (HasValue) 
            value = new T(*other.value);
        else 
            value = nullptr;
    }
    return *this;
}

template <class T>
bool Option<T>::IsSome()  
{
    return HasValue; 
}

template <class T>
bool Option<T>::IsNone()  
{ 
    return !HasValue; 
}

template <class T>
T Option<T>::GetValue()  
{
    if (!HasValue) 
        throw std::runtime_error("Ошибка: Option не содержит значения");
    return *reinterpret_cast<T*>(value);
}

template <class T>
T Option<T>::GetValueOrDefault(T defaultValue)  
{
    return HasValue ? *reinterpret_cast<T*>(value) : defaultValue;
}

template <class T>
template <class R>
Option<R> Option<T>::Map(R (*func)(T))  
{
    if (!HasValue || !func) return Option<R>();
    return Option<R>(func(*reinterpret_cast<T*>(value)));
}

template <class T>
Option<T> Option<T>::Where(bool (*predicate)(T))  {
    if (!HasValue || !predicate) return Option<T>();
    return predicate(*reinterpret_cast<T*>(value)) ? *this : Option<T>();
}

template <class T>
T Option<T>::OrElse(T defaultValue)  {
    return HasValue ? *reinterpret_cast<T*>(value) : defaultValue;
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