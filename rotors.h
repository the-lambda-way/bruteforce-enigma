#pragma once

#include <string_view>
#include <vector>


struct Rotor
{
     Rotor (std::string pretty_name,
            std::string_view str,
            std::string_view turnovers)
     : pretty_name {pretty_name}
     {
          init_rotor(str, forward);
          init_rotor(inverse_rotor_string(str), reverse);

          turnoverA = turnovers[0] - 'A';
          turnoverB = turnovers.length() == 1 ? turnoverA : turnovers[1] - 'A';
     }

     // In the enigma machine, using ordinals instead of characters reduces the amount of arithmetic involved.
     // Triplicating the rotor reduces costly modular divisions.
     std::string pretty_name;
     int forward[78];
     int reverse[78];
     int turnoverA;
     int turnoverB;


private:
     // str must have 26 characters
     void init_rotor (std::string_view str, int* out)
     {
          std::copy(str.begin(), str.end(), out);
          std::copy(str.begin(), str.end(), out + 26);
          std::copy(str.begin(), str.end(), out + 52);

          chars_to_ordinals(out, 78);
     }

     // Assumes all uppercase and length 26
     std::string inverse_rotor_string (std::string_view forward)
     {
          std::string inverse(26, '-');

          for (int i = 0; i < 26; ++i)
          {
               inverse[forward[i] - 'A'] = 'A' + i;
          }

          return inverse;
     }

     void chars_to_ordinals (int* arr, int length)
     {
          for (int i = 0; i < length; ++i)     *arr++ -= 'A';
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


     EnigmaBase (const Rotor& stator,
                 const Rotor& rotor1, const Rotor& rotor2, const Rotor& rotor3,
                 const Rotor& reflector)
     : stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, reflector {&reflector}
     {}

     EnigmaBase (const EnigmaBase&) = default;
     EnigmaBase (EnigmaBase&&) = default;
     EnigmaBase& operator= (const EnigmaBase&) = default;
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
     int ring1_pos;
     int ring2_pos;
     int ring3_pos;
     int rotor1_pos;
     int rotor2_pos;
     int rotor3_pos;


     EnigmaConfiguration (
          const EnigmaBase& config,
          std::string_view plugboard,
          std::string_view ring_positions,
          std::string_view rotor_positions
     )
     : EnigmaConfiguration(*config.stator, *config.rotor1, *config.rotor2, *config.rotor3, *config.reflector,
                           plugboard,
                           ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A',
                           rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A')
     {}


     EnigmaConfiguration (
          const EnigmaBase& base,
          std::string_view plugboard,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0
     )
     : EnigmaConfiguration(*base.stator, *base.rotor1, *base.rotor2, *base.rotor3, *base.reflector,
                           plugboard,
                           ring1_pos, ring2_pos, ring3_pos,
                           rotor1_pos, rotor2_pos, rotor3_pos)
     {}


     EnigmaConfiguration (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          std::string_view ring_positions,
          std::string_view rotor_positions
     )
     : EnigmaConfiguration(stator, rotor1, rotor2, rotor3, reflector,
                           plugboard,
                           ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A',
                           rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A')
     {}


     EnigmaConfiguration (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0
     )
     : stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, reflector {&reflector},
       plugboard {plugboard},
       ring1_pos {ring1_pos}, ring2_pos {ring2_pos}, ring3_pos {ring3_pos},
       rotor1_pos {rotor1_pos}, rotor2_pos {rotor2_pos}, rotor3_pos {rotor3_pos}
     {}


     EnigmaConfiguration (const EnigmaConfiguration& c) = default;
     EnigmaConfiguration (EnigmaConfiguration&& c) = default;
     EnigmaConfiguration& operator= (const EnigmaConfiguration&) = default;
};
