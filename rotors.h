#pragma once

#include <algorithm>
#include <string_view>
#include <vector>


struct Rotor
{
     // In the enigma machine, using ordinals instead of characters reduces the amount of arithmetic involved.
     // Triplicating the rotor reduces costly modular divisions.
     std::string pretty_name;
     int forward[78];
     int reverse[78];
     int turnoverA;
     int turnoverB;


     Rotor (std::string pretty_name,
            std::string_view str,
            std::string_view turnovers)
     : pretty_name {pretty_name}
     {
          for (int i = 0; i < 26; ++i)     forward[i] = str[i] - 'A';
          std::copy(forward, forward + 26, forward + 26);
          std::copy(forward, forward + 26, forward + 52);

          for (int i = 0; i < 26; ++i)     reverse[forward[i]] = i;
          std::copy(reverse, reverse + 26, reverse + 26);
          std::copy(reverse, reverse + 26, reverse + 52);

          turnoverA = turnovers.length() > 0 ? turnovers[0] - 'A' : -1;
          turnoverB = turnovers.length() > 1 ? turnovers[1] - 'A' : turnoverA;
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
struct EnigmaConfiguration
{
     const Rotor* stator;        // ETW
     const Rotor* rotor1;
     const Rotor* rotor2;
     const Rotor* rotor3;
     const Rotor* reflector;     // UKW
     std::string_view plugboard;
     int rotor1_pos;
     int rotor2_pos;
     int rotor3_pos;
     int ring1_pos;
     int ring2_pos;
     int ring3_pos;


     constexpr EnigmaConfiguration (
          const EnigmaBase& config,
          std::string_view plugboard,
          std::string_view ring_positions,
          std::string_view rotor_positions
     )
     : EnigmaConfiguration(*config.stator, *config.rotor1, *config.rotor2, *config.rotor3, *config.reflector,
                           plugboard,
                           rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A',
                           ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A')
     {}


     constexpr EnigmaConfiguration (
          const EnigmaBase& base,
          std::string_view plugboard,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0
     )
     : EnigmaConfiguration(*base.stator, *base.rotor1, *base.rotor2, *base.rotor3, *base.reflector,
                           plugboard,
                           rotor1_pos, rotor2_pos, rotor3_pos,
                           ring1_pos, ring2_pos, ring3_pos)
     {}


     constexpr EnigmaConfiguration (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          std::string_view rotor_positions,
          std::string_view ring_positions
     )
     : EnigmaConfiguration(stator, rotor1, rotor2, rotor3, reflector,
                           plugboard,
                           rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A',
                           ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A')
     {}


     constexpr EnigmaConfiguration (
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
     : stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, reflector {&reflector},
       plugboard {plugboard},
       rotor1_pos {rotor1_pos}, rotor2_pos {rotor2_pos}, rotor3_pos {rotor3_pos},
       ring1_pos {ring1_pos}, ring2_pos {ring2_pos}, ring3_pos {ring3_pos}
     {}


     constexpr EnigmaConfiguration (const EnigmaConfiguration& c) = default;
     constexpr EnigmaConfiguration (EnigmaConfiguration&& c) = default;
     constexpr EnigmaConfiguration& operator= (const EnigmaConfiguration&) = default;
     constexpr bool operator== (const EnigmaConfiguration&) const = default;
};
