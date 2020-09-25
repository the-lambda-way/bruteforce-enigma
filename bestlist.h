#pragma once

#include <cstdio>
#include <utility>        // std::forward
#include <functional>     // std::greater
#include <string>
#include <string_view>
#include <queue>
#include "models.h"


// priority_queue does not give access to its member container, but we need it to iterate over the entries
template <class T, class F>
class VectorQueue : public std::priority_queue<T, std::vector<T>, F>
{
public:
    const std::vector<T>& base () const    { return this->c; }
}; // class VectorQueue


struct ScoreEntry
{
     double              score;
     EnigmaConfiguration config;
     std::string         text;


     ScoreEntry ()
     : ScoreEntry(-1e12,
                  {ETW_ABCDEF, m3_I, m3_II, m3_III, UKWB, ""},
                  "(Default)")
     {}


     ScoreEntry (double score, EnigmaConfiguration config, std::string_view text)
     : score {score}, config {std::move(config)}, text {text}
     {}


     friend bool operator> (const ScoreEntry& a, const ScoreEntry& b)     { return a.score > b.score; }


     void print (int i) const
     {
          std::printf("%2i   % 8f   %-4s   %-4s %-4s %-4s   %2d %2d %2d   %2d %2d %2d   %.*s\n",
                      i, score,
                      config.reflector->pretty_name.data(),
                      config.rotor1->pretty_name.data(), config.rotor2->pretty_name.data(), config.rotor3->pretty_name.data(),
                      config.ring1_pos, config.ring2_pos, config.ring3_pos,
                      config.rotor1_pos, config.rotor2_pos, config.rotor3_pos,
                      int(text.length()), text.c_str());
     }
};


template <int N>
class BestList
{
public:
     BestList ()
     {
          ScoreEntry default_entry {};
          for (int i = 0; i < N; ++i)     entries.push(default_entry);
     }

     // Must manually check that the score should be added first, to avoid needless copying of the settings.
     bool is_good_score (double score)     { return score > entries.top().score; }


     void add (double score, const EnigmaConfiguration& config, std::string_view text)
     {
          for (const auto& entry : entries.base())
               if (score == entry.score && text == entry.text)     return;

          entries.emplace(score, config, text);
          entries.pop();

          // // Hack to print during computation
          // std::printf("% 8f   %-4s   %-4s %-4s %-4s   %2d %2d %2d   %2d %2d %2d   %.*s\n",
          //             score,
          //             config.reflector.pretty_name.data(),
          //             config.rotor1->pretty_name.data(), config.rotor2->pretty_name.data(), config.rotor3->pretty_name.data(),
          //             config.ring1_pos, config.ring2_pos, config.ring3_pos,
          //             config.rotor1_pos, config.rotor2_pos, config.rotor3_pos,
          //             int(text.length()), text.c_str());
     }


     void add (ScoreEntry e)
     {
          for (const auto& entry : entries.base())
               if (e.score == entry.score && e.text == entry.text)     return;

          entries.push(std::move(e));
          entries.pop();
     }


     void print ()
     {
          std::printf("#    Score          Refl   Rotor Order      Ring Pos   Rotor Pos  Text  \n");

          int i = 0;
          for (const ScoreEntry& entry : get_entries())
               entry.print(++i);
     }


     std::vector<ScoreEntry> get_entries ()
     {
          auto copy = entries;
          std::vector<ScoreEntry> ordered_entries;

          while (!copy.empty())
          {
               ordered_entries.push_back(copy.top());
               copy.pop();
          }

          // Typically we want the elements ordered by highest score first
          std::reverse(ordered_entries.begin(), ordered_entries.end());

          return ordered_entries;
     }


private:
     VectorQueue<ScoreEntry, std::greater<>> entries;

     template <int M, int SizeA, int SizeB>
     friend BestList<M> combine_best (const BestList<SizeA>& a, const BestList<SizeB>& b);

}; // class BestList


template <int N, int SizeA, int SizeB>
BestList<N> combine_best (const BestList<SizeA>& a, const BestList<SizeB>& b)
{
     BestList<N> out;

     for (const ScoreEntry& entry : a.entries.base())     out.add(entry);
     for (const ScoreEntry& entry : b.entries.base())     out.add(entry);

     return out;
}


template <int N, int SizeA, int SizeB, class... List>
BestList<N> combine_best (const BestList<SizeA>& a, const BestList<SizeB>& b, List&&... rest)
{
     return combine_best<N>(combine_best<N>(a, b), std::forward<List>(rest)...);
}
