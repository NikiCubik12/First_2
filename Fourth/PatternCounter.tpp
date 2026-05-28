#ifndef PATTERN_COUNTER_TPP
#define PATTERN_COUNTER_TPP

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
    for (size_t i = 0; i < patterns.GetLength(); ++i)
    {
        const std::string& p = patterns.Get(i);
        if (!p.empty()) patterns_.Append(p);
    }
    
    for (size_t i = 0; i < patterns_.GetLength(); ++i)
    {
        counts_.Append(0);
        lastMatchEnd_.Append(-1);
    }
    
    nodes_.Append(CharNode());
    BuildTrie();
    BuildFailLinks();
}

inline void PatternCounter::BuildTrie()
{
    for (size_t i = 0; i < patterns_.GetLength(); ++i)
    {
        int cur = 0;
        const std::string& pattern = patterns_.Get(i);
        
        for (size_t j = 0; j < pattern.size(); ++j)
        {
            unsigned char c = static_cast<unsigned char>(pattern[j]);
            
            CharNode& curNode = nodes_.GetRef(cur);
            
            if (curNode.next[c] == -1)
            {
                nodes_.Append(CharNode());
                int idx = static_cast<int>(nodes_.GetLength()) - 1;
                curNode.next[c] = idx;
                cur = idx;
            }
            else
            {
                cur = curNode.next[c];
            }
        }
        
        nodes_.GetRef(cur).patternEnd = static_cast<int>(i);
    }
}

inline void PatternCounter::BuildFailLinks()
{
    MutableArraySequence<int> queue;
    
    nodes_.GetRef(0).fail = 0;
    nodes_.GetRef(0).outputLink = -1;
    
    for (int c = 0; c < 256; ++c)
    {
        if (nodes_.GetRef(0).next[c] != -1)
        {
            int child = nodes_.GetRef(0).next[c];
            nodes_.GetRef(child).fail = 0;
            nodes_.GetRef(child).outputLink = nodes_.GetRef(0).patternEnd >= 0 ? 0 : -1;
            queue.Append(child);
        }
    }
    
    size_t qIdx = 0;
    while (qIdx < queue.GetLength())
    {
        int u = queue.Get(qIdx);
        ++qIdx;
        
        CharNode& uNode = nodes_.GetRef(u);
        
        for (int c = 0; c < 256; ++c)
        {
            int v = uNode.next[c];
            if (v != -1)
            {
                int f = uNode.fail;
                while (f != 0 && nodes_.GetRef(f).next[c] == -1)
                    f = nodes_.GetRef(f).fail;
                
                int failNode = (nodes_.GetRef(f).next[c] != -1 && nodes_.GetRef(f).next[c] != v) 
                              ? nodes_.GetRef(f).next[c] : 0;
                if (u == 0) failNode = 0;
                
                nodes_.GetRef(v).fail = failNode;
                nodes_.GetRef(v).outputLink = (nodes_.GetRef(failNode).patternEnd >= 0)
                                          ? failNode
                                          : nodes_.GetRef(failNode).outputLink;
                queue.Append(v);
            }
        }
    }
}

inline int PatternCounter::Goto(int s, char c) const
{
    unsigned char uc = static_cast<unsigned char>(c);
    
    // Для доступа из const метода используем const_cast
    PatternCounter* nonConst = const_cast<PatternCounter*>(this);
    
    while (s != 0 && nonConst->nodes_.Get(s).next[uc] == -1)
        s = nonConst->nodes_.Get(s).fail;
    
    if (nonConst->nodes_.Get(s).next[uc] != -1)
        return nonConst->nodes_.Get(s).next[uc];
    return 0;
}

inline void PatternCounter::EmitMatches(int s)
{
    int cur = s;
    while (cur != -1 && cur != 0)
    {
        if (nodes_.Get(cur).patternEnd >= 0)
        {
            int idx = nodes_.Get(cur).patternEnd;
            long long len = static_cast<long long>(patterns_.Get(idx).size());
            long long endPos = position_ - 1;
            
            if (allowOverlapping_ || endPos >= lastMatchEnd_.Get(idx) + len)
            {
                size_t newCount = counts_.Get(idx) + 1;
                counts_.GetRef(idx) = newCount;
                lastMatchEnd_.GetRef(idx) = endPos;
            }
        }
        cur = nodes_.Get(cur).outputLink;
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
    size_t i = 0;
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
    for (size_t i = 0; i < counts_.GetLength(); ++i)
        counts_.GetRef(i) = 0;
    for (size_t i = 0; i < lastMatchEnd_.GetLength(); ++i)
        lastMatchEnd_.GetRef(i) = -1;
}

inline Sequence<std::pair<std::string, std::size_t>>* PatternCounter::GetCounts() const
{
    auto* result = new MutableArraySequence<std::pair<std::string, std::size_t>>();
    PatternCounter* nonConst = const_cast<PatternCounter*>(this);
    for (size_t i = 0; i < nonConst->patterns_.GetLength(); ++i)
        result->Append(std::make_pair(nonConst->patterns_.Get(i), nonConst->counts_.Get(i)));
    return result;
}

#endif