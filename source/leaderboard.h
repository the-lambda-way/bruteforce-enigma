#pragma once

#include <deque>
#include <utility>        // std::forward
#include <functional>     // std::greater
#include <limits>
#include <queue>
#include <string_view>
#include <vector>


// Relevant properties:
//      * Small, known size
//      * Needs fast insertion
//      * Stability not important
//      * Existing list already satisfies invariant
//      * Slow extraction of results is okay
//      * Scores are numeric

// This has not yet become a bottleneck, so uses a naive design.



template <class Identifier>
struct ScoreEntry
{
     double     score = std::numeric_limits<double>::lowest();
     Identifier id    = {};


     friend bool operator> (const ScoreEntry<Identifier>& a, const ScoreEntry<Identifier>& b)
     {
          return a.score > b.score;
     }
};


template <int N, class Identifier>
class Leaderboard
{
public:
     constexpr Leaderboard ()
     {
          ScoreEntry<Identifier> default_entry {};
          for (int i = 0; i < N; ++i)     entries.push(default_entry);
     }

     // Must manually check that the score should be added first, to avoid needless copying of the settings.
     bool is_good_score (double score) const     { return score > entries.top().score; }


     void add (double score, Identifier id)
     {
          entries.emplace(score, std::move(id));
          entries.pop();
     }


     void add (ScoreEntry<Identifier> e)
     {
          entries.push(std::move(e));
          entries.pop();
     }


     std::deque<ScoreEntry<Identifier>> get_entries () const
     {
          auto copy = entries;
          std::deque<ScoreEntry<Identifier>> ordered_entries;

          while (!copy.empty())
          {
               ordered_entries.push_front(copy.top());
               copy.pop();
          }

          return ordered_entries;
     }


private:
     // priority_queue does not give access to its member container, but we need it to iterate over the entries
     template <class T, class F>
     class VectorQueue : public std::priority_queue<T, std::vector<T>, F>
     {
          public:
          const std::vector<T>& base () const    { return this->c; }
     }; // class VectorQueue


     VectorQueue<ScoreEntry<Identifier>, std::greater<>> entries;

     template <int M, int SizeA, int SizeB, class Id>
     friend Leaderboard<M, Id> combine_leaderboards (
          const Leaderboard<SizeA, Id>& a,
          const Leaderboard<SizeB, Id>& b
     );
}; // class Leaderboard


template <int N, int SizeA, int SizeB, class Identifier>
Leaderboard<N, Identifier> combine_leaderboards (
     const Leaderboard<SizeA, Identifier>& a,
     const Leaderboard<SizeB, Identifier>& b)
{
     if constexpr (N == SizeA && N != SizeB)
     {
          Leaderboard<N, Identifier> out {a};

          for (const ScoreEntry<Identifier>& entry : b.entries.base())     out.add(entry);

          return out;
     }

     else if constexpr (N == SizeB)     // also catches N == SizeA && N == SizeB
     {
          Leaderboard<N, Identifier> out {b};

          for (const ScoreEntry<Identifier>& entry : a.entries.base())     out.add(entry);

          return out;
     }

     else
     {
          Leaderboard<N, Identifier> out;

          for (const ScoreEntry<Identifier>& entry : a.entries.base())     out.add(entry);
          for (const ScoreEntry<Identifier>& entry : b.entries.base())     out.add(entry);

          return out;
     }
}


template <int N, int SizeA, int SizeB, class Identifier, class... List>
Leaderboard<N, Identifier> combine_leaderboards (
     const Leaderboard<SizeA, Identifier>& a,
     const Leaderboard<SizeB, Identifier>& b,
     List&&... rest)
{
     return combine_leaderboards<N>(
          combine_leaderboards<N, SizeA, SizeB, Identifier>(a, b), std::forward<List>(rest)...
     );
}
