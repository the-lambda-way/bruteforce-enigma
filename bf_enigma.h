#pragma once

#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "bestlist.h"
#include "enigma.h"
#include "rotors.h"
#include "score.h"


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


void str_from_ordinals (std::string& out, int* ordinals, int length)
{
     for (int i = 0; i < length; ++i)     out[i] = ordinals[i] + 'A';
}


template <int N = 10>
BestList<N> bf_decipher (
     const EnigmaModel& model,
     std::string_view plugboard,
     std::string_view ct,
     int ring1_start = 0, int ring1_end = 25, int ring_max = 25
)
{
     double    score;
     const int length = ct.length();
     int       pt_ordinal[length];
     int       ct_ordinal[length];

     str_to_ordinals(ct_ordinal, ct);


     std::puts("Breaking enigma...");


     BestList<N> best {ct};

     for (const EnigmaBase& base : all_configurations(model))
     {
          Enigma enigma {base, plugboard, ring1_start};

          // TODO: Determine which settings are equivalent, and skip

          for (int i = ring1_start; i < ring1_end + 1; ++i,     enigma.increment_ring(1))
          for (int i = 0;           i < ring_max + 1;  ++i,     enigma.increment_ring(2))
          for (int i = 0;           i < ring_max + 1;  ++i,     enigma.increment_ring(3))
          for (int i = 0;           i < ring_max + 1;  ++i,     enigma.increment_rotor(1))
          for (int i = 0;           i < ring_max + 1;  ++i,     enigma.increment_rotor(2))
          for (int i = 0;           i < ring_max + 1;  ++i,     enigma.increment_rotor(3))
          {
               enigma.encrypt(pt_ordinal, ct_ordinal, length);
               score = scoreIntQgram(pt_ordinal, length);

               if (best.is_good_score(score))
                    best.add(score, enigma.get_config());
          }
     }


     return best;
}


// TODO: It's probably faster if we don't save intermediate plaintexts. The can be regerated from the config.


// Get best rotor positions, then use those to determine best ring positions
template <int N>
BestList<N> smart_decipher (
     const EnigmaModel& model,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25
)
{
     double    score;
     const int length = ct.length();
     int       pt_ordinal[length];
     int       ct_ordinal[length];

     str_to_ordinals(ct_ordinal, ct);


     std::puts("Breaking enigma...");


     // Find the N best rotors
     BestList<N> best_rotors {ct};

     for (const EnigmaBase& base : all_configurations(model))
     {
          Enigma enigma {base, plugboard, 0, 0, 0, rotor1_start};

          for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor(1))
          for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor(2))
          for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor(3))
          {
               enigma.encrypt(pt_ordinal, ct_ordinal, length);
               score = scoreIntQgram(pt_ordinal, length);

               if (best_rotors.is_good_score(score))
                    best_rotors.add(score, enigma.get_config());
          }
     }


     // Now find the best rings from the top results
     BestList<N> best_rings {ct};

     for (const ScoreEntry& entry : best_rotors.get_entries())
     {
          Enigma enigma {entry.config};

          for (int i = 0; i < 26; ++i,     enigma.increment_ring(1), enigma.increment_rotor(1))
          for (int i = 0; i < 26; ++i,     enigma.increment_ring(2), enigma.increment_rotor(2))
          // Third ring has no effect
          {
               enigma.encrypt(pt_ordinal, ct_ordinal, length);
               score = scoreIntQgram(pt_ordinal, length);

               if (best_rings.is_good_score(score))
                    best_rings.add(score, enigma.get_config());
          }
     }


     return best_rings;
}


// TODO: Add a parallelized algorithm.
// Thoughts:
//      * Create a closed-form solution to the rotor offset at each character position during an encryption.
//      * Find a way to collect good-scoring results without requiring a lock. That would let us use the C++ unsequenced
//        policy.
//      * Separate rotors memory from positions and rings, so that calculations are separated from memory ownership.
//      * Research whether precalculating all rotor positions would create vectorization opportunities.


// TODO: Add a secondary encryption step. Pass in a function that gets called on a encrypted text before it gets scored.

