#pragma once

#include <cstdio>
#include <deque>
#include <utility>        // std::forward
#include <functional>     // std::greater
#include <queue>
#include <string_view>
#include <vector>
#include "enigma.h"
#include "models.h"
#include "rotors.h"


struct ScoreEntry
{
     double    score;
     EnigmaKey key;


     constexpr ScoreEntry ()
     : ScoreEntry{-1e12, {ETW_ABCDEF, m3_I, m3_II, m3_III, UKWB, "ABCDEFGHIJKLMNOPQRSTUVWXYZ"}}
     {}


     constexpr ScoreEntry (double score, EnigmaKey key)
     : score {score}, key {std::move(key)}
     {}


     friend bool operator> (const ScoreEntry& a, const ScoreEntry& b);
     void print (int i, std::string_view ct) const;
};


// priority_queue does not give access to its member container, but we need it to iterate over the entries
template <class T, class F>
class VectorQueue : public std::priority_queue<T, std::vector<T>, F>
{
public:
    const std::vector<T>& base () const    { return this->c; }
}; // class VectorQueue


template <int N>
class BestList
{
public:
     constexpr BestList (std::string_view ct)
     : ct {ct}
     {
          ScoreEntry default_entry {};
          for (int i = 0; i < N; ++i)     entries.push(default_entry);
     }

     // Must manually check that the score should be added first, to avoid needless copying of the settings.
     bool is_good_score (double score) const     { return score > entries.top().score; }


     void add (double score, EnigmaKey key)
     {
          entries.emplace(score, std::move(key));
          entries.pop();
     }


     void add (ScoreEntry e)
     {
          entries.push(std::move(e));
          entries.pop();
     }


     void print () const
     {
          std::printf("#    Score         Refl     Rotor Order         Ring Pos   Rotor Pos  Text  \n");

          int i = 0;
          for (const ScoreEntry& entry : get_entries())     entry.print(++i, ct);
     }


     std::deque<ScoreEntry> get_entries () const
     {
          auto copy = entries;
          std::deque<ScoreEntry> ordered_entries;

          while (!copy.empty())
          {
               ordered_entries.push_front(copy.top());
               copy.pop();
          }

          return ordered_entries;
     }


private:
     std::string_view ct;
     VectorQueue<ScoreEntry, std::greater<>> entries;

     template <int M, int SizeA, int SizeB>
     friend BestList<M> combine_best (const BestList<SizeA>& a, const BestList<SizeB>& b);

}; // class BestList


template <int N, int SizeA, int SizeB>
BestList<N> combine_best (const BestList<SizeA>& a, const BestList<SizeB>& b)
{
     if constexpr (N == SizeA && N != SizeB)
     {
          BestList<N> out {a};

          for (const ScoreEntry& entry : b.entries.base())     out.add(entry);

          return out;
     }

     else if constexpr (N == SizeB)     // also catches N == SizeA && N == SizeB
     {
          BestList<N> out {b};

          for (const ScoreEntry& entry : a.entries.base())     out.add(entry);

          return out;
     }

     else
     {
          BestList<N> out {a.ct};

          for (const ScoreEntry& entry : a.entries.base())     out.add(entry);
          for (const ScoreEntry& entry : b.entries.base())     out.add(entry);

          return out;
     }
}


template <int N, int SizeA, int SizeB, class... List>
BestList<N> combine_best (const BestList<SizeA>& a, const BestList<SizeB>& b, List&&... rest)
{
     return combine_best<N>(combine_best<N>(a, b), std::forward<List>(rest)...);
}
