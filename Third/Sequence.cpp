#include "Sequence.hpp"
using std::string;

SequenceException::SequenceException(string msg) : message(msg) {}

const char* SequenceException::what() const noexcept
{ 
    return message.c_str(); 
}

IndexOutOfRangeException::IndexOutOfRangeException(string msg) : SequenceException(msg) {}

SequenceEmptyException::SequenceEmptyException(string msg) : SequenceException(msg) {}

NullPointerException::NullPointerException(string msg) : SequenceException(msg) {}

InvalidBitException::InvalidBitException(string msg) : SequenceException(msg) {}

DifferentSizeException::DifferentSizeException(string msg) : SequenceException(msg) {}