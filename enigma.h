#pragma once

#include <algorithm>     // std::copy
#include <cassert>       // increment_ring
#include <cstring>       // std::strncpy
#include <string>
#include <string_view>
#include "enigma_models.h"


class Enigma
{
public:
     Enigma (const EnigmaConfiguration& config,
             std::string_view plugboard,
             int ring1_pos  = 0,
             int ring2_pos  = 0,
             int ring3_pos  = 0,
             int rotor1_pos = 0,
             int rotor2_pos = 0,
             int rotor3_pos = 0
     )
     : Enigma(config.stator, config.rotor1, config.rotor2, config.rotor3, config.reflector,
              plugboard, ring1_pos, ring2_pos, ring3_pos, rotor1_pos, rotor2_pos, rotor3_pos)
     {}

     Enigma (const EnigmaConfiguration& config,
             std::string_view plugboard,
             std::string_view ring_positions,
             std::string_view rotor_positions
     )
     : Enigma(config.stator, config.rotor1, config.rotor2, config.rotor3, config.reflector,
              plugboard,
              ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A',
              rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A')
     {}


     Enigma (
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
     {
          init_plug(plugboard,          plugboardA);
          init_wheel(stator.forward,    stator_forward);
          init_wheel(rotor1.forward,    rotor1_forward);
          init_wheel(rotor2.forward,    rotor2_forward);
          init_wheel(rotor3.forward,    rotor3_forward);
          init_wheel(reflector.forward, this->reflector);
          init_wheel(rotor3.reverse,    rotor3_reverse);
          init_wheel(rotor2.reverse,    rotor2_reverse);
          init_wheel(rotor1.reverse,    rotor1_reverse);
          init_wheel(stator.reverse,    stator_reverse);
          init_plug(plugboard,          plugboardB);

          // Note: alpha did not initialize correctly with default member initialization (compiler bug?)
          const char* str = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";
          std::strncpy(alpha, str, 78);

          rotor1_turnoverA = rotor1.turnovers[0] - 'A';
          rotor1_turnoverB = rotor1.turnovers[1] == '\0' ? rotor1.turnovers[0] - 'A' : rotor1.turnovers[1] - 'A';
          rotor2_turnoverA = rotor2.turnovers[0] - 'A';
          rotor2_turnoverB = rotor2.turnovers[1] == '\0' ? rotor2.turnovers[0] - 'A' : rotor2.turnovers[1] - 'A';

          rotor1_turnoverA = calculate_offset(rotor1_turnoverA, ring1_pos);
          rotor1_turnoverB = calculate_offset(rotor1_turnoverB, ring1_pos);
          rotor2_turnoverA = calculate_offset(rotor2_turnoverA, ring2_pos);
          rotor2_turnoverB = calculate_offset(rotor2_turnoverB, ring2_pos);

          this->ring1_pos = ring1_pos;
          this->ring2_pos = ring2_pos;
          this->ring3_pos = ring3_pos;

          this->rotor1_pos = calculate_offset(rotor1_pos, ring1_pos);
          this->rotor2_pos = calculate_offset(rotor2_pos, ring2_pos);
          this->rotor3_pos = calculate_offset(rotor3_pos, ring3_pos);
     }

     Enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          std::string_view plugboard,
          std::string_view ring_positions,
          std::string_view rotor_positions
     )
     : Enigma(stator, rotor1, rotor2, rotor3, reflector,
              plugboard,
              ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A',
              rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A')
     {}


     std::string encrypt (std::string_view input)
     {
          rotor1_i = rotor1_pos;
          rotor2_i = rotor2_pos;
          rotor3_i = rotor3_pos;

          std::string s = "";
          s.reserve(input.length());

          for (auto c : input)     s += encrypt_letter(c);

          return s;
     }


     void encrypt (char* out, std::string_view input)
     {
          rotor1_i = rotor1_pos;
          rotor2_i = rotor2_pos;
          rotor3_i = rotor3_pos;

          for (int i = 0; i < input.length(); ++i)
               out[i] = encrypt_letter(input[i]);

     }


     // While bruteforcing different rings, share memory efficiently by adjusting the ring in place
     void increment_ring (int ring)
     {
          // assert(1 <= ring && ring <= 3);

          // incrementing the ring decrements the rotor
          switch (ring)
          {
               case 1 :
                    increment_index(ring1_pos);
                    for (int& i : rotor1_forward)     decrement_index(i);
                    for (int& i : rotor1_reverse)     decrement_index(i);
                    decrement_index(rotor1_turnoverA);
                    decrement_index(rotor1_turnoverB);
                    break;
               case 2 :
                    increment_index(ring2_pos);
                    for (int& i : rotor2_forward)     decrement_index(i);
                    for (int& i : rotor2_reverse)     decrement_index(i);
                    decrement_index(rotor2_turnoverA);
                    decrement_index(rotor2_turnoverB);
                    break;
               case 3 :
                    increment_index(ring3_pos);
                    for (int& i : rotor3_forward)     decrement_index(i);
                    for (int& i : rotor3_reverse)     decrement_index(i);
                    break;
          }
     }


     // While bruteforcing different rotor positions, share memory efficiently by adjusting the rotor in place
     void increment_rotor (int rotor)
     {
          // assert(1 <= rotor && rotor <= 3);

          switch (rotor)
          {
               case 1 :
                    increment_index(rotor1_pos);
                    for (int& i : rotor1_forward)     increment_index(i);
                    for (int& i : rotor1_reverse)     increment_index(i);
                    increment_index(rotor1_turnoverA);
                    increment_index(rotor1_turnoverB);
                    break;
               case 2 :
                    increment_index(rotor2_pos);
                    for (int& i : rotor2_forward)     increment_index(i);
                    for (int& i : rotor2_reverse)     increment_index(i);
                    increment_index(rotor2_turnoverA);
                    increment_index(rotor2_turnoverB);
                    break;
               case 3 :
                    increment_index(rotor3_pos);
                    for (int& i : rotor3_forward)     increment_index(i);
                    for (int& i : rotor3_reverse)     increment_index(i);
                    break;
          }
     }


private:
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
     char alpha[79];

     int rotor1_turnoverA;
     int rotor1_turnoverB;
     int rotor2_turnoverA;
     int rotor2_turnoverB;
     int ring1_pos;
     int ring2_pos;
     int ring3_pos;
     int rotor1_pos;
     int rotor2_pos;
     int rotor3_pos;

     // Positions during encryption
     int rotor1_i;
     int rotor2_i;
     int rotor3_i;


     // str must have 26 characters, out must have 78
     void init_wheel (std::string_view str, int* out)
     {
          std::copy(str.begin(), str.end(), out);
          std::copy(str.begin(), str.end(), out + 26);
          std::copy(str.begin(), str.end(), out + 52);

          chars_to_ordinals(out, 78);
     }


     void init_plug (std::string_view str, int* out)
     {
          std::copy(str.begin(), str.end(), out);
          chars_to_ordinals(out, 26);
     }


     void chars_to_ordinals (int* arr, int length)
     {
          for (int i = 0; i < length; ++i)     *arr++ -= 'A';
     }


     int calculate_offset (int rotor_pos, int ring_pos) const
     {
          return rotor_pos - ring_pos + (rotor_pos > ring_pos ? 0 : 26);
     }


     void increment_index (int& pos)
     {
          pos += pos < 25 ? 1 : -25;
     }


     void decrement_index (int& pos)
     {
          pos -= pos > 0 ? 1 : -25;
     }


     void step_positions ()
     {
          if (rotor2_i == rotor2_turnoverA || rotor2_i == rotor2_turnoverB)
          {
               increment_index(rotor3_i);
               increment_index(rotor2_i);
          }

          if (rotor1_i == rotor1_turnoverA || rotor1_i == rotor1_turnoverB)
               increment_index(rotor2_i);

          increment_index(rotor1_i);
     }


     int run_rotor (const int* rotor, int offset, int pos) const
     {
          return rotor[pos + offset] - offset + 26;
     }


     char encrypt_letter (char keypressed)
     {
          return alpha[encrypt_ordinal(keypressed - 'A')];
     }


     int encrypt_ordinal (int i)
     {
          step_positions();

          i = plugboardA[i];

          i = stator_forward[i];
          i = run_rotor(rotor1_forward, rotor1_i, i);
          i = run_rotor(rotor2_forward, rotor2_i, i);
          i = run_rotor(rotor3_forward, rotor3_i, i);
          i = reflector[i];
          i = run_rotor(rotor3_reverse, rotor3_i, i);
          i = run_rotor(rotor2_reverse, rotor2_i, i);
          i = run_rotor(rotor1_reverse, rotor1_i, i);
          i = stator_reverse[i];

          return plugboardB[i];
     }

}; // class Enigma



