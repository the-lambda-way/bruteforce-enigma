#pragma once

#include <algorithm>
#include <bitset>
#include <execution>
#include <string_view>
#include <vector>
#include "modular_int.h"


struct Rotor
{
     // In the enigma machine, using ordinals instead of characters reduces the amount of arithmetic involved.
     // Triplicating the rotor reduces costly modular divisions.
     std::string_view    pretty_name;
     std::array<int, 78> forward;
     std::array<int, 78> reverse;
     std::bitset<26>     turnovers;


     constexpr Rotor (std::string_view pretty_name,
                      std::string_view str,
                      std::string_view turnovers)
     : pretty_name {pretty_name}
     {
          // ranges should be 26 <= i < 52 for optimization purposes

          std::transform(
               std::execution::par_unseq,
               str.begin(), str.end(),
               forward.begin(),
               [] (char c) { return c - 'A' + 26; }
          );

          std::copy(std::execution::par_unseq, &forward[0], &forward[26], &forward[26]);
          std::copy(std::execution::par_unseq, &forward[0], &forward[26], &forward[52]);

          for (int i = 0; i < 26; ++i)     reverse[forward[i] - 26] = i + 26;
          std::copy(std::execution::par_unseq, &reverse[0], &reverse[26], &reverse[26]);
          std::copy(std::execution::par_unseq, &reverse[0], &reverse[26], &reverse[52]);

          for (char c : turnovers)     this->turnovers.set(c - 'A');
     }
};


// Useful for creating permutations of different settings.
struct EnigmaModel
{
     const Rotor*              stator;         // ETW
     std::vector<const Rotor*> rotors;
     std::vector<const Rotor*> reflectors;     // UKW
};


// Useful when cracking rotor and ring positions or plugboards.
struct EnigmaBase
{
     const Rotor* stator;        // ETW
     const Rotor* rotor1;
     const Rotor* rotor2;
     const Rotor* rotor3;
     const Rotor* reflector;     // UKW


     constexpr EnigmaBase (const Rotor& stator,
                           const Rotor& rotor1, const Rotor& rotor2, const Rotor& rotor3,
                           const Rotor& reflector)
     : stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, reflector {&reflector}
     {}

     constexpr EnigmaBase (const EnigmaBase&) = default;
     constexpr EnigmaBase (EnigmaBase&&) = default;
     constexpr EnigmaBase& operator= (const EnigmaBase&) = default;
     constexpr bool operator== (const EnigmaBase&) const = default;
};


// Useful for passing in settings, or retaining notable results.
struct EnigmaKey
{
     const Rotor* stator;        // ETW
     const Rotor* rotor1;
     const Rotor* rotor2;
     const Rotor* rotor3;
     const Rotor* reflector;     // UKW
     std::string_view plugboard;
     modular_int<26> rotor1_pos;
     modular_int<26> rotor2_pos;
     modular_int<26> rotor3_pos;
     modular_int<26> ring1_pos;
     modular_int<26> ring2_pos;
     modular_int<26> ring3_pos;


     constexpr EnigmaKey (
          const EnigmaBase& base,
          std::string_view plugboard,
          std::string_view ring_positions,
          std::string_view rotor_positions
     )
     : EnigmaKey {*base.stator, *base.rotor1, *base.rotor2, *base.rotor3, *base.reflector,
                  plugboard,
                  rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A',
                  ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A'}
     {}


     constexpr EnigmaKey (
          const EnigmaBase& base,
          std::string_view plugboard,
          modular_int<26> rotor1_pos = 0,
          modular_int<26> rotor2_pos = 0,
          modular_int<26> rotor3_pos = 0,
          modular_int<26> ring1_pos  = 0,
          modular_int<26> ring2_pos  = 0,
          modular_int<26> ring3_pos  = 0
     )
     : EnigmaKey {*base.stator, *base.rotor1, *base.rotor2, *base.rotor3, *base.reflector,
                  plugboard,
                  rotor1_pos, rotor2_pos, rotor3_pos,
                  ring1_pos, ring2_pos, ring3_pos}
     {}


     constexpr EnigmaKey (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          std::string_view rotor_positions,
          std::string_view ring_positions
     )
     : EnigmaKey {stator, rotor1, rotor2, rotor3, reflector,
                  plugboard,
                  rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A',
                  ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A'}
     {}


     constexpr EnigmaKey (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          modular_int<26> rotor1_pos = 0,
          modular_int<26> rotor2_pos = 0,
          modular_int<26> rotor3_pos = 0,
          modular_int<26> ring1_pos  = 0,
          modular_int<26> ring2_pos  = 0,
          modular_int<26> ring3_pos  = 0
     )
     : stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, reflector {&reflector},
       plugboard {plugboard},
       rotor1_pos {rotor1_pos}, rotor2_pos {rotor2_pos}, rotor3_pos {rotor3_pos},
       ring1_pos {ring1_pos}, ring2_pos {ring2_pos}, ring3_pos {ring3_pos}
     {}


     constexpr EnigmaKey (const EnigmaKey&) = default;
     constexpr EnigmaKey (EnigmaKey&&) = default;
     constexpr EnigmaKey& operator= (const EnigmaKey&) = default;
     constexpr bool operator== (const EnigmaKey&) const = default;
};
