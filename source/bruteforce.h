#pragma once

#include <array>
#include <functional>     // std::invoke
#include <future>
#include <iostream>
#include <ranges>
#include <string_view>
#include <vector>
#include "leaderboard.h"
#include "enigma.h"
#include "datatypes.h"
#include "score.h"


// ---------------------------------------------------------------------------------------------------------------------
// Common
// ---------------------------------------------------------------------------------------------------------------------
std::vector<std::array<int, 3>> triples_from_n (int n);
std::vector<EnigmaBase> all_configurations (const EnigmaModel& model);
std::vector<Enigma4Base> all_configurations (const Enigma4Model& model);


template <class EnigmaType, std::ranges::input_range R1,
          std::ranges::forward_range R2, class LeaderboardType,
          class F>
void test_enigma_configuration (
     EnigmaType& enigma, const R1& ct_ordinals, R2& pt_ordinals, LeaderboardType& scores, F scoring_function
)
{
     enigma.encrypt(ct_ordinals, std::ranges::begin(pt_ordinals));

     double score = std::invoke(scoring_function, pt_ordinals);

     if (scores.is_good_score(score))
          scores.add(score, enigma.get_key());
};


// ---------------------------------------------------------------------------------------------------------------------
// Bruteforce
// ---------------------------------------------------------------------------------------------------------------------
template <std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
void bruteforce_positions (
     const basic_enigma_base<EnigmaKey>& base, const Plugboard& plugboard, const R& ct_ordinals,
     Leaderboard<N, EnigmaKey>& scores,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     std::vector<int> pt_ordinals(std::ranges::size(ct_ordinals), 0);
     Enigma_optimize_unknown_positions<EnigmaKey> enigma {base, plugboard};

     for (int i = 0; i < 26; ++i,     enigma.increment_rotor1())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor2())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor3())
     for (int i = 0; i < 26; ++i,     enigma.increment_ring1())
     for (int i = 0; i < 26; ++i,     enigma.increment_ring2())
     // Third ring has no effect
          test_enigma_configuration(enigma, ct_ordinals, pt_ordinals, scores, scoring_function);
}


// Useful for multithreading a single configuration.
template <std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
Leaderboard<N, EnigmaKey> bruteforce_positions (
     const basic_enigma_base<EnigmaKey>& base, const Plugboard& plugboard, const R& ct_ordinals,
     F scoring_function = score_by_Qgram<std::vector<int>>, int rotor1_start = 0, int rotor1_end = 26
)
{
     Leaderboard<N, EnigmaKey> scores;
     std::vector<int> pt_ordinals(std::ranges::size(ct_ordinals), 0);
     Enigma_optimize_unknown_positions<EnigmaKey> enigma {base, plugboard, rotor1_start};

     for (int i = rotor1_start; i < rotor1_end; ++i,     enigma.increment_rotor1())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_rotor2())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_rotor3())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_ring1())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_ring2())
     // Third ring has no effect
          test_enigma_configuration(enigma, ct_ordinals, pt_ordinals, scores, scoring_function);

     return scores;
}


template <std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
void bruteforce_positions (
     const basic_enigma_base<Enigma4Key>& base, const Plugboard& plugboard, const R& ct_ordinals,
     Leaderboard<N, Enigma4Key>& scores,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     std::vector<int> pt_ordinals(std::ranges::size(ct_ordinals), 0);
     Enigma_optimize_unknown_positions<Enigma4Key> enigma {base, plugboard};

     for (int i = 0; i < 26; ++i,     enigma.increment_rotor1())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor2())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor3())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor4())
     for (int i = 0; i < 26; ++i,     enigma.increment_ring1())
     for (int i = 0; i < 26; ++i,     enigma.increment_ring2())
     // Third and fourth rings have no effect
          test_enigma_configuration(enigma, ct_ordinals, pt_ordinals, scores, scoring_function);
}


// Useful for multithreading a single configuration.
template <std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
Leaderboard<N, Enigma4Key> bruteforce_positions (
     const basic_enigma_base<Enigma4Key>& base, const Plugboard& plugboard, const R& ct_ordinals,
     F scoring_function = score_by_Qgram<std::vector<int>>, int rotor1_start = 0, int rotor1_end = 26
)
{
     Leaderboard<N, Enigma4Key> scores;
     std::vector<int> pt_ordinals(std::ranges::size(ct_ordinals), 0);
     Enigma_optimize_unknown_positions<Enigma4Key> enigma {base, plugboard, rotor1_start};

     for (int i = rotor1_start; i < rotor1_end; ++i,     enigma.increment_rotor1())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_rotor2())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_rotor3())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_rotor4())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_ring1())
     for (int i = 0;            i < 26;         ++i,     enigma.increment_ring2())
     // Third and fourth rings have no effect
          test_enigma_configuration(enigma, ct_ordinals, pt_ordinals, scores, scoring_function);

     return scores;
}


// Useful for multithreading mutliple bases.
template <class KeyType, std::ranges::forward_range R1, std::ranges::forward_range R2,
          int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R1>, basic_enigma_base<KeyType>>
Leaderboard<N, KeyType> bruteforce_positions (
     const R1& bases, const Plugboard& plugboard, const R2& ct_ordinals,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     Leaderboard<N, KeyType> scores;

     for (const basic_enigma_base<KeyType>& base : bases)
          bruteforce_positions(base, plugboard, ct_ordinals, scores, scoring_function);

     return scores;
}


// ---------------------------------------------------------------------------------------------------------------------
// Smart
// ---------------------------------------------------------------------------------------------------------------------
template <std::ranges::forward_range R, int N, class F = decltype(score_by_Qgram<std::vector<int>>)>
void smart_decipher_positions (
     const basic_enigma_base<EnigmaKey>& base, const Plugboard& plugboard, const R& ct_ordinals,
     Leaderboard<N, EnigmaKey>& scores,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     std::vector<int> pt_ordinals(std::ranges::size(ct_ordinals), 0);
     Enigma_optimize_unknown_positions<EnigmaKey> enigma {base, plugboard};

     Leaderboard<N, EnigmaKey> rotor_scores;

     for (int i = 0; i < 26; ++i,     enigma.increment_rotor1())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor2())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor3())
          test_enigma_configuration(enigma, ct_ordinals, pt_ordinals, rotor_scores, scoring_function);


     for (const auto& entry : rotor_scores.get_entries())
     {
          Enigma_optimize_unknown_positions<EnigmaKey> enigma {entry.id};

          for (int i = 0; i < 26; ++i,     enigma.increment_ring1(), enigma.increment_rotor1())
          for (int i = 0; i < 26; ++i,     enigma.increment_ring2(), enigma.increment_rotor2())
          // Third ring has no effect
               test_enigma_configuration(enigma, ct_ordinals, pt_ordinals, scores, scoring_function);
     }
}


template <std::ranges::forward_range R, int N, class F = decltype(score_by_Qgram<std::vector<int>>)>
void smart_decipher_positions (
     const basic_enigma_base<Enigma4Key>& base, const Plugboard& plugboard, const R& ct_ordinals,
     Leaderboard<N, Enigma4Key>& scores,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     std::vector<int> pt_ordinals(std::ranges::size(ct_ordinals), 0);
     Enigma_optimize_unknown_positions<Enigma4Key> enigma {base, plugboard};

     Leaderboard<N, Enigma4Key> rotor_scores;

     for (int i = 0; i < 26; ++i,     enigma.increment_rotor1())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor2())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor3())
     for (int i = 0; i < 26; ++i,     enigma.increment_rotor4())
          test_enigma_configuration(enigma, ct_ordinals, pt_ordinals, rotor_scores, scoring_function);


     for (const auto& entry : rotor_scores.get_entries())
     {
          Enigma_optimize_unknown_positions<Enigma4Key> enigma {entry.id};

          for (int i = 0; i < 26; ++i,     enigma.increment_ring1(), enigma.increment_rotor1())
          for (int i = 0; i < 26; ++i,     enigma.increment_ring2(), enigma.increment_rotor2())
          // Third and fourth rings have no effect
               test_enigma_configuration(enigma, ct_ordinals, pt_ordinals, scores, scoring_function);
     }
}


// Useful for a single configuration.
template <class KeyType, std::ranges::input_range R, int N, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R>, int>
Leaderboard<N, KeyType> smart_decipher_positions (
     const basic_enigma_base<KeyType>& base, const Plugboard& plugboard, const R& ct_ordinals,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     Leaderboard<N, KeyType> scores;
     smart_decipher_positions(base, plugboard, ct_ordinals, scores);

     return scores;
}


// Useful for multithreading mutliple bases.
template <class KeyType, std::ranges::input_range R1, std::ranges::input_range R2,
          int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R1>, basic_enigma_base<KeyType>>
Leaderboard<N, KeyType> smart_decipher_positions (
     const R1& bases, const Plugboard& plugboard, const R2& ct_ordinals,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     Leaderboard<N, KeyType> scores;

     for (const basic_enigma_base<KeyType>& base : bases)
          smart_decipher_positions(base, plugboard, ct_ordinals, scores, scoring_function);

     return scores;
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
