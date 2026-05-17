#ifndef PATTERN_COUNTER_TPP
#define PATTERN_COUNTER_TPP

#include <queue>
#include "PatternCounter.hpp"

inline PatternCounter::PatternCounter(Sequence<std::string>& patterns, bool allowOverlapping)
    : patterns_(),
      nodes_(),
      counts_(),
      lastMatchEnd_(),
      allowOverlapping_(allowOverlapping),
      state_(0),
      position_(0)
{
    patterns_.reserve(patterns.GetLength());
    for (std::size_t i = 0; i < patterns.GetLength(); ++i)
    {
        const std::string& p = patterns.Get(i);
        if (!p.empty()) patterns_.push_back(p);
    }
    counts_.assign(patterns_.size(), 0);
    lastMatchEnd_.assign(patterns_.size(), -1);

    nodes_.emplace_back();
    BuildTrie();
    BuildFailLinks();
}

inline void PatternCounter::BuildTrie()
{
    for (std::size_t i = 0; i < patterns_.size(); ++i)
    {
        int cur = 0;
        for (char c : patterns_[i])
        {
            auto it = nodes_[cur].children.find(c);
            if (it == nodes_[cur].children.end())
            {
                nodes_.emplace_back();
                int idx = static_cast<int>(nodes_.size()) - 1;
                nodes_[cur].children[c] = idx;
                cur = idx;
            }
            else
            {
                cur = it->second;
            }
        }
        nodes_[cur].patternEnd = static_cast<int>(i);
    }
}

inline void PatternCounter::BuildFailLinks()
{
    std::queue<int> q;
    nodes_[0].fail       = 0;
    nodes_[0].outputLink = -1;
    for (auto& kv : nodes_[0].children)
    {
        nodes_[kv.second].fail       = 0;
        nodes_[kv.second].outputLink =
            nodes_[0].patternEnd >= 0 ? 0 : -1;
        q.push(kv.second);
    }
    while (!q.empty())
    {
        int u = q.front(); q.pop();
        for (auto& kv : nodes_[u].children)
        {
            char c = kv.first;
            int  v = kv.second;
            int  f = nodes_[u].fail;
            while (f != 0 && nodes_[f].children.find(c) == nodes_[f].children.end())
                f = nodes_[f].fail;
            auto it = nodes_[f].children.find(c);
            int  failNode = (it != nodes_[f].children.end() && it->second != v) ? it->second : 0;
            if (u == 0) failNode = 0;
            nodes_[v].fail = failNode;
            nodes_[v].outputLink = (nodes_[failNode].patternEnd >= 0)
                                       ? failNode
                                       : nodes_[failNode].outputLink;
            q.push(v);
        }
    }
}

inline int PatternCounter::Goto(int s, char c) const
{
    while (s != 0 && nodes_[s].children.find(c) == nodes_[s].children.end())
        s = nodes_[s].fail;
    auto it = nodes_[s].children.find(c);
    if (it != nodes_[s].children.end()) return it->second;
    return 0;
}

inline void PatternCounter::EmitMatches(int s)
{
    int cur = s;
    while (cur != -1 && cur != 0)
    {
        if (nodes_[cur].patternEnd >= 0)
        {
            int idx = nodes_[cur].patternEnd;
            long long len = static_cast<long long>(patterns_[idx].size());
            long long endPos = position_ - 1;
            if (allowOverlapping_ || endPos >= lastMatchEnd_[idx] + len)
            {
                ++counts_[idx];
                lastMatchEnd_[idx] = endPos;
            }
        }
        cur = nodes_[cur].outputLink;
    }
}

inline void PatternCounter::ConsumeChar(char c)
{
    state_ = Goto(state_, c);
    ++position_;
    EmitMatches(state_);
}

inline void PatternCounter::ConsumeStream(ReadOnlyStream<char>& stream)
{
    while (!stream.IsEndOfStream())
        ConsumeChar(stream.Read());
}

inline void PatternCounter::ConsumeLazy(LazySequence<char>& lazy)
{
    std::size_t i = 0;
    while (true)
    {
        try { ConsumeChar(lazy.Get(i)); ++i; }
        catch (const IndexOutOfRangeException&) { break; }
    }
}

inline void PatternCounter::Reset()
{
    state_ = 0;
    position_ = 0;
    std::fill(counts_.begin(), counts_.end(), 0);
    std::fill(lastMatchEnd_.begin(), lastMatchEnd_.end(), -1);
}

inline Sequence<std::pair<std::string, std::size_t>>* PatternCounter::GetCounts() const
{
    auto* result = new MutableArraySequence<std::pair<std::string, std::size_t>>();
    for (std::size_t i = 0; i < patterns_.size(); ++i)
        result->Append(std::make_pair(patterns_[i], counts_[i]));
    return result;
}

#endif
