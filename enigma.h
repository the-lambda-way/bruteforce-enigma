#pragma once

#include <algorithm>     // std::copy
#include <cstring>       // std::strncpy
#include <string>
#include <string_view>
#include "rotors.h"


void str_to_ordinals (std::string_view str, int* out);
void str_from_ordinals (int* ordinals, int length, std::string& out);


class Enigma
{
public:
     constexpr Enigma (
          const EnigmaBase& base,
          std::string_view plugboard,
          std::string_view ring_positions,
          std::string_view rotor_positions
     )
     : Enigma(EnigmaConfiguration {base, plugboard, ring_positions, rotor_positions})
     {}


     constexpr Enigma (
          const EnigmaBase& base,
          std::string_view plugboard,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0
     )
     : Enigma(EnigmaConfiguration {base, plugboard,
                                   ring1_pos, ring2_pos, ring3_pos, rotor1_pos, rotor2_pos, rotor3_pos})
     {}


     constexpr Enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          std::string_view ring_positions,
          std::string_view rotor_positions
     )
     : Enigma(EnigmaConfiguration {stator, rotor1, rotor2, rotor3, reflector,
                                   plugboard,
                                   ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A',
                                   rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A'})
     {}


     constexpr Enigma (
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
     : Enigma(EnigmaConfiguration {stator, rotor1, rotor2, rotor3, reflector, plugboard,
                                   ring1_pos, ring2_pos, ring3_pos, rotor1_pos, rotor2_pos, rotor3_pos})
     {}


     constexpr Enigma (EnigmaConfiguration config)
     : config {std::move(config)}
     {
          str_to_ordinals(this->config.plugboard, plugboardA);

          init_rotor(this->config.stator->forward,    stator_forward);
          init_rotor(this->config.rotor1->forward,    rotor1_forward);
          init_rotor(this->config.rotor2->forward,    rotor2_forward);
          init_rotor(this->config.rotor3->forward,    rotor3_forward);
          init_rotor(this->config.reflector->forward, reflector);
          init_rotor(this->config.rotor3->reverse,    rotor3_reverse);
          init_rotor(this->config.rotor2->reverse,    rotor2_reverse);
          init_rotor(this->config.rotor1->reverse,    rotor1_reverse);
          init_rotor(this->config.stator->reverse,    stator_reverse);

          std::copy(plugboardA, plugboardA + 26, plugboardB);

          // Note: alpha did not initialize correctly with default member initialization (compiler bug?)
          constexpr const char* str = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
          std::strncpy(alpha, str, 26);

          // Rings don't move during encryption, so we can precompute their effects
          reset_ring_pos(1, config.ring1_pos);
          reset_ring_pos(2, config.ring2_pos);
          reset_ring_pos(3, config.ring3_pos);
     }


     // Operation ------------------------------------------------------------------------------------------------------
     std::string encrypt (std::string_view input);

     // Out must have one extra element than input, for '\0'
     void encrypt (std::string_view input, char* out);

     // Out must have one extra element than length, for '\0'
     void encrypt (const int* ordinals, int length, char* out);
     void encrypt (const int* ordinals, int length, int* out);

     void reset_ring_pos (int ring, int pos);
     void reset_rotor_pos (int rotor, int pos);

     // While bruteforcing ring positions, save calculations by rotating the rotor instead of recalculating it
     void increment_ring (int ring);

     // While bruteforcing rotor positions, save calculations by rotating the rotor instead of recalculating it
     void increment_rotor (int rotor);

     EnigmaConfiguration get_config ();


private:
     EnigmaConfiguration config;

     // Optimization ---------------------------------------------------------------------------------------------------
     int plugboardA[26];
     int stator_forward[78];
     int rotor1_forward[78];
     int rotor2_forward[78];
     int rotor3_forward[78];
     int reflector[78];
     int rotor3_reverse[78];
     int rotor2_reverse[78];
     int rotor1_reverse[78];
     int stator_reverse[78];
     int plugboardB[26];
     char alpha[26];

     // Rotor offsets combine rotor position and ring position
     int rotor1_offset;
     int rotor2_offset;
     int rotor3_offset;

     // Turnover offsets tracks the turnover position relative to the current offset
     int turnover1A_offset;
     int turnover1B_offset;
     int turnover2A_offset;
     int turnover2B_offset;

     // Relative positions during encryption
     int offset1;
     int offset2;
     int offset3;


     void init_rotor (const int* in, int* out);
     constexpr int calculate_offset (int rotor_pos, int ring_pos) const;


     // Operation ------------------------------------------------------------------------------------------------------
     constexpr int increment_of (int pos) const;
     constexpr int decrement_of (int pos) const;
     void step_positions ();
     int run_rotor (const int* rotor, int offset, int pos) const;
     char encrypt_letter (char keypressed);
     int encrypt_ordinal (int j);

}; // class Enigma



