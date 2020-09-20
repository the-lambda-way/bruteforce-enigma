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
     : Enigma(EnigmaConfiguration {base, plugboard, ring_positions, rotor_positions})
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
     : Enigma(EnigmaConfiguration {base, plugboard,
                                   ring1_pos, ring2_pos, ring3_pos, rotor1_pos, rotor2_pos, rotor3_pos})
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
          init_plug(this->config.plugboard, plugboardA);

          init_rotor(this->config.stator->forward,    stator_forward);
          init_rotor(this->config.rotor1->forward,    rotor1_forward);
          init_rotor(this->config.rotor2->forward,    rotor2_forward);
          init_rotor(this->config.rotor3->forward,    rotor3_forward);
          init_rotor(this->config.reflector->forward, reflector);
          init_rotor(this->config.rotor3->reverse,    rotor3_reverse);
          init_rotor(this->config.rotor2->reverse,    rotor2_reverse);
          init_rotor(this->config.rotor1->reverse,    rotor1_reverse);
          init_rotor(this->config.stator->reverse,    stator_reverse);

          init_plug(this->config.plugboard, plugboardB);

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
          offset1 = rotor1_offset;
          offset2 = rotor2_offset;
          offset3 = rotor3_offset;

          std::string s = "";
          s.reserve(input.length());

          for (auto c : input)     s += encrypt_letter(c);

          return s;
     }


     // Out must have one extra element than input, for '\0'
     void encrypt (char* out, std::string_view input)
     {
          offset1 = rotor1_offset;
          offset2 = rotor2_offset;
          offset3 = rotor3_offset;

          for (int i = 0; i < input.length(); ++i)
               out[i] = encrypt_letter(input[i]);

          out[input.length()] = '\0';
     }


     // Out must have one extra element than length, for '\0'
     void encrypt (char* out, int* ordinals, int length)
     {
          offset1 = rotor1_offset;
          offset2 = rotor2_offset;
          offset3 = rotor3_offset;

          for (int i = 0; i < length; ++i)
               out[i] = alpha[encrypt_ordinal(ordinals[i])];

          out[length] = '\0';
     }


     void reset_ring_pos (int ring, int pos)
     {
          assert(1 <= ring && ring <= 3);
          assert(0 <= pos && pos <= 25);

          switch (ring)
          {
               case 1 :
                    config.ring1_pos = pos;

                    turnover1A_offset = calculate_offset(config.rotor1->turnoverA, pos);
                    turnover1B_offset = calculate_offset(config.rotor1->turnoverB, pos);

                    rotor1_offset = calculate_offset(config.rotor1_pos, pos);
                    break;
               case 2 :
                    config.ring2_pos = pos;

                    turnover2A_offset = calculate_offset(config.rotor2->turnoverA, pos);
                    turnover2B_offset = calculate_offset(config.rotor2->turnoverB, pos);

                    rotor2_offset = calculate_offset(config.rotor2_pos, pos);
                    break;
               case 3 :
                    config.ring3_pos = pos;
                    rotor3_offset = calculate_offset(config.rotor3_pos, pos);
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
                    rotor1_offset = calculate_offset(pos, config.ring1_pos);
                    break;
               case 2 :
                    config.rotor2_pos = pos;
                    rotor2_offset = calculate_offset(pos, config.ring2_pos);
                    break;
               case 3 :
                    config.rotor3_pos = pos;
                    rotor3_offset = calculate_offset(pos, config.ring3_pos);
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
                    increment(config.ring1_pos);
                    decrement(rotor1_offset);
                    decrement(turnover1A_offset);
                    decrement(turnover1B_offset);
                    break;
               case 2 :
                    increment(config.ring2_pos);
                    decrement(rotor2_offset);
                    decrement(turnover2A_offset);
                    decrement(turnover2B_offset);
                    break;
               case 3 :
                    increment(config.ring3_pos);
                    decrement(rotor3_offset);
          }
     }


     // While bruteforcing rotor positions, save calculations by rotating the rotor instead of recalculating it
     void increment_rotor (int rotor)
     {
          assert(1 <= rotor && rotor <= 3);

          switch (rotor)
          {
               case 1 :
                    increment(config.rotor1_pos);
                    increment(rotor1_offset);
                    break;
               case 2 :
                    increment(config.rotor2_pos);
                    increment(rotor2_offset);
                    break;
               case 3 :
                    increment(config.rotor3_pos);
                    increment(rotor3_offset);
          }
     }


     EnigmaConfiguration get_config ()     { return config; }


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

     // Rotor offsets combine
     int rotor1_offset;
     int rotor2_offset;
     int rotor3_offset;
     int turnover1A_offset;
     int turnover1B_offset;
     int turnover2A_offset;
     int turnover2B_offset;

     // Relative positions during encryption
     int offset1;
     int offset2;
     int offset3;


     void init_rotor (const int* in, int* out)
     {
          std::copy(in, in + 78, out);
     }


     void init_plug (std::string_view str, int* out)
     {
          std::copy(str.begin(), str.end(), out);

          for (int i = 0; i < 26; ++i)     *out++ -= 'A';
     }


     int calculate_offset (int rotor_pos, int ring_pos) const
     {
          return rotor_pos - ring_pos + (rotor_pos < ring_pos ? 26 : 0);
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
          if (offset2 == turnover2A_offset || offset2 == turnover2B_offset)
          {
               increment(offset3);
               increment(offset2);
          }

          if (offset1 == turnover1A_offset || offset1 == turnover1B_offset)
               increment(offset2);

          increment(offset1);
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
          j = run_rotor(rotor1_forward, offset1, j);
          j = run_rotor(rotor2_forward, offset2, j);
          j = run_rotor(rotor3_forward, offset3, j);
          j = reflector[j];
          j = run_rotor(rotor3_reverse, offset3, j);
          j = run_rotor(rotor2_reverse, offset2, j);
          j = run_rotor(rotor1_reverse, offset1, j);
          j = stator_reverse[j];

          return plugboardB[j];
     }

}; // class Enigma



