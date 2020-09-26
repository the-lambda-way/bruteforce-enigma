#pragma once

#include <cstdio>
#include <deque>
#include <utility>        // std::forward
#include <functional>     // std::greater
#include <queue>
#include <string>
#include <string_view>
#include <vector>
#include "enigma.h"
#include "models.h"
#include "rotors.h"


struct ScoreEntry
{
     double              score;
     EnigmaConfiguration config;


     constexpr ScoreEntry ()
     : ScoreEntry(-1e12, {ETW_ABCDEF, m3_I, m3_II, m3_III, UKWB, ""})
     {}


     constexpr ScoreEntry (double score, EnigmaConfiguration config)
     : score {score}, config {std::move(config)}
     {}


     friend bool operator> (const ScoreEntry& a, const ScoreEntry& b)     { return a.score > b.score; }


     void print (int i, std::string_view ct) const
     {
          std::string pt = Enigma(config).encrypt(ct);

          std::printf("%2i   % 8f   %-4s   %-4s %-4s %-4s   %2d %2d %2d   %2d %2d %2d   %.*s\n",
                      i, score,
                      config.reflector->pretty_name.data(),
                      config.rotor1->pretty_name.data(), config.rotor2->pretty_name.data(), config.rotor3->pretty_name.data(),
                      config.ring1_pos, config.ring2_pos, config.ring3_pos,
                      config.rotor1_pos, config.rotor2_pos, config.rotor3_pos,
                      int(pt.length()), pt.c_str());
     }
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
     bool is_good_score (double score)     { return score > entries.top().score; }


     void add (double score, const EnigmaConfiguration& config)
     {
          entries.emplace(score, config);
          entries.pop();
     }


     void add (ScoreEntry e)
     {
          entries.push(std::move(e));
          entries.pop();
     }


     void print () const
     {
          std::printf("#    Score          Refl   Rotor Order      Ring Pos   Rotor Pos  Text  \n");

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
