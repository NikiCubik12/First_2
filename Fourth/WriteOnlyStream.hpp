#ifndef WRITE_ONLY_STREAM_HPP
#define WRITE_ONLY_STREAM_HPP

#include <cstddef>
// #include <functional>
#include <memory>
#include "IStream.hpp"
#include "Sequence.hpp"
#include "Serializer.hpp"

template <typename T>
class WriteOnlyStream : public IStream<T>
{
private:
    std::function<void(const T&)> sink_;
    size_t                        pos_;
    bool                          open_;

public:
    explicit WriteOnlyStream(Sequence<T>* dst);
    WriteOnlyStream(std::shared_ptr<std::ostream> out, Serializer<T> serializer);
    WriteOnlyStream(WriteOnlyStream<T>& other);

    void   Open()              override { open_ = true; }
    void   Close()             override { open_ = false; }
    size_t GetPosition() const override { return pos_; }

    void   Write(const T& value);
};

#include "WriteOnlyStream.tpp"

#endif
