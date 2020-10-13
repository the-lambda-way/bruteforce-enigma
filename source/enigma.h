#pragma once

#include <algorithm>     // std::copy
#include <cstring>       // std::strncpy
#include <execution>
#include <iterator>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include "rotors.h"


std::vector<int> str_to_ordinals (std::string_view str);
std::string ordinals_to_str (std::span<const int> ordinals);



// A single alphabet permutation can be represented with 130 bits. Maybe we can calculate an encryption using bit
//      twiddling and bit masking.
// Optimized memory access is done in a single operation. However, using pure calculations opens the
//      possibility of fully vectorizing an encryption.
// https://graphics.stanford.edu/~seander/bithacks.html




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

     // Pre-calculations
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
class Enigma_optimize_unknown_positions
{
public:
     // Use rotor and ring start to divide up thread jobs.
     constexpr Enigma_optimize_unknown_positions (
          const EnigmaBase& base,
          std::string_view plugboard,
          modular_int<26> rotor1_start = modular_int<26>{0},
          modular_int<26> rotor2_start = modular_int<26>{0},
          modular_int<26> rotor3_start = modular_int<26>{0},
          modular_int<26> ring1_start  = modular_int<26>{0},
          modular_int<26> ring2_start  = modular_int<26>{0},
          modular_int<26> ring3_start  = modular_int<26>{0}
     )
     : Enigma_optimize_unknown_positions {
          EnigmaKey {base, plugboard, rotor1_start, rotor2_start, rotor3_start, ring1_start, ring2_start, ring3_start}}
     {}


     constexpr Enigma_optimize_unknown_positions (EnigmaKey key)
     : key {std::move(key)}, offsets {this->key}
     {
          // the plugboard_stator_forward should have the range 0 <= i < 26
          std::transform(
               std::execution::par_unseq,
               this->key.plugboard.begin(), this->key.plugboard.end(),
               plugboard_stator_forward.begin(),
               [this] (char c) { return this->key.stator->forward[c - 'A'] - 26; }
          );

          // rotors keep the range 26 <= i < 52
          std::copy(std::execution::par_unseq,
                    this->key.rotor1->forward.begin(), this->key.rotor1->forward.begin() + rotor1_forward.size(),
                    rotor1_forward.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor2->forward.begin(), this->key.rotor2->forward.end(),
                    rotor2_forward.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor3->forward.begin(), this->key.rotor3->forward.end(),
                    rotor3_forward.begin());

          // the reflector should have the range 0 <= i < 26
          std::transform(
               std::execution::par_unseq,
               this->key.reflector->forward.begin(), this->key.reflector->forward.begin() + reflector.size(),
               reflector.begin(),
               [] (int o) { return o - 26; }
          );

          // rotors keep the range 26 <= i < 52
          std::copy(std::execution::par_unseq,
                    this->key.rotor3->reverse.begin(), this->key.rotor3->reverse.begin() + rotor3_reverse.size(),
                    rotor3_reverse.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor2->reverse.begin(), this->key.rotor2->reverse.end(),
                    rotor2_reverse.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor1->reverse.begin(), this->key.rotor1->reverse.end(),
                    rotor1_reverse.begin());

          // the plugboard_stator_forward should have the range 0 <= i < 26
          std::transform(
               std::execution::par_unseq,
               this->key.stator->reverse.begin(), this->key.stator->reverse.begin() + 26,
               stator_plugboard_reverse.begin(),
               [this] (int o) { return this->key.plugboard[o - 26] - 'A'; }
          );

          std::copy(std::execution::par_unseq,
                    stator_plugboard_reverse.begin(), stator_plugboard_reverse.begin() + 26,
                    stator_plugboard_reverse.begin() + 26);
     }


     template <std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O>
          requires std::same_as<std::iter_value_t<I>, int> && std::same_as<std::iter_value_t<O>, int>
     void encrypt (I first, S last, O result)
     {
          offsets.reset();

          // std::transform does not ensure in-order traversal; std::for_each does not have an output parameter
          for (; first != last; ++first, (void)++result)
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

}; // class Enigma_optimize_unknown_positions


class Enigma4_optimize_unknown_positions
{
public:
     // Use rotor and ring start to divide up thread jobs.
     constexpr Enigma4_optimize_unknown_positions (
          const EnigmaBase& base,
          const Rotor& rotor4,
          std::string_view plugboard,
          modular_int<26> rotor1_start = modular_int<26>{0},
          modular_int<26> rotor2_start = modular_int<26>{0},
          modular_int<26> rotor3_start = modular_int<26>{0},
          modular_int<26> rotor4_start = modular_int<26>{0},
          modular_int<26> ring1_start  = modular_int<26>{0},
          modular_int<26> ring2_start  = modular_int<26>{0},
          modular_int<26> ring3_start  = modular_int<26>{0},
          modular_int<26> ring4_start  = modular_int<26>{0}
     )
     : Enigma4_optimize_unknown_positions {
          Enigma4Key {base, rotor4, plugboard,
                      rotor1_start, rotor2_start, rotor3_start, rotor4_start,
                      ring1_start, ring2_start, ring3_start, ring4_start}}
     {}


     constexpr Enigma4_optimize_unknown_positions (Enigma4Key key)
     : key {std::move(key)}, offsets {this->key}
     {
          // the plugboard_stator_forward should have the range 0 <= i < 26
          std::transform(
               std::execution::par_unseq,
               this->key.plugboard.begin(), this->key.plugboard.end(),
               plugboard_stator_forward.begin(),
               [this] (char c) { return this->key.stator->forward[c - 'A'] - 26; }
          );

          // rotors keep the range 26 <= i < 52
          std::copy(std::execution::par_unseq,
                    this->key.rotor1->forward.begin(), this->key.rotor1->forward.begin() + rotor1_forward.size(),
                    rotor1_forward.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor2->forward.begin(), this->key.rotor2->forward.end(),
                    rotor2_forward.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor3->forward.begin(), this->key.rotor3->forward.end(),
                    rotor3_forward.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor4->forward.begin(), this->key.rotor4->forward.end(),
                    rotor4_forward.begin());

          // the reflector should have the range 0 <= i < 26
          std::transform(
               std::execution::par_unseq,
               this->key.reflector->forward.begin(), this->key.reflector->forward.begin() + reflector.size(),
               reflector.begin(),
               [] (int o) { return o - 26; }
          );

          // rotors keep the range 26 <= i < 52
          std::copy(std::execution::par_unseq,
                    this->key.rotor4->reverse.begin(), this->key.rotor4->reverse.begin() + rotor4_reverse.size(),
                    rotor4_reverse.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor3->reverse.begin(), this->key.rotor3->reverse.end(),
                    rotor3_reverse.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor2->reverse.begin(), this->key.rotor2->reverse.end(),
                    rotor2_reverse.begin());
          std::copy(std::execution::par_unseq,
                    this->key.rotor1->reverse.begin(), this->key.rotor1->reverse.end(),
                    rotor1_reverse.begin());

          // the plugboard_stator_forward should have the range 0 <= i < 26
          std::transform(
               std::execution::par_unseq,
               this->key.stator->reverse.begin(), this->key.stator->reverse.begin() + 26,
               stator_plugboard_reverse.begin(),
               [this] (int o) { return this->key.plugboard[o - 26] - 'A'; }
          );

          std::copy(std::execution::par_unseq,
                    stator_plugboard_reverse.begin(), stator_plugboard_reverse.begin() + 26,
                    stator_plugboard_reverse.begin() + 26);
     }


     template <std::input_iterator I, std::sentinel_for<I> S, std::weakly_incrementable O>
          requires std::same_as<std::iter_value_t<I>, int> && std::same_as<std::iter_value_t<O>, int>
     void encrypt (I first, S last, O result)
     {
          offsets.reset();

          // std::transform does not ensure in-order traversal; std::for_each does not have an output parameter
          for (; first != last; ++first, (void)++result)
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

}; // class Enigma4_optimize_unknown_positions


// class Enigma_unknown_plugboard
// {
//      // TODO: provide an efficient way to iterate through plugboard permutations
// }; // class Enigma_unknown_plugboard


// ---------------------------------------------------------------------------------------------------------------------
// Simple interface
// ---------------------------------------------------------------------------------------------------------------------
class Enigma
{
public:
     constexpr Enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          std::string_view rotor_positions,
          std::string_view ring_positions
     )
     : Enigma {EnigmaKey {stator, rotor1, rotor2, rotor3, reflector, plugboard, rotor_positions, ring_positions}}
     {}


     constexpr Enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          modular_int<26> rotor1_pos = modular_int<26>{0},
          modular_int<26> rotor2_pos = modular_int<26>{0},
          modular_int<26> rotor3_pos = modular_int<26>{0},
          modular_int<26> ring1_pos  = modular_int<26>{0},
          modular_int<26> ring2_pos  = modular_int<26>{0},
          modular_int<26> ring3_pos  = modular_int<26>{0}
     )
     : Enigma {EnigmaKey {stator, rotor1, rotor2, rotor3, reflector, plugboard,
                          rotor1_pos, rotor2_pos, rotor3_pos, ring1_pos, ring2_pos, ring3_pos}}
     {}

     constexpr Enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0
     )
     : Enigma {EnigmaKey {stator, rotor1, rotor2, rotor3, reflector, plugboard,
                          modular_int<26>{rotor1_pos}, modular_int<26>{rotor2_pos}, modular_int<26>{rotor3_pos},
                          modular_int<26>{ring1_pos}, modular_int<26>{ring2_pos}, modular_int<26>{ring3_pos}}}
     {}


     constexpr Enigma (EnigmaKey key)
     : enigma {std::move(key)}
     {}


     std::string encrypt (std::string_view input);

private:
     Enigma_optimize_unknown_positions enigma;

}; // class Enigma


class Enigma4
{
public:
     constexpr Enigma4 (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& rotor4,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          std::string_view rotor_positions,
          std::string_view ring_positions
     )
     : Enigma4 {Enigma4Key {stator, rotor1, rotor2, rotor3, rotor4, reflector, plugboard,
                            rotor_positions, ring_positions}}
     {}


     constexpr Enigma4 (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& rotor4,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          modular_int<26> rotor1_pos = modular_int<26>{0},
          modular_int<26> rotor2_pos = modular_int<26>{0},
          modular_int<26> rotor3_pos = modular_int<26>{0},
          modular_int<26> rotor4_pos = modular_int<26>{0},
          modular_int<26> ring1_pos  = modular_int<26>{0},
          modular_int<26> ring2_pos  = modular_int<26>{0},
          modular_int<26> ring3_pos  = modular_int<26>{0},
          modular_int<26> ring4_pos  = modular_int<26>{0}
     )
     : Enigma4 {Enigma4Key {stator, rotor1, rotor2, rotor3, rotor4, reflector, plugboard,
                            rotor1_pos, rotor2_pos, rotor3_pos, rotor4_pos, ring1_pos, ring2_pos, ring3_pos, ring4_pos}}
     {}

     constexpr Enigma4 (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& rotor4,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0,
          int rotor4_pos = 0,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0,
          int ring4_pos  = 0
     )
     : Enigma4 {Enigma4Key {stator, rotor1, rotor2, rotor3, rotor4, reflector, plugboard,
                            modular_int<26>{rotor1_pos}, modular_int<26>{rotor2_pos},
                            modular_int<26>{rotor3_pos}, modular_int<26>{rotor4_pos},
                            modular_int<26>{ring1_pos}, modular_int<26>{ring2_pos},
                            modular_int<26>{ring3_pos}, modular_int<26>{ring4_pos}}}
     {}

     constexpr Enigma4 (Enigma4Key key)
     : enigma {std::move(key)}
     {}


     std::string encrypt (std::string_view input);

private:
     Enigma4_optimize_unknown_positions enigma;

}; // class Enigma4