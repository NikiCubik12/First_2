#ifndef PATTERN_COUNTER_HPP
#define PATTERN_COUNTER_HPP

#include <cstddef>
#include <string>
#include <utility>
#include "Sequence.hpp"
#include "MutableArraySequence.hpp"
#include "ReadOnlyStream.hpp"
#include "LazySequence.hpp"

struct CharNode
{
    int  next[256];
    int  fail;
    int  outputLink;
    int  patternEnd;
    
    CharNode()
    {
        for (int i = 0; i < 256; ++i)
            next[i] = -1;
        fail = 0;
        outputLink = -1;
        patternEnd = -1;
    }
};

class PatternCounter
{
public:
    PatternCounter(Sequence<std::string>& patterns, bool allowOverlapping);

    void   ConsumeChar(char c);
    // void   ConsumeStream(ReadOnlyStream<char>& stream);
    void   ConsumeLazy(LazySequence<char>& lazy);

    void   Reset();
    size_t GetProcessedCount() const { return position_; }

    Sequence<std::pair<std::string, std::size_t>>* GetCounts() const;

private:
    MutableArraySequence<std::string> patterns_;
    MutableArraySequence<CharNode>    nodes_;
    MutableArraySequence<std::size_t> counts_;
    MutableArraySequence<long long>   lastMatchEnd_;
    bool                     allowOverlapping_;
    int                      state_;
    long long                position_;

    void BuildTrie();
    void BuildFailLinks();
    int  Goto(int s, char c) const;
    void EmitMatches(int s);
};

#include "PatternCounter.tpp"

#endif