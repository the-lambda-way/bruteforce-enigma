#include "enigma.h"

#include <cassert>       // set_ring, set_rotor
#include <execution>


// Out must have the same length or greater as str
void str_to_ordinals (std::string_view str, std::span<int> out)
{
     std::transform(
          std::execution::par_unseq,
          str.begin(), str.end(), out.begin(),
          [] (char c) { return c - 'A'; }
     );
}


// Operation ------------------------------------------------------------------------------------------------------
std::string Enigma::encrypt (std::string_view input)
{
     offset1 = rotor1_offset;
     offset2 = rotor2_offset;
     offset3 = rotor3_offset;

     std::string s;
     s.reserve(input.length());

     for (auto c : input)     s.push_back(encrypt_letter(c));

     return s;
}


void Enigma::encrypt (std::span<const int> ordinals, std::span<int> out)
{
     offset1 = rotor1_offset;
     offset2 = rotor2_offset;
     offset3 = rotor3_offset;

     std::ranges::transform(ordinals, out.begin(), [this] (int o) { return encrypt_ordinal(o); });
}



void Enigma::reset_ring_pos (int ring, int pos)
{
     assert(1 <= ring && ring <= 3);
     assert(0 <= pos && pos <= 25);

     switch (ring)
     {
          case 1 :
               key.ring1_pos = pos;

               turnover1A_offset = calculate_offset(key.rotor1->turnoverA, pos);
               turnover1B_offset = calculate_offset(key.rotor1->turnoverB, pos);

               rotor1_offset = calculate_offset(key.rotor1_pos, pos);
               break;
          case 2 :
               key.ring2_pos = pos;

               turnover2A_offset = calculate_offset(key.rotor2->turnoverA, pos);
               turnover2B_offset = calculate_offset(key.rotor2->turnoverB, pos);

               rotor2_offset = calculate_offset(key.rotor2_pos, pos);
               break;
          case 3 :
               key.ring3_pos = pos;
               rotor3_offset = calculate_offset(key.rotor3_pos, pos);
     }
}


void Enigma::reset_rotor_pos (int rotor, int pos)
{
     assert(1 <= rotor && rotor <= 3);
     assert(0 <= pos && pos <= 25);

     switch (rotor)
     {
          case 1 :
               key.rotor1_pos = pos;
               rotor1_offset  = calculate_offset(pos, key.ring1_pos);
               break;
          case 2 :
               key.rotor2_pos = pos;
               rotor2_offset  = calculate_offset(pos, key.ring2_pos);
               break;
          case 3 :
               key.rotor3_pos = pos;
               rotor3_offset  = calculate_offset(pos, key.ring3_pos);
     }
}


// While bruteforcing ring positions, save calculations by rotating the rotor instead of recalculating it
void Enigma::increment_ring (int ring)
{
     assert(1 <= ring && ring <= 3);

     // incrementing the ring decrements the rotor offset
     switch (ring)
     {
          case 1 :
               key.ring1_pos     = increment_of(key.ring1_pos);
               rotor1_offset     = decrement_of(rotor1_offset);
               turnover1A_offset = decrement_of(turnover1A_offset);
               turnover1B_offset = decrement_of(turnover1B_offset);
               break;
          case 2 :
               key.ring2_pos     = increment_of(key.ring2_pos);
               rotor2_offset     = decrement_of(rotor2_offset);
               turnover2A_offset = decrement_of(turnover2A_offset);
               turnover2B_offset = decrement_of(turnover2B_offset);
               break;
          case 3 :
               key.ring3_pos = increment_of(key.ring3_pos);
               rotor3_offset = decrement_of(rotor3_offset);
     }
}


// While bruteforcing rotor positions, save calculations by rotating the rotor instead of recalculating it
void Enigma::increment_rotor (int rotor)
{
     assert(1 <= rotor && rotor <= 3);

     switch (rotor)
     {
          case 1 :
               key.rotor1_pos = increment_of(key.rotor1_pos);
               rotor1_offset  = increment_of(rotor1_offset);
               break;
          case 2 :
               key.rotor2_pos = increment_of(key.rotor2_pos);
               rotor2_offset  = increment_of(rotor2_offset);
               break;
          case 3 :
               key.rotor3_pos = increment_of(key.rotor3_pos);
               rotor3_offset  = increment_of(rotor3_offset);
     }
}


EnigmaKey Enigma::get_key ()     { return key; }



// Optimization ---------------------------------------------------------------------------------------------------
void Enigma::init_rotor (const int* in, int* out)
{
     std::copy(in, in + 78, out);
}


constexpr int Enigma::calculate_offset (int rotor_pos, int ring_pos) const
{
     int offset = rotor_pos - ring_pos;
     return offset < 0 ? offset + 26 : offset;
}


// Operation ------------------------------------------------------------------------------------------------------
constexpr int Enigma::increment_of (int pos) const
{
     return pos == 25 ? 0 : pos + 1;
}


constexpr int Enigma::decrement_of (int pos) const
{
     return pos == 0 ? 25 : pos - 1;
}


void Enigma::step_positions ()
{
     if (offset2 == turnover2A_offset || offset2 == turnover2B_offset)
     {
          offset3 = increment_of(offset3);
          offset2 = increment_of(offset2);
     }

     if (offset1 == turnover1A_offset || offset1 == turnover1B_offset)
          offset2 = increment_of(offset2);

     offset1 = increment_of(offset1);
}


int Enigma::run_rotor (const int* rotor, int offset, int pos) const
{
     return rotor[pos + offset] - offset + 26;
}


char Enigma::encrypt_letter (char keypressed)
{
     return alpha[encrypt_ordinal(keypressed - 'A')];
}


int Enigma::encrypt_ordinal (int j)
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




