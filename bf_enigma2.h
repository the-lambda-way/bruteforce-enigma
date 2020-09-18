#pragma once

#include <array>
#include <iostream>
#include <future>
#include <string>
#include <string_view>
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


std::vector<EnigmaBase> all_configurations (const EnigmaModel& model)
{
     std::vector<EnigmaBase> out;
     // std::vector<std::array<int, 3>> rotor_permutations = three_permutations_of_n(model.rotors.size());
     std::vector<std::array<int, 3>> rotor_permutations = triples_from_n(model.rotors.size());

     for (const Rotor& reflector : model.reflectors)
     for (const std::array<int, 3> permutation : rotor_permutations)
          out.emplace_back(model.stator,
                           model.rotors[permutation[0]], model.rotors[permutation[1]], model.rotors[permutation[2]],
                           reflector);

     return out;
}


template <int N = 10>
NBestList<N> bf_decipher (
     std::string_view ct,
     std::string_view plugboard,
     const EnigmaModel& model,
     int ring1_start, int ring1_end, int ring_max = 25
)
{
     const int    length = ct.length();
     char         pt[length];
     double       score;
     NBestList<N> best;


     for (EnigmaBase base : all_configurations(model))
     {
          Enigma enigma {base, plugboard, ring1_start};

          // TODO: Determine which settings are equivalent, and skip

          // for (int ring1pos  = ring1_start; ring1pos  < ring1_end; ++ring1pos,  enigma.increment_ring(1))
          // for (int ring2pos  = 0;           ring2pos  < ring_max;  ++ring2pos,  enigma.increment_ring(2))
          // for (int ring3pos  = 0;           ring3pos  < ring_max;  ++ring3pos,  enigma.increment_ring(3))
          for (int rotor1pos = 0;           rotor1pos < ring_max;      ++rotor1pos, enigma.increment_rotor(1))
          for (int rotor2pos = 0;           rotor2pos < ring_max;      ++rotor2pos, enigma.increment_rotor(2))
          for (int rotor3pos = 0;           rotor3pos < ring_max;      ++rotor3pos, enigma.increment_rotor(3))
          {
               enigma.encrypt(pt, ct);

               score = scoreTextQgram(pt, length);

               if (best.is_good_score(score))
                    best.add(score,
                             base.rotor1.pretty_name, base.rotor2.pretty_name, base.rotor3.pretty_name,
                             rotor1pos, rotor2pos, rotor3pos,
                         //     ring1pos, ring2pos, ring3pos,
                             0, 0, 0,
                             pt);
          }
     }


     return best;
}



// // Get best rotor positions, then use those to determine best ring positions
// template <int N = 10>
// NBestList<N> smart_decipher (
//      std::string_view ct,
//      std::string_view plugboard,
//      const EnigmaModel& model,
//      int ring1_start, int ring1_end, int ring_max = 25
// )
// {
//      const int    length = ct.length();
//      char         pt[length];
//      double       score;
//      NBestList<N> best_rotors;
//      NBestList<N> best_rings;


//      for (EnigmaBase base : all_configurations(model))
//      {
//           Enigma enigma {base, plugboard, ring1_start};

//           for (int i = 0;     i < 25;     ++i, enigma.set_rotor(1, i))
//           for (int j = 0;     j < 25;     ++j, enigma.set_rotor(2, j))
//           for (int k = 0;     k < 25;     ++k, enigma.set_rotor(3, k))
//           {
//                enigma.encrypt(pt, ct);

//                score = scoreTextQgram(pt, length);

//                if (best_rotors.is_good_score(score))
//                     best_rotors.add(score,
//                                     base.rotor1.pretty_name, base.rotor2.pretty_name, base.rotor3.pretty_name,
//                                     i, j, k,
//                                     0, 0, 0,
//                                     pt);
//           }

//           for (const EnigmaConfiguration& config : best_rotors)
//           {
//                Enigma enigma {config};

//                for (int i = 0;     i < 25;     ++i, enigma.set_ring(1, i))
//                for (int j = 0;     j < 25;     ++j, enigma.set_ring(2, j))
//                for (int k = 0;     k < 25;     ++k, enigma.set_ring(3, k))
//                {
//                     enigma.encrypt(pt, ct);

//                     score = scoreTextQgram(pt, length);

//                     if (best_rotors.is_good_score(score))
//                          best_rotors.add(score,
//                                         config.rotor1.pretty_name, config.rotor2.pretty_name, config.rotor3.pretty_name,
//                                         i, j, k,
//                                         0, 0, 0,
//                                         pt);
//                }
//           }
//      }


//      return best;
// }