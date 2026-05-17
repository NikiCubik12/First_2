#ifndef CARDINAL_HPP
#define CARDINAL_HPP

#include <cstddef>
#include <stdexcept>

class Cardinal
{
private:
    bool   isInfinity_;
    size_t value_;

    Cardinal(bool isInf, size_t v) : isInfinity_(isInf), value_(v) {}

public:
    Cardinal() : isInfinity_(false), value_(0) {}
    Cardinal(size_t v) : isInfinity_(false), value_(v) {}

    static Cardinal Infinity() { return Cardinal(true, 0); }
    static Cardinal Finite(size_t v) { return Cardinal(false, v); }

    bool   IsFinite()   const { return !isInfinity_; }
    bool   IsInfinite() const { return  isInfinity_; }

    size_t GetFinite() const
    {
        if (isInfinity_)
            throw std::runtime_error("Cardinal::GetFinite: значение бесконечно");
        return value_;
    }

    bool operator==(const Cardinal& o) const
    {
        if (isInfinity_ != o.isInfinity_) return false;
        if (isInfinity_) return true;
        return value_ == o.value_;
    }
    bool operator!=(const Cardinal& o) const { return !(*this == o); }

    bool operator<(const Cardinal& o) const
    {
        if (isInfinity_)      return false;
        if (o.isInfinity_)    return true;
        return value_ < o.value_;
    }
    bool operator<=(const Cardinal& o) const { return *this == o || *this < o; }
    bool operator>(const Cardinal& o)  const { return o < *this; }
    bool operator>=(const Cardinal& o) const { return o <= *this; }
};

#endif
