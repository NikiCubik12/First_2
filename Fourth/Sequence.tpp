#ifndef SEQUENCE_TPP
#define SEQUENCE_TPP

template <class T>
Sequence<T>* Sequence<T>::Append(T item)
{
    return AppendImpl(item);
}

template <class T>
Sequence<T>* Sequence<T>::Prepend(T item)
{
    return PrependImpl(item);
}

template <class T>
Sequence<T>* Sequence<T>::InsertAt(T item, size_t index)
{
    return InsertAtImpl(item, index);
}

// Реализации исключений
inline SequenceException::SequenceException(const std::string& msg) : message(msg) {}
inline const char* SequenceException::what() const noexcept { return message.c_str(); }
inline IndexOutOfRangeException::IndexOutOfRangeException(const std::string& msg) : SequenceException(msg) {}
inline SequenceEmptyException::SequenceEmptyException(const std::string& msg) : SequenceException(msg) {}
inline NullPointerException::NullPointerException(const std::string& msg) : SequenceException(msg) {}
inline InvalidBitException::InvalidBitException(const std::string& msg) : SequenceException(msg) {}
inline DifferentSizeException::DifferentSizeException(const std::string& msg) : SequenceException(msg) {}

#endif