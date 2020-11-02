#pragma once

#include <algorithm>
#include <bitset>
#include <exception>
#include <execution>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "leaderboard.h"
#include "modular_int.h"
#include "util.h"


// ---------------------------------------------------------------------------------------------------------------------
// Text validation
// ---------------------------------------------------------------------------------------------------------------------
struct Plugboard
{
     std::vector<int> ordinals;

     Plugboard (const char* pairs);
     Plugboard (std::string_view pairs);

     std::string to_string () const;

}; // struct Plugboard

const Plugboard default_plugboard {""};


std::ostream& operator<< (std::ostream& os, const Plugboard& plug);


struct Ciphertext
{
     std::string      text;
     std::vector<int> ordinals;

     Ciphertext (std::string text, std::vector<int> ordinals);
     Ciphertext (const char* text);
     Ciphertext (std::string_view text);

     bool operator== (const Ciphertext& rhs) const;
     bool operator== (std::string_view rhs) const;
};


std::ostream& operator<< (std::ostream& os, const Ciphertext& ct);


template <int N = 3>
struct RotorPositions
{
     std::array<modular_int<26>, N> positions;

     constexpr RotorPositions (std::string_view positions)
     {
          int count = 0;

          for (char c : positions)
          {
               if (is_space(c))      continue;
               if (!is_alpha(c))     throw std::domain_error("Rotor positions can only have alphabetical characters");
               if (count == N)       throw std::invalid_argument("Too many rotor positions, expected " + std::to_string(N));

               this->positions[count++] = modular_int<26>{to_ordinal(c)};
          }

          if (count < N)     throw std::invalid_argument("Not enough rotor positions, expected " + std::to_string(N));
     }
};


template <int N = 3>
struct RingPositions
{
     std::array<modular_int<26>, N> positions;

     constexpr RingPositions (std::string_view positions)
     {
          int count = 0;

          for (char c : positions)
          {
               if (is_space(c))      continue;
               if (!is_alpha(c))     throw std::domain_error("Ring positions can only have alphabetical characters");
               if (count == N)       throw std::invalid_argument("Too many ring positions, expected " + std::to_string(N));

               this->positions[count++] = modular_int<26>{to_ordinal(c)};
          }

          if (count < N)     throw std::invalid_argument("Not enough ring positions, expected " + std::to_string(N));
     }
};


// ---------------------------------------------------------------------------------------------------------------------
// Rotors
// ---------------------------------------------------------------------------------------------------------------------
struct Rotor
{
     // In the enigma machine, using ordinals instead of characters reduces the amount of arithmetic involved.
     // Triplicating the rotor reduces costly modular divisions.
     std::string_view    pretty_name;
     std::array<int, 78> forward;
     std::array<int, 78> reverse;
     std::bitset<26>     turnovers;


     constexpr Rotor (std::string_view pretty_name,
                      std::string_view permutation,
                      std::string_view turnovers)
     : pretty_name {pretty_name}, turnovers {convert_to_bits(turnovers)}
     {
          if (permutation.length() != 26)     throw std::length_error("Rotor permutation must have length 26");

          int status = validate_unique_letters(permutation);
          if (status == 1)     throw std::domain_error("Rotor permutation can only have alphabetical characters.");
          if (status == 2)     throw std::invalid_argument("Rotor permutation characters must all be unique.");

          // ranges should be 26 <= i < 52 for optimization purposes

          // NOTE: C++ parallel policy does not support constexpr, so can't be used here.
          std::ranges::transform(permutation, forward.begin(), [] (char c) { return c - 'A' + 26; } );

          std::copy(&forward[0], &forward[26], &forward[26]);
          std::copy(&forward[0], &forward[26], &forward[52]);

          for (int i = 0; i < 26; ++i)     reverse[forward[i] - 26] = i + 26;
          std::copy(&reverse[0], &reverse[26], &reverse[26]);
          std::copy(&reverse[0], &reverse[26], &reverse[52]);
     }


private:
     // Note: std::bitset is mostly non-constexpr. Thus, constexpr initialization must take place via the constructor.
     constexpr unsigned long long convert_to_bits (std::string_view turnovers)
     {
          if (turnovers.length() > 26)     throw std::length_error("Turnovers cannot contain more than 26 characters.");

          int status = validate_unique_letters(turnovers);
          if (status == 1)      throw std::domain_error("Turnovers can only be alphabetical characters.");
          if (status == 2)      throw std::invalid_argument("Turnover characters must all be unique.");

          unsigned long long turnover_value = 0;

          for (char c : turnovers | std::views::transform(to_upper))     turnover_value |= (1 << (c - 'A'));

          return turnover_value;
     }
};


const Rotor default_rotor {"(default)", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", ""};


// ---------------------------------------------------------------------------------------------------------------------
// Enigma Keys
// ---------------------------------------------------------------------------------------------------------------------
// Useful for passing in settings, or retaining notable results.
struct EnigmaKey
{
     const Rotor* stator;        // ETW
     const Rotor* rotor1;
     const Rotor* rotor2;
     const Rotor* rotor3;
     const Rotor* reflector;     // UKW
     const std::vector<int>* plug_ordinals;
     modular_int<26> rotor1_pos;
     modular_int<26> rotor2_pos;
     modular_int<26> rotor3_pos;
     modular_int<26> ring1_pos;
     modular_int<26> ring2_pos;
     modular_int<26> ring3_pos;


     EnigmaKey ();

     EnigmaKey (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          const Plugboard& plugboard,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0
     );

     EnigmaKey (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          const Plugboard& plugboard,
          const char* rotor_positions,
          const char* ring_positions
     );


     constexpr EnigmaKey (const EnigmaKey&) = default;
     constexpr EnigmaKey (EnigmaKey&&) = default;
     constexpr EnigmaKey& operator= (const EnigmaKey&) = default;
     constexpr bool operator== (const EnigmaKey&) const = default;

}; // class EnigmaKey


struct Enigma4Key
{
     const Rotor* stator;        // ETW
     const Rotor* rotor1;
     const Rotor* rotor2;
     const Rotor* rotor3;
     const Rotor* rotor4;
     const Rotor* reflector;     // UKW
     const std::vector<int>* plug_ordinals;
     modular_int<26> rotor1_pos;
     modular_int<26> rotor2_pos;
     modular_int<26> rotor3_pos;
     modular_int<26> rotor4_pos;
     modular_int<26> ring1_pos;
     modular_int<26> ring2_pos;
     modular_int<26> ring3_pos;
     modular_int<26> ring4_pos;


     Enigma4Key ();

     Enigma4Key (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& rotor4,
          const Rotor& reflector,     // UKW
          const Plugboard& plugboard,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0,
          int rotor4_pos = 0,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0,
          int ring4_pos  = 0
     );


     Enigma4Key (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& rotor4,
          const Rotor& reflector,     // UKW
          const Plugboard& plugboard,
          const char* rotor_positions,
          const char* ring_positions
     );


     constexpr Enigma4Key (const Enigma4Key&) = default;
     constexpr Enigma4Key (Enigma4Key&&) = default;
     constexpr Enigma4Key& operator= (const Enigma4Key&) = default;
     constexpr bool operator== (const Enigma4Key&) const = default;

}; // class Enigma4Key


// ---------------------------------------------------------------------------------------------------------------------
// Models
// ---------------------------------------------------------------------------------------------------------------------
template <class KeyType> class basic_enigma_model {};


template <>
struct basic_enigma_model<EnigmaKey>
{
     const Rotor*              stator;         // ETW
     std::vector<const Rotor*> rotors;
     std::vector<const Rotor*> reflectors;     // UKW
};


template <>
struct basic_enigma_model<Enigma4Key>
{
     const Rotor*              stator;         // ETW
     std::vector<const Rotor*> rotors;
     std::vector<const Rotor*> rotors4;
     std::vector<const Rotor*> reflectors;     // UKW
};

using EnigmaModel  = basic_enigma_model<EnigmaKey>;
using Enigma4Model = basic_enigma_model<Enigma4Key>;


template <class EngimaKeyType> class basic_enigma_base {};


template<>
struct basic_enigma_base<EnigmaKey>
{
     const Rotor* stator;        // ETW
     const Rotor* rotor1;
     const Rotor* rotor2;
     const Rotor* rotor3;
     const Rotor* reflector;     // UKW


     constexpr basic_enigma_base (
          const Rotor& stator,
          const Rotor& rotor1, const Rotor& rotor2, const Rotor& rotor3,
          const Rotor& reflector)
     : stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, reflector {&reflector}
     {}

     constexpr basic_enigma_base (const basic_enigma_base&) = default;
     constexpr basic_enigma_base (basic_enigma_base&&) = default;
     constexpr basic_enigma_base& operator= (const basic_enigma_base&) = default;
     constexpr bool operator== (const basic_enigma_base&) const = default;
};


template <>
struct basic_enigma_base<Enigma4Key>
{
     const Rotor* stator;        // ETW
     const Rotor* rotor1;
     const Rotor* rotor2;
     const Rotor* rotor3;
     const Rotor* rotor4;
     const Rotor* reflector;     // UKW


     constexpr basic_enigma_base (const Rotor& stator,
                                  const Rotor& rotor1, const Rotor& rotor2, const Rotor& rotor3, const Rotor& rotor4,
                                  const Rotor& reflector)
     : stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, rotor4 {&rotor4}, reflector {&reflector}
     {}

     constexpr basic_enigma_base (const basic_enigma_base&) = default;
     constexpr basic_enigma_base (basic_enigma_base&&) = default;
     constexpr basic_enigma_base& operator= (const basic_enigma_base&) = default;
     constexpr bool operator== (const basic_enigma_base&) const = default;
};

using EnigmaBase  = basic_enigma_base<EnigmaKey>;
using Enigma4Base = basic_enigma_base<Enigma4Key>;
