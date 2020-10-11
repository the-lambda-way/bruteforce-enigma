#pragma once

#include <array>
#include <functional>      // std::invoke
#include <future>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "bestlist.h"
#include "enigma.h"
#include "models.h"        // for automatic import into user code
#include "rotors.h"
#include "score.h"


std::string convert_to_ct (std::string_view in);
std::string convert_to_plug (std::string_view pairs);
std::vector<std::array<int, 3>> three_permutations_of_n (int n);
std::vector<std::array<int, 3>> triples_from_n (int n);
std::vector<EnigmaBase> all_configurations_no_replace (const EnigmaModel& model);
std::vector<EnigmaBase> all_configurations (const EnigmaModel& model);


template <class EnigmaType, int N, class F>
void test_configuration (EnigmaType& enigma,
                         std::span<const int> ct_ordinal, std::span<int> pt_ordinal,
                         BestList<N>& scores,
                         F scoring_function = score_by_Qgram)
{
     enigma.encrypt(ct_ordinal, pt_ordinal.begin());
     double score = std::invoke(scoring_function, pt_ordinal);

     if (scores.is_good_score(score))
          scores.add(score, enigma.get_key());
};


// TODO: Delegate this to the "4_threads" function and divide the tasks by configurations instead of rotor positions.
// Will take 1/4 the space costs.

template <int N = 10, class F = decltype(score_by_Qgram)>
BestList<N> bf_decipher (
     const EnigmaModel& model,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25, int rotor_max = 25,
     F scoring_function = score_by_Qgram
)
{
     std::vector<int> pt_ordinal(ct.length());
     const std::vector<int> ct_ordinal = str_to_ordinals(ct);


     std::puts("Breaking enigma...");


     BestList<N> best {ct};

     for (const EnigmaBase& base : all_configurations(model))
     {
          Enigma_optimize_unknown_positions enigma {base, plugboard, modular_int<26>{rotor1_start}};

          // TODO: Determine which settings are equivalent, and skip

          for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor1())
          for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_rotor2())
          for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_rotor3())
          for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_ring1())
          for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_ring2())
          // Third ring has no effect
               test_configuration(enigma, ct_ordinal, pt_ordinal, best, scoring_function);
     }

     return best;
}


template <int N = 10, class F = decltype(score_by_Qgram)>
BestList<N> bf_decipher (
     const EnigmaBase& base,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25, int rotor_max = 25,
     F scoring_function = score_by_Qgram
)
{
     std::vector<int> pt_ordinal(ct.length());
     const std::vector<int> ct_ordinal = str_to_ordinals(ct);


     std::puts("Breaking enigma...");


     BestList<N> best {ct};
     Enigma_optimize_unknown_positions enigma {base, plugboard, modular_int<26>{rotor1_start}};

     for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor1())
     for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_rotor2())
     for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_rotor3())
     for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_ring1())
     for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_ring2())
     // Third ring has no effect
          test_configuration(enigma, ct_ordinal, pt_ordinal, best, scoring_function);

     return best;
}


template <int N, class F = decltype(score_by_Qgram)>
BestList<N> bf_4_threads (const EnigmaModel& model, std::string_view plug, std::string_view ct,
                          int rotor_max = 25, F scoring_function = score_by_Qgram)
{
     int quarter = rotor_max / 4 ;

     int a = 0 * quarter;
     int b = 1 * quarter;
     int c = 2 * quarter;
     int d = 3 * quarter;
     int e = rotor_max;
     int max = rotor_max;

     auto future1 = std::async(std::launch::async,
                               [&] () { return bf_decipher<N>(model, plug, ct, a,     b, max, scoring_function); });
     auto future2 = std::async(std::launch::async,
                               [&] () { return bf_decipher<N>(model, plug, ct, b + 1, c, max, scoring_function); });
     auto future3 = std::async(std::launch::async,
                               [&] () { return bf_decipher<N>(model, plug, ct, c + 1, d, max, scoring_function); });
     auto future4 = std::async(std::launch::async,
                               [&] () { return bf_decipher<N>(model, plug, ct, d + 1, e, max, scoring_function); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


template <int N, class F = decltype(score_by_Qgram)>
BestList<N> bf_4_threads (const EnigmaBase& base, std::string_view plug, std::string_view ct,
                          int rotor_max = 25, F scoring_function = score_by_Qgram)
{
     int quarter = rotor_max / 4 ;

     int a = 0 * quarter;
     int b = 1 * quarter;
     int c = 2 * quarter;
     int d = 3 * quarter;
     int e = rotor_max;
     int max = rotor_max;

     auto future1 = std::async(std::launch::async,
                               [&] () { return bf_decipher<N>(base, plug, ct, a,     b, max, scoring_function); });
     auto future2 = std::async(std::launch::async,
                               [&] () { return bf_decipher<N>(base, plug, ct, b + 1, c, max, scoring_function); });
     auto future3 = std::async(std::launch::async,
                               [&] () { return bf_decipher<N>(base, plug, ct, c + 1, d, max, scoring_function); });
     auto future4 = std::async(std::launch::async,
                               [&] () { return bf_decipher<N>(base, plug, ct, d + 1, e, max, scoring_function); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


// Get best rotor positions, then use those to determine best ring positions
template <int N, class F = decltype(score_by_Qgram)>
BestList<N> smart_decipher (
     const EnigmaModel& model,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25,
     F scoring_function = score_by_Qgram
)
{
     std::vector<int> pt_ordinal(ct.length());
     const std::vector<int> ct_ordinal = str_to_ordinals(ct);


     std::puts("Breaking enigma...");


     // Find the N best rotors
     BestList<N> best_rotors {ct};

     for (const EnigmaBase& base : all_configurations(model))
     {
          Enigma_optimize_unknown_positions enigma {base, plugboard, modular_int<26>{rotor1_start}};

          for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor1())
          for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor2())
          for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor3())
               test_configuration(enigma, ct_ordinal, pt_ordinal, best_rotors, scoring_function);
     }


     // Now find the best rings from the top results
     BestList<N> best_rings {ct};

     for (const ScoreEntry& entry : best_rotors.get_entries())
     {
          Enigma_optimize_unknown_positions enigma {entry.key};

          for (int i = 0; i < 26; ++i,     enigma.increment_ring1(), enigma.increment_rotor1())
          for (int i = 0; i < 26; ++i,     enigma.increment_ring2(), enigma.increment_rotor2())
          // Third ring has no effect
               test_configuration(enigma, ct_ordinal, pt_ordinal, best_rings, scoring_function);
     }


     return best_rings;
}


template <int N, class F = decltype(score_by_Qgram)>
BestList<N> smart_decipher (
     const EnigmaBase& base,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25,
     F scoring_function = score_by_Qgram
)
{
     std::vector<int> pt_ordinal(ct.length());
     const std::vector<int> ct_ordinal = str_to_ordinals(ct);


     std::puts("Breaking enigma...");


     BestList<N> best_rotors {ct};
     Enigma_optimize_unknown_positions enigma {base, plugboard, modular_int<26>{rotor1_start}};

     for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor1())
     for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor2())
     for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor3())
          test_configuration(enigma, ct_ordinal, pt_ordinal, best_rotors, scoring_function);


     BestList<N> best_rings {ct};

     for (const ScoreEntry& entry : best_rotors.get_entries())
     {
          Enigma_optimize_unknown_positions enigma {entry.key};

          for (int i = 0; i < 26; ++i,     enigma.increment_ring1(), enigma.increment_rotor1())
          for (int i = 0; i < 26; ++i,     enigma.increment_ring2(), enigma.increment_rotor2())
          // Third ring has no effect
               test_configuration(enigma, ct_ordinal, pt_ordinal, best_rings, scoring_function);
     }


     return best_rings;
}


template <int N, class F = decltype(score_by_Qgram)>
BestList<N> smart_4_threads (const EnigmaModel& model, std::string_view plug, std::string_view ct,
                             F scoring_function = score_by_Qgram)
{
     auto future1 = std::async(std::launch::async,
                               [&] () { return smart_decipher<N>(model, plug, ct, 0,  5 , scoring_function); });
     auto future2 = std::async(std::launch::async,
                               [&] () { return smart_decipher<N>(model, plug, ct, 6,  11, scoring_function); });
     auto future3 = std::async(std::launch::async,
                               [&] () { return smart_decipher<N>(model, plug, ct, 12, 17, scoring_function); });
     auto future4 = std::async(std::launch::async,
                               [&] () { return smart_decipher<N>(model, plug, ct, 18, 25, scoring_function); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


template <int N, class F = decltype(score_by_Qgram)>
BestList<N> smart_4_threads (const EnigmaBase& base, std::string_view plug, std::string_view ct,
                             F scoring_function = score_by_Qgram)
{
     auto future1 = std::async(std::launch::async,
                               [&] () { return smart_decipher<N>(base, plug, ct, 0,  5 , scoring_function); });
     auto future2 = std::async(std::launch::async,
                               [&] () { return smart_decipher<N>(base, plug, ct, 6,  11, scoring_function); });
     auto future3 = std::async(std::launch::async,
                               [&] () { return smart_decipher<N>(base, plug, ct, 12, 17, scoring_function); });
     auto future4 = std::async(std::launch::async,
                               [&] () { return smart_decipher<N>(base, plug, ct, 18, 25, scoring_function); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


// TODO: Add a fully parallelized algorithm.
// Thoughts:
//      * Create a closed-form solution to the rotor offset at each character position during an encryption.
//      * Find a way to collect good-scoring results without requiring a lock. That would let us use the C++ unsequenced
//        policy.
//      * Separate rotors memory from positions and rings, so that calculations are separated from memory ownership.
//      * Maybe a map-reduce design would help.


// TODO: Add a secondary encryption step. Pass in a function that gets called on a encrypted text before it gets scored.

// TODO: No letter can map to itself. So if we find a key that decrypts a letter to the same as the given ciphertext, it
//      can be rejected. Maybe this can be made an optimization.
