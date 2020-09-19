#pragma once

#include <array>
#include <cstdio>
#include <iostream>
#include <future>
#include <string>
#include <string_view>
#include <vector>
#include "bestlist2.h"
#include "enigma.h"
#include "enigma_models.h"
#include "scoreText.h"


std::string convert_to_ct (std::string_view in)
{
     std::string s;

     for (char c : in)
     {
          if      ('a' <= c && c <= 'z')     s += (c - 32);
          else if ('A' <= c && c <= 'Z')     s += c;
     }

     return s;
}


std::vector<std::array<int, 3>> three_permutations_of_n (int n)
{
     std::vector<std::array<int, 3>> out;

     int i, j, k;
     int* permutations[6][3] = {{&i, &j, &k}, {&i, &k, &j}, {&j, &i, &k}, {&j, &k, &i}, {&k, &i, &j}, {&k, &j, &i}};

     for (i = 0;     i < n - 2; ++i)
     for (j = i + 1; j < n - 1; ++j)
     for (k = j + 1; k < n;     ++k)
     for (auto p : permutations)
          // Note: emplace_back doesn't work with array types, which require brace-enclosed initialization
          out.push_back(std::move(std::array<int, 3> {*p[0], *p[1], *p[2]}));

     return out;
}


std::vector<std::array<int, 3>> triples_from_n (int n)
{
     std::vector<std::array<int, 3>> out;

     for (int i = 0; i < n; ++i)
     for (int j = 0; j < n; ++j)
     for (int k = 0; k < n; ++k)
          out.push_back(std::move(std::array<int, 3> {i, j, k}));

     return out;
}


std::vector<EnigmaBase> all_configurations_no_replace (const EnigmaModel& model)
{
     std::vector<EnigmaBase> out;
     std::vector<std::array<int, 3>> rotor_permutations = three_permutations_of_n(model.rotors.size());

     for (const Rotor* reflector : model.reflectors)
     for (const std::array<int, 3> permutation : rotor_permutations)
          out.emplace_back(*model.stator,
                           *model.rotors[permutation[0]], *model.rotors[permutation[1]], *model.rotors[permutation[2]],
                           *reflector);

     return out;
}


std::vector<EnigmaBase> all_configurations (const EnigmaModel& model)
{
     std::vector<EnigmaBase> out;
     std::vector<std::array<int, 3>> rotor_permutations = triples_from_n(model.rotors.size());

     for (const Rotor* reflector : model.reflectors)
     for (const std::array<int, 3> permutation : rotor_permutations)
          out.emplace_back(*model.stator,
                           *model.rotors[permutation[0]], *model.rotors[permutation[1]], *model.rotors[permutation[2]],
                           *reflector);

     return out;
}


void str_to_ordinals (int* out, std::string_view str)
{
     for (int i = 0; i < str.length(); ++i)     out[i] = str[i] - 'A';
}


template <int N = 10>
NBestList<N> bf_decipher (
     const EnigmaModel& model,
     std::string_view plugboard,
     std::string_view ct,
     int ring1_start = 0, int ring1_end = 25, int ring_max = 25
)
{
     const int    length = ct.length();
     char         pt[length];
     double       score;
     NBestList<N> best;

     int ct_ordinal[length];
     str_to_ordinals(ct_ordinal, ct);

     std::puts("Breaking enigma...");


     for (const EnigmaBase& base : all_configurations(model))
     {
          Enigma enigma {base, plugboard, ring1_start};

          // TODO: Determine which settings are equivalent, and skip

          for (int i = ring1_start;     i < ring1_end + 1;     ++i, enigma.increment_ring(1))
          for (int i = 0;               i < ring_max + 1;      ++i, enigma.increment_ring(2))
          for (int i = 0;               i < ring_max + 1;      ++i, enigma.increment_ring(3))
          for (int i = 0;               i < ring_max + 1;      ++i, enigma.increment_rotor(1))
          for (int i = 0;               i < ring_max + 1;      ++i, enigma.increment_rotor(2))
          for (int i = 0;               i < ring_max + 1;      ++i, enigma.increment_rotor(3))
          {
               enigma.encrypt(pt, ct_ordinal, length);

               // TODO: score based on ordinals instead of characters
               score = scoreTextQgram(pt, length);

               if (best.is_good_score(score))
                    best.add(score, enigma.get_config(), pt);
          }
     }


     return best;
}



// Get best rotor positions, then use those to determine best ring positions
NBestList<5> smart_decipher (
     const EnigmaModel& model,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25
)
{
     const int    length = ct.length();
     char         pt[length];
     double       score;
     NBestList<5> best_rotors;
     NBestList<5> best_rings;

     int ct_ordinal[length];
     str_to_ordinals(ct_ordinal, ct);

     std::puts("Breaking enigma...");


     // Find the N best rotors
     for (const EnigmaBase& base : all_configurations(model))
     {
          Enigma enigma {base, plugboard, 0, 0, 0, rotor1_start};

          for (int i = rotor1_start;     i < rotor1_end + 1;     ++i, enigma.increment_rotor(1))
          for (int i = 0;                i < 26;                 ++i, enigma.increment_rotor(2))
          for (int i = 0;                i < 26;                 ++i, enigma.increment_rotor(3))
          {
               enigma.encrypt(pt, ct_ordinal, length);

               score = scoreTextQgram(pt, length);

               if (best_rotors.is_good_score(score))
                    best_rotors.add(score, enigma.get_config(), pt);
          }
     }


     // Now find the best rings from the top result
     std::vector<ScoreEntry> top_rotors = best_rotors.get_entries();
     Enigma enigma {top_rotors[0].config};

     for (int i = 0;     i < 26;     ++i, enigma.increment_ring(1), enigma.increment_rotor(1))
     for (int i = 0;     i < 26;     ++i, enigma.increment_ring(2), enigma.increment_rotor(2))
     for (int i = 0;     i < 26;     ++i, enigma.increment_ring(3), enigma.increment_rotor(3))
     {
          enigma.encrypt(pt, ct_ordinal, length);

          score = scoreTextQgram(pt, length);

          if (best_rings.is_good_score(score))
               best_rings.add(score, enigma.get_config(), pt);
     }


     return best_rings;
}