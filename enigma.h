#pragma once

#include <algorithm>     // std::copy
#include <cassert>       // set_ring, set_rotor
#include <cstring>       // std::strncpy
#include <string>
#include <string_view>
#include "enigma_models.h"


class Enigma
{
public:
     Enigma (const EnigmaBase& base,
             std::string_view plugboard,
             std::string_view ring_positions,
             std::string_view rotor_positions
     )
     : Enigma(EnigmaConfiguration {base.stator, base.rotor1, base.rotor2, base.rotor3, base.reflector,
                                   plugboard,
                                   ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A',
                                   rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A'})
     {}


     Enigma (const EnigmaBase& base,
             std::string_view plugboard,
             int ring1_pos  = 0,
             int ring2_pos  = 0,
             int ring3_pos  = 0,
             int rotor1_pos = 0,
             int rotor2_pos = 0,
             int rotor3_pos = 0
     )
     : Enigma(EnigmaConfiguration {base.stator, base.rotor1, base.rotor2, base.rotor3, base.reflector,
                                   plugboard, ring1_pos, ring2_pos, ring3_pos, rotor1_pos, rotor2_pos, rotor3_pos})
     {}


     Enigma (const Rotor& stator,        // ETW
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


     Enigma (const Rotor& stator,        // ETW
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


     Enigma (EnigmaConfiguration config)
     : config {std::move(config)}
     {
          init_plug(this->config.plugboard,          plugboardA);
          init_wheel(this->config.stator.forward,    stator_forward);
          init_wheel(this->config.rotor1.forward,    rotor1_forward);
          init_wheel(this->config.rotor2.forward,    rotor2_forward);
          init_wheel(this->config.rotor3.forward,    rotor3_forward);
          init_wheel(this->config.reflector.forward, reflector);
          init_wheel(this->config.rotor3.reverse,    rotor3_reverse);
          init_wheel(this->config.rotor2.reverse,    rotor2_reverse);
          init_wheel(this->config.rotor1.reverse,    rotor1_reverse);
          init_wheel(this->config.stator.reverse,    stator_reverse);
          init_plug(this->config.plugboard,          plugboardB);

          // Note: alpha did not initialize correctly with default member initialization (compiler bug?)
          const char* str = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";
          std::strncpy(alpha, str, 78);

          // Rings don't move during encryption, so we can precompute their effects
          reset_ring_pos(1, config.ring1_pos);
          reset_ring_pos(2, config.ring2_pos);
          reset_ring_pos(3, config.ring3_pos);
     }


     // Operation ------------------------------------------------------------------------------------------------------
     std::string encrypt (std::string_view input)
     {
          rotor1_offset = rotor1_pos;
          rotor2_offset = rotor2_pos;
          rotor3_offset = rotor3_pos;

          std::string s = "";
          s.reserve(input.length());

          for (auto c : input)     s += encrypt_letter(c);

          return s;
     }


     void encrypt (char* out, std::string_view input)
     {
          rotor1_offset = rotor1_pos;
          rotor2_offset = rotor2_pos;
          rotor3_offset = rotor3_pos;

          for (int i = 0; i < input.length(); ++i)
               out[i] = encrypt_letter(input[i]);
     }


     void reset_ring_pos (int ring, int pos)
     {
          assert(1 <= ring && ring <= 3);
          assert(0 <= pos && pos <= 25);

          switch (ring)
          {
               case 1 :
                    config.ring1_pos = pos;

                    rotor1_turnoverA = config.rotor1.turnovers[0] - 'A';
                    rotor1_turnoverB = config.rotor1.turnovers[1] == '\0'
                                     ? config.rotor1.turnovers[0] - 'A'
                                     : config.rotor1.turnovers[1] - 'A';

                    rotor1_turnoverA = calculate_offset(rotor1_turnoverA, pos);
                    rotor1_turnoverB = calculate_offset(rotor1_turnoverB, pos);

                    rotor1_pos = calculate_offset(config.rotor1_pos, pos);
                    break;
               case 2 :
                    config.ring2_pos = pos;

                    rotor2_turnoverA = config.rotor2.turnovers[0] - 'A';
                    rotor2_turnoverB = config.rotor2.turnovers[1] == '\0'
                                     ? config.rotor2.turnovers[0] - 'A'
                                     : config.rotor2.turnovers[1] - 'A';

                    rotor2_turnoverA = calculate_offset(rotor2_turnoverA, pos);
                    rotor2_turnoverB = calculate_offset(rotor2_turnoverB, pos);

                    rotor2_pos = calculate_offset(config.rotor2_pos, pos);
                    break;
               case 3 :
                    config.ring3_pos = pos;
                    rotor3_pos = calculate_offset(config.rotor3_pos, pos);
          }
     }


     void reset_rotor_pos (int rotor, int pos)
     {
          assert(1 <= rotor && rotor <= 3);
          assert(0 <= pos && pos <= 25);

          switch (rotor)
          {
               case 1 :
                    config.rotor1_pos = pos;
                    rotor1_pos = calculate_offset(pos, config.ring1_pos);
                    break;
               case 2 :
                    config.rotor2_pos = pos;
                    rotor2_pos = calculate_offset(pos, config.ring2_pos);
                    break;
               case 3 :
                    config.rotor3_pos = pos;
                    rotor3_pos = calculate_offset(pos, config.ring3_pos);
          }
     }



     // While bruteforcing ring positions, save calculations by rotating the rotor instead of recalculating it
     void increment_ring (int ring)
     {
          assert(1 <= ring && ring <= 3);

          // incrementing the ring decrements the rotor position
          switch (ring)
          {
               case 1 :
                    decrement(rotor1_pos);
                    decrement(rotor1_turnoverA);
                    decrement(rotor1_turnoverB);
                    break;
               case 2 :
                    decrement(rotor2_pos);
                    decrement(rotor2_turnoverA);
                    decrement(rotor2_turnoverB);
                    break;
               case 3 :
                    decrement(rotor3_pos);
          }
     }


     // While bruteforcing rotor positions, save calculations by rotating the rotor instead of recalculating it
     void increment_rotor (int rotor)
     {
          assert(1 <= rotor && rotor <= 3);

          switch (rotor)
          {
               case 1 :
                    increment(rotor1_pos);
                    break;
               case 2 :
                    increment(rotor2_pos);
                    break;
               case 3 :
                    increment(rotor3_pos);
          }
     }


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
     char alpha[78];

     int rotor1_turnoverA;
     int rotor1_turnoverB;
     int rotor2_turnoverA;
     int rotor2_turnoverB;
     int rotor1_pos;
     int rotor2_pos;
     int rotor3_pos;

     // Relative positions during encryption
     int rotor1_offset;
     int rotor2_offset;
     int rotor3_offset;


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


     // Operation ------------------------------------------------------------------------------------------------------
     void increment (int& pos)
     {
          pos += pos < 25 ? 1 : -25;
     }


     void decrement (int& pos)
     {
          pos -= pos > 0 ? 1 : -25;
     }


     void step_positions ()
     {
          if (rotor2_offset == rotor2_turnoverA || rotor2_offset == rotor2_turnoverB)
          {
               increment(rotor3_offset);
               increment(rotor2_offset);
          }

          if (rotor1_offset == rotor1_turnoverA || rotor1_offset == rotor1_turnoverB)
               increment(rotor2_offset);

          increment(rotor1_offset);
     }


     int run_rotor (const int* rotor, int offset, int pos) const
     {
          return rotor[pos + offset] - offset + 26;
     }


     char encrypt_letter (char keypressed)
     {
          return alpha[encrypt_ordinal(keypressed - 'A')];
     }


     int encrypt_ordinal (int j)
     {
          step_positions();

          j = plugboardA[j];

          j = stator_forward[j];
          j = run_rotor(rotor1_forward, rotor1_offset, j);
          j = run_rotor(rotor2_forward, rotor2_offset, j);
          j = run_rotor(rotor3_forward, rotor3_offset, j);
          j = reflector[j];
          j = run_rotor(rotor3_reverse, rotor3_offset, j);
          j = run_rotor(rotor2_reverse, rotor2_offset, j);
          j = run_rotor(rotor1_reverse, rotor1_offset, j);
          j = stator_reverse[j];

          return plugboardB[j];
     }

}; // class Enigma



