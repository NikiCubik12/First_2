#ifndef WRITE_ONLY_STREAM_TPP
#define WRITE_ONLY_STREAM_TPP

#include "WriteOnlyStream.hpp"

template <typename T>
WriteOnlyStream<T>::WriteOnlyStream(Sequence<T>* dst)
    : sink_(),
      pos_(0),
      open_(true)
{
    if (!dst) throw NullPointerException("WriteOnlyStream(Sequence*): nullptr");
    sink_ = [dst](const T& v) { dst->Append(v); };
}

template <typename T>
WriteOnlyStream<T>::WriteOnlyStream(std::shared_ptr<std::ostream> out,
                                    Serializer<T> serializer)
    : sink_(),
      pos_(0),
      open_(true)
{
    if (!out) throw NullPointerException("WriteOnlyStream(ostream): nullptr");
    if (!serializer) throw NullPointerException("WriteOnlyStream(ostream): serializer == null");
    sink_ = [out, serializer](const T& v) { serializer(*out, v); };
}

template <typename T>
WriteOnlyStream<T>::WriteOnlyStream(WriteOnlyStream<T>& other)
    : sink_(other.sink_),
      pos_(other.pos_),
      open_(other.open_)
{}

template <typename T>
void WriteOnlyStream<T>::Write(const T& value)
{
    if (!open_)
        throw std::runtime_error("WriteOnlyStream::Write: поток закрыт");
    if (!sink_)
        throw std::runtime_error("WriteOnlyStream::Write: sink не задан");
    sink_(value);
    ++pos_;
}

#endif
