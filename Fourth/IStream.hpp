#ifndef ISTREAM_HPP
#define ISTREAM_HPP

#include <cstddef>
#include <stdexcept>

class EndOfStreamException : public std::runtime_error
{
public:
    explicit EndOfStreamException(const std::string& msg) : std::runtime_error(msg) {}
};

template <typename T>
class IStreamBase
{
public:
    virtual ~IStreamBase() = default;
    virtual void   Open()                  = 0;
    virtual void   Close()                 = 0;
    virtual size_t GetPosition() const     = 0;
};

#endif