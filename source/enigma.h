#pragma once

#include <algorithm>     // std::copy
#include <cstring>       // std::strncpy
#include <execution>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include "datatypes.h"



// A single alphabet permutation can be represented with 130 bits. Maybe we can calculate an encryption using bit
//      twiddling and bit masking.
// Optimized memory access is done in a single operation. However, using pure calculations opens the
//      possibility of fully vectorizing an encryption.
// https://graphics.stanford.edu/~seander/bithacks.html
// https://www.chessprogramming.org/Bitboards




// ---------------------------------------------------------------------------------------------------------------------
// Rotor offsets
// ---------------------------------------------------------------------------------------------------------------------
class rotor_offsets
{
public:
     constexpr rotor_offsets (modular_int<26> rotor1, modular_int<26> ring1,
                              modular_int<26> rotor2, modular_int<26> ring2,
                              modular_int<26> rotor3, modular_int<26> ring3,
                              const std::bitset<26>& turnover1, const std::bitset<26>& turnover2)
     {
          // Rings effectively rotate rotor positions
          this->rotor1 = rotor1 - ring1;
          this->rotor2 = rotor2 - ring2;
          this->rotor3 = rotor3 - ring3;

          // Rings effectively rotate turnovers
          this->turnover1 = (turnover1 >> ring1) | (turnover1 << (26 - ring1));
          this->turnover2 = (turnover2 >> ring2) | (turnover2 << (26 - ring2));

          reset();
          precalculate_deltas();
     }

     constexpr rotor_offsets (const EnigmaKey& key)
     : rotor_offsets {key.rotor1_pos, key.ring1_pos, key.rotor2_pos, key.ring2_pos, key.rotor3_pos, key.ring3_pos,
                      key.rotor1->turnovers, key.rotor2->turnovers}
     {}


     // Rotor offsets combine rotor position and ring position
     modular_int<26> rotor1;
     modular_int<26> rotor2;
     modular_int<26> rotor3;

     // Relative positions during encryption
     modular_int<26> one;
     modular_int<26> two;
     modular_int<26> three;

     // Turnover offsets tracks the turnover position relative to the current rotor offset
     std::bitset<26> turnover1;
     std::bitset<26> turnover2;

     // Pre-calculations, modular checks avoided for efficiency
     int two_minus_one;
     int three_minus_two;


     void reset ();
     void step ();


     // Standard library doesn't have a rotate for bitsets
     void rotr_turnover1 ();
     void rotr_turnover2 ();


private:
     void precalculate_deltas ();

}; // class rotor_offsets


class rotor4_offsets
{
public:
     constexpr rotor4_offsets (modular_int<26> rotor1, modular_int<26> ring1,
                               modular_int<26> rotor2, modular_int<26> ring2,
                               modular_int<26> rotor3, modular_int<26> ring3,
                               modular_int<26> rotor4, modular_int<26> ring4,
                               const std::bitset<26>& turnover1, const std::bitset<26>& turnover2)
     {
          // Rings effectively rotate rotor positions
          this->rotor1 = rotor1 - ring1;
          this->rotor2 = rotor2 - ring2;
          this->rotor3 = rotor3 - ring3;
          this->rotor4 = rotor4 - ring4;

          // Rings effectively rotate turnovers
          this->turnover1 = (turnover1 >> ring1) | (turnover1 << (26 - ring1));
          this->turnover2 = (turnover2 >> ring2) | (turnover2 << (26 - ring2));

          reset();
          precalculate_deltas();
     }

     constexpr rotor4_offsets (const Enigma4Key& key)
     : rotor4_offsets {key.rotor1_pos, key.ring1_pos, key.rotor2_pos, key.ring2_pos, key.rotor3_pos, key.ring3_pos,
                       key.rotor4_pos, key.ring4_pos, key.rotor1->turnovers, key.rotor2->turnovers}
     {}


     // Rotor offsets combine rotor position and ring position
     modular_int<26> rotor1;
     modular_int<26> rotor2;
     modular_int<26> rotor3;
     modular_int<26> rotor4;

     // Turnover offsets tracks the turnover position relative to the current rotor offset
     std::bitset<26> turnover1;
     std::bitset<26> turnover2;

     // Relative positions during encryption
     modular_int<26> one;
     modular_int<26> two;
     modular_int<26> three;
     modular_int<26> four;

     // Pre-calculations
     int two_minus_one;
     int three_minus_two;
     int four_minus_three;

     void reset ();
     void step ();

     // Standard library doesn't have a rotate for bitsets
     void rotr_turnover1 ();
     void rotr_turnover2 ();

private:
     void precalculate_deltas ();

}; // class rotor4_offsets


// ---------------------------------------------------------------------------------------------------------------------
// Unknown positions
// ---------------------------------------------------------------------------------------------------------------------
template <class EnigmaKeyType> class Enigma_optimize_unknown_positions {};


template<>
class Enigma_optimize_unknown_positions<EnigmaKey>
{
public:
     Enigma_optimize_unknown_positions (
          const basic_enigma_base<EnigmaKey>& base,
          const Plugboard& plugboard,
          int rotor1_start = 0,
          int rotor2_start = 0,
          int rotor3_start = 0,
          int ring1_start  = 0,
          int ring2_start  = 0,
          int ring3_start  = 0
     );


     Enigma_optimize_unknown_positions (EnigmaKey key);


     template <std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O>
          requires std::same_as<std::iter_value_t<I>, int> && std::same_as<std::iter_value_t<O>, int>
     void encrypt (I first, S last, O result)
     {
          offsets.reset();

          // std::transform does not ensure in-order traversal; std::for_each does not have an output parameter
          for (; first != last; ++first, ++result)
               *result = encrypt_ordinal(*first);
     }


     template <std::ranges::input_range R, std::weakly_incrementable O>
          requires std::same_as<std::ranges::range_value_t<R>, int> && std::same_as<std::iter_value_t<O>, int>
     void encrypt (R&& r, O result)
     {
          return encrypt(std::ranges::begin(r), std::ranges::end(r), std::move(result));
     }


     EnigmaKey get_key () const;

     void increment_ring1 ();
     void increment_ring2 ();
     void increment_ring3 ();
     void increment_rotor1 ();
     void increment_rotor2 ();
     void increment_rotor3 ();


private:
     EnigmaKey key;
     rotor_offsets offsets;

     std::array<int, 26> plugboard_stator_forward;
     std::array<int, 52> rotor1_forward;
     std::array<int, 78> rotor2_forward;
     std::array<int, 78> rotor3_forward;
     std::array<int, 52> reflector;
     std::array<int, 52> rotor3_reverse;
     std::array<int, 78> rotor2_reverse;
     std::array<int, 78> rotor1_reverse;
     std::array<int, 52> stator_plugboard_reverse;

     int encrypt_ordinal (int j);

}; // class Enigma_optimize_unknown_positions<EnigmaKey>


template<>
class Enigma_optimize_unknown_positions<Enigma4Key>
{
public:
     Enigma_optimize_unknown_positions (
          const basic_enigma_base<Enigma4Key>& base,
          const Plugboard& plugboard,
          int rotor1_start = 0,
          int rotor2_start = 0,
          int rotor3_start = 0,
          int rotor4_start = 0,
          int ring1_start  = 0,
          int ring2_start  = 0,
          int ring3_start  = 0,
          int ring4_start  = 0
     );


     Enigma_optimize_unknown_positions (Enigma4Key key);


     template <std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O>
          requires std::same_as<std::iter_value_t<I>, int> && std::same_as<std::iter_value_t<O>, int>
     void encrypt (I first, S last, O result)
     {
          offsets.reset();

          // std::transform does not ensure in-order traversal; std::for_each does not have an output parameter
          for (; first != last; ++first, ++result)
               *result = encrypt_ordinal(*first);
     }


     template <std::ranges::input_range R, std::weakly_incrementable O>
          requires std::same_as<std::ranges::range_value_t<R>, int> && std::same_as<std::iter_value_t<O>, int>
     void encrypt (R&& r, O result)
     {
          return encrypt(std::ranges::begin(r), std::ranges::end(r), std::move(result));
     }


     Enigma4Key get_key () const;

     void increment_ring1 ();
     void increment_ring2 ();
     void increment_ring3 ();
     void increment_ring4 ();
     void increment_rotor1 ();
     void increment_rotor2 ();
     void increment_rotor3 ();
     void increment_rotor4 ();


private:
     Enigma4Key key;
     rotor4_offsets offsets;

     std::array<int, 26> plugboard_stator_forward;
     std::array<int, 52> rotor1_forward;
     std::array<int, 78> rotor2_forward;
     std::array<int, 78> rotor3_forward;
     std::array<int, 78> rotor4_forward;
     std::array<int, 52> reflector;
     std::array<int, 52> rotor4_reverse;
     std::array<int, 78> rotor3_reverse;
     std::array<int, 78> rotor2_reverse;
     std::array<int, 78> rotor1_reverse;
     std::array<int, 52> stator_plugboard_reverse;

     int encrypt_ordinal (int j);

}; // class Enigma_optimize_unknown_positions<Enigma4Key>


// class Enigma_optimize_unknown_plugboard
// {
//      // TODO: provide an efficient way to iterate through plugboard permutations
// }; // class Enigma_optimize_unknown_plugboard

