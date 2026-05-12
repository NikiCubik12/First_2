#include <iostream>
#include "MutableArraySequence.hpp"

int main()
{
    MutableArraySequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);

    std::cout << "Fourth: каркас собран. Sequence:";
    for (size_t i = 0; i < seq.GetLength(); ++i)
        std::cout << ' ' << seq.Get(i);
    std::cout << std::endl;

    return 0;
}