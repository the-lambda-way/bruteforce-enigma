#pragma once

#include <array>
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
std::vector<std::array<int, 3>> three_permutations_of_n (int n);
std::vector<std::array<int, 3>> triples_from_n (int n);
std::vector<EnigmaBase> all_configurations_no_replace (const EnigmaModel& model);
std::vector<EnigmaBase> all_configurations (const EnigmaModel& model);


template <int N>
void test_configuration (Enigma& enigma, int* ct_ordinal, int ct_length, int* pt_ordinal, BestList<N>& scores)
{
     enigma.encrypt(ct_ordinal, ct_length, pt_ordinal);
     double score = scoreIntQgram(pt_ordinal, ct_length);

     if (scores.is_good_score(score))
          scores.add(score, enigma.get_config());
};


template <int N = 10>
BestList<N> bf_decipher (
     const EnigmaModel& model,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25, int rotor_max = 25
)
{
     const int length = ct.length();
     int       pt_ordinal[length];
     int       ct_ordinal[length];

     str_to_ordinals(ct, ct_ordinal);


     std::puts("Breaking enigma...");


     BestList<N> best {ct};

     for (const EnigmaBase& base : all_configurations(model))
     {
          Enigma enigma {base, plugboard, rotor1_start};

          // TODO: Determine which settings are equivalent, and skip

          for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor(1))
          for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_rotor(2))
          for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_rotor(3))
          for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_ring(1))
          for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_ring(2))
          // Third ring has no effect
               test_configuration(enigma, ct_ordinal, length, pt_ordinal, best);
     }

     return best;
}


template <int N = 10>
BestList<N> bf_decipher (
     const EnigmaBase& base,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25, int rotor_max = 25
)
{
     const int length = ct.length();
     int       pt_ordinal[length];
     int       ct_ordinal[length];

     str_to_ordinals(ct, ct_ordinal);


     std::puts("Breaking enigma...");


     BestList<N> best {ct};
     Enigma enigma {base, plugboard, rotor1_start};

     for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor(1))
     for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_rotor(2))
     for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_rotor(3))
     for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_ring(1))
     for (int i = 0;            i < rotor_max + 1;  ++i,     enigma.increment_ring(2))
     // Third ring has no effect
          test_configuration(enigma, ct_ordinal, length, pt_ordinal, best);

     return best;
}


template <int N>
BestList<N> bf_4_threads (const EnigmaModel& model, std::string_view plug, std::string_view ct, int rotor_end = 25)
{
     int quarter = rotor_end / 4 ;

     int a = 0 * quarter;
     int b = 1 * quarter;
     int c = 2 * quarter;
     int d = 3 * quarter;
     int e = rotor_end;
     int max = rotor_end;

     auto future1 = std::async(std::launch::async, [&] () { return bf_decipher<N>(model, plug, ct, a,     b, max); });
     auto future2 = std::async(std::launch::async, [&] () { return bf_decipher<N>(model, plug, ct, b + 1, c, max); });
     auto future3 = std::async(std::launch::async, [&] () { return bf_decipher<N>(model, plug, ct, c + 1, d, max); });
     auto future4 = std::async(std::launch::async, [&] () { return bf_decipher<N>(model, plug, ct, d + 1, e, max); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


template <int N>
BestList<N> bf_4_threads (const EnigmaBase& base, std::string_view plug, std::string_view ct, int rotor_end = 25)
{
     int quarter = rotor_end / 4 ;

     int a = 0 * quarter;
     int b = 1 * quarter;
     int c = 2 * quarter;
     int d = 3 * quarter;
     int e = rotor_end;
     int max = rotor_end;

     auto future1 = std::async(std::launch::async, [&] () { return bf_decipher<N>(base, plug, ct, a,     b, max); });
     auto future2 = std::async(std::launch::async, [&] () { return bf_decipher<N>(base, plug, ct, b + 1, c, max); });
     auto future3 = std::async(std::launch::async, [&] () { return bf_decipher<N>(base, plug, ct, c + 1, d, max); });
     auto future4 = std::async(std::launch::async, [&] () { return bf_decipher<N>(base, plug, ct, d + 1, e, max); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


// Get best rotor positions, then use those to determine best ring positions
template <int N>
BestList<N> smart_decipher (
     const EnigmaModel& model,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25
)
{
     const int length = ct.length();
     int       pt_ordinal[length];
     int       ct_ordinal[length];

     str_to_ordinals(ct, ct_ordinal);


     std::puts("Breaking enigma...");


     // Find the N best rotors
     BestList<N> best_rotors {ct};

     for (const EnigmaBase& base : all_configurations(model))
     {
          Enigma enigma {base, plugboard, 0, 0, 0, rotor1_start};

          for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor(1))
          for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor(2))
          for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor(3))
               test_configuration(enigma, ct_ordinal, length, pt_ordinal, best_rotors);
     }


     // Now find the best rings from the top results
     BestList<N> best_rings {ct};

     for (const ScoreEntry& entry : best_rotors.get_entries())
     {
          Enigma enigma {entry.config};

          for (int i = 0; i < 26; ++i,     enigma.increment_ring(1), enigma.increment_rotor(1))
          for (int i = 0; i < 26; ++i,     enigma.increment_ring(2), enigma.increment_rotor(2))
          // Third ring has no effect
               test_configuration(enigma, ct_ordinal, length, pt_ordinal, best_rings);
     }


     return best_rings;
}


template <int N>
BestList<N> smart_decipher (
     const EnigmaBase& base,
     std::string_view plugboard,
     std::string_view ct,
     int rotor1_start = 0, int rotor1_end = 25
)
{
     const int length = ct.length();
     int       pt_ordinal[length];
     int       ct_ordinal[length];

     str_to_ordinals(ct, ct_ordinal);


     std::puts("Breaking enigma...");


     BestList<N> best_rotors {ct};
     Enigma enigma {base, plugboard, 0, 0, 0, rotor1_start};

     for (int i = rotor1_start; i < rotor1_end + 1; ++i,     enigma.increment_rotor(1))
     for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor(2))
     for (int i = 0;            i < 26;             ++i,     enigma.increment_rotor(3))
          test_configuration(enigma, ct_ordinal, length, pt_ordinal, best_rotors);


     BestList<N> best_rings {ct};

     for (const ScoreEntry& entry : best_rotors.get_entries())
     {
          Enigma enigma {entry.config};

          for (int i = 0; i < 26; ++i,     enigma.increment_ring(1), enigma.increment_rotor(1))
          for (int i = 0; i < 26; ++i,     enigma.increment_ring(2), enigma.increment_rotor(2))
          // Third ring has no effect
               test_configuration(enigma, ct_ordinal, length, pt_ordinal, best_rings);
     }


     return best_rings;
}


template <int N>
BestList<N> smart_4_threads (const EnigmaModel& model, std::string_view plug, std::string_view ct)
{
     auto future1 = std::async(std::launch::async, [&] () { return smart_decipher<N>(model, plug, ct, 0,  5); });
     auto future2 = std::async(std::launch::async, [&] () { return smart_decipher<N>(model, plug, ct, 6,  11); });
     auto future3 = std::async(std::launch::async, [&] () { return smart_decipher<N>(model, plug, ct, 12, 17); });
     auto future4 = std::async(std::launch::async, [&] () { return smart_decipher<N>(model, plug, ct, 18, 25); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


template <int N>
BestList<N> smart_4_threads (const EnigmaBase& base, std::string_view plug, std::string_view ct)
{
     auto future1 = std::async(std::launch::async, [&] () { return smart_decipher<N>(base, plug, ct, 0,  5); });
     auto future2 = std::async(std::launch::async, [&] () { return smart_decipher<N>(base, plug, ct, 6,  11); });
     auto future3 = std::async(std::launch::async, [&] () { return smart_decipher<N>(base, plug, ct, 12, 17); });
     auto future4 = std::async(std::launch::async, [&] () { return smart_decipher<N>(base, plug, ct, 18, 25); });

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

