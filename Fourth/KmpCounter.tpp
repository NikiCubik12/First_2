#ifndef KMP_COUNTER_TPP
#define KMP_COUNTER_TPP

#include "KmpCounter.hpp"

inline KmpCounter::KmpCounter(const std::string& pattern, bool allowOverlapping)
    : pattern_(pattern),
      failure_(pattern.size(), 0),
      state_(0),
      count_(0),
      position_(0),
      lastMatchEnd_(-1),
      allowOverlapping_(allowOverlapping)
{
    BuildFailure();
}

inline void KmpCounter::BuildFailure()
{
    for (std::size_t i = 1; i < pattern_.size(); ++i)
    {
        int k = failure_[i - 1];
        while (k > 0 && pattern_[k] != pattern_[i]) k = failure_[k - 1];
        if (pattern_[k] == pattern_[i]) ++k;
        failure_[i] = k;
    }
}

inline void KmpCounter::ConsumeChar(char c)
{
    if (pattern_.empty()) { ++position_; return; }
    while (state_ > 0 && pattern_[state_] != c) state_ = failure_[state_ - 1];
    if (pattern_[state_] == c) ++state_;
    ++position_;
    if (state_ == static_cast<int>(pattern_.size()))
    {
        long long endPos = position_ - 1;
        long long len    = static_cast<long long>(pattern_.size());
        if (allowOverlapping_ || endPos >= lastMatchEnd_ + len)
        {
            ++count_;
            lastMatchEnd_ = endPos;
        }
        state_ = failure_[state_ - 1];
    }
}

// inline void KmpCounter::ConsumeStream(ReadOnlyStream<char>& stream)
// {
//     while (!stream.IsEndOfStream())
//         ConsumeChar(stream.Read());
// }

inline void KmpCounter::ConsumeLazy(LazySequence<char>& lazy)
{
    std::size_t i = 0;
    while (true)
    {
        try { ConsumeChar(lazy.Get(i)); ++i; }
        catch (const IndexOutOfRangeException&) { break; }
    }
}

inline void KmpCounter::Reset()
{
    state_         = 0;
    count_         = 0;
    position_      = 0;
    lastMatchEnd_  = -1;
}

#endif
