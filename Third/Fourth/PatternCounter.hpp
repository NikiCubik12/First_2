#ifndef PATTERN_COUNTER_HPP
#define PATTERN_COUNTER_HPP

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include "Sequence.hpp"
#include "MutableArraySequence.hpp"
#include "ReadOnlyStream.hpp"
#include "LazySequence.hpp"

class PatternCounter
{
public:
    PatternCounter(Sequence<std::string>& patterns, bool allowOverlapping);

    void   ConsumeChar(char c);
    void   ConsumeStream(ReadOnlyStream<char>& stream);
    void   ConsumeLazy(LazySequence<char>& lazy);

    void   Reset();
    size_t GetProcessedCount() const { return position_; }

    Sequence<std::pair<std::string, std::size_t>>* GetCounts() const;

private:
    struct Node
    {
        std::unordered_map<char, int> children;
        int                            fail;
        int                            outputLink;
        int                            patternEnd;

        Node() : children(), fail(0), outputLink(-1), patternEnd(-1) {}
    };

    std::vector<std::string> patterns_;
    std::vector<Node>        nodes_;
    std::vector<std::size_t> counts_;
    std::vector<long long>   lastMatchEnd_;
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
