#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <functional>
#include <iostream>

template <typename T>
using Serializer = std::function<void(std::ostream&, const T&)>;

template <typename T>
using Deserializer = std::function<bool(std::istream&, T&)>;

#endif
