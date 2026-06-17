#ifndef OPTION_HPP
#define OPTION_HPP

#include <stdexcept>

template <typename T> // нужен для более безопасного чтения данных
class Optional
{
private:
    bool hasValue_;
    T    value_;

public:
    Optional() : hasValue_(false), value_() {}
    explicit Optional(const T& value) : hasValue_(true), value_(value) {}

    static Optional<T> Some(const T& value) { return Optional<T>(value); }
    static Optional<T> None() { return Optional<T>(); }

    bool HasValue() const { return hasValue_; }

    const T& GetValue() const
    {
        if (!hasValue_)
            throw std::runtime_error("Optional::GetValue: значение отсутствует (None)");
        return value_;
    }

    T OrElse(const T& fallback) const
    {
        return hasValue_ ? value_ : fallback;
    }
};

#endif
