#ifndef KMP_COUNTER_HPP
#define KMP_COUNTER_HPP

#include <cstddef>
#include <string>
#include <vector>
#include "ReadOnlyStream.hpp"
#include "LazySequence.hpp"

class KmpCounter
{
public:
    KmpCounter(const std::string& pattern, bool allowOverlapping);

    void   ConsumeChar(char c);
    void   ConsumeStream(ReadOnlyStream<char>& stream);
    void   ConsumeLazy(LazySequence<char>& lazy);

    void   Reset();
    size_t GetCount() const { return count_; }
    size_t GetProcessedCount() const { return position_; }

private:
    std::string      pattern_;
    std::vector<int> failure_;
    int              state_;
    std::size_t      count_;
    long long        position_;
    long long        lastMatchEnd_;
    bool             allowOverlapping_;

    void BuildFailure();
};

#include "KmpCounter.tpp"

#endif
