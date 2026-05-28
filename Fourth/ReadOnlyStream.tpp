#ifndef READ_ONLY_STREAM_TPP
#define READ_ONLY_STREAM_TPP

#include "ReadOnlyStream.hpp"

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(Sequence<T>* src)
    : producer_(),
      buffer_(),
      exhausted_(true),
      pos_(0),
      open_(true)
{
    if (!src) throw NullPointerException("ReadOnlyStream(Sequence*): nullptr");
    for (size_t i = 0; i < src->GetLength(); ++i)
        buffer_.Append(src->Get(i));
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(LazySequence<T>* src)
    : producer_(),
      buffer_(),
      exhausted_(false),
      pos_(0),
      open_(true)
{
    if (!src) throw NullPointerException("ReadOnlyStream(LazySequence*): nullptr");
    auto idx = std::make_shared<size_t>(0);
    auto srcPtr = src;
    producer_ = [idx, srcPtr]() -> Optional<T>
    {
        try {
            T v = srcPtr->Get(*idx);
            ++(*idx);
            return Optional<T>::Some(v);
        } catch (const IndexOutOfRangeException&) {
            return Optional<T>::None();
        }
    };
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(const std::string& s)
    : producer_(),
      buffer_(),
      exhausted_(true),
      pos_(0),
      open_(true)
{
    for (char c : s)
        buffer_.Append(static_cast<T>(c));
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(std::shared_ptr<std::istream> in,
                                  Deserializer<T> deserializer)
    : producer_(),
      buffer_(),
      exhausted_(false),
      pos_(0),
      open_(true)
{
    if (!in) throw NullPointerException("ReadOnlyStream(istream): nullptr");
    if (!deserializer) throw NullPointerException("ReadOnlyStream(istream): deserializer == null");
    producer_ = [in, deserializer]() -> Optional<T>
    {
        T v;
        if (deserializer(*in, v)) return Optional<T>::Some(v);
        return Optional<T>::None();
    };
}

template <typename T>
ReadOnlyStream<T>::ReadOnlyStream(ReadOnlyStream<T>& other)
    : producer_(other.producer_),
      buffer_(other.buffer_),
      exhausted_(other.exhausted_),
      pos_(other.pos_),
      open_(other.open_)
{}

template <typename T>
void ReadOnlyStream<T>::EnsureBuffered(size_t n)
{
    while (!exhausted_ && producer_ && buffer_.GetLength() < n)
    {
        Optional<T> nxt = producer_();
        if (!nxt.HasValue()) { exhausted_ = true; break; }
        buffer_.Append(nxt.GetValue());
    }
}

template <typename T>
bool ReadOnlyStream<T>::IsEndOfStream()
{
    EnsureBuffered(pos_ + 1);
    return pos_ >= buffer_.GetLength();
}

template <typename T>
T ReadOnlyStream<T>::Read()
{
    if (!open_)
        throw std::runtime_error("ReadOnlyStream::Read: поток закрыт");
    EnsureBuffered(pos_ + 1);
    if (pos_ >= buffer_.GetLength())
        throw EndOfStreamException("ReadOnlyStream::Read: конец потока");
    T v = buffer_.Get(pos_);
    ++pos_;
    return v;
}

template <typename T>
void ReadOnlyStream<T>::Seek(size_t newPos)
{
    EnsureBuffered(newPos);
    if (newPos > buffer_.GetLength())
        throw std::out_of_range("ReadOnlyStream::Seek: позиция вне диапазона");
    pos_ = newPos;
}

#endif