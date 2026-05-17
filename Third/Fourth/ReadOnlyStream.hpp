#ifndef READ_ONLY_STREAM_HPP
#define READ_ONLY_STREAM_HPP

#include <cstddef>
#include <functional>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "IStream.hpp"
#include "Sequence.hpp"
#include "LazySequence.hpp"
#include "Option.hpp"
#include "Serializer.hpp"

template <typename T>
class ReadOnlyStream : public IStream<T>
{
private:
    std::function<Optional<T>()> producer_;
    std::vector<T>               buffer_;
    bool                         exhausted_;
    size_t                       pos_;
    bool                         open_;

    void EnsureBuffered(size_t n);

public:
    explicit ReadOnlyStream(Sequence<T>* src);
    explicit ReadOnlyStream(LazySequence<T>* src);
    ReadOnlyStream(const std::string& s);
    ReadOnlyStream(std::shared_ptr<std::istream> in, Deserializer<T> deserializer);
    explicit ReadOnlyStream(ReadOnlyStream<T>& other);

    void   Open()              override { open_ = true; }
    void   Close()             override { open_ = false; }
    size_t GetPosition() const override { return pos_; }

    bool   IsEndOfStream();
    T      Read();
    bool   IsCanSeek()   const { return true; }
    void   Seek(size_t newPos);
    bool   IsCanGoBack() const { return true; }
};

#include "ReadOnlyStream.tpp"

#endif
