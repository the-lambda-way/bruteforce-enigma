#include "enigma.h"

#include <cassert>       // set_ring, set_rotor
#include <execution>


std::vector<int> str_to_ordinals (std::string_view str)
{
     std::vector<int> out;
     out.reserve(str.length());

     for (char c : str)    out.push_back(c - 'A');

     return out;
}


std::string ordinals_to_str (std::span<const int> ordinals)
{
     std::string out;
     out.reserve(ordinals.size());

     for (int o : ordinals)    out.push_back(o + 'A');

     return out;
}


// rotor_offsets -------------------------------------------------------------------------------------------------------
void rotor_offsets::reset ()
{
     one   = rotor1;
     two   = rotor2;
     three = rotor3;
}


// TODO: Test with a turnover offset that advances when hit.
void rotor_offsets::step ()
{
     // Tested with a highly optimized switch statement. The if-branch algorithm was 20% more efficient.
     // The solution used below was 20% more efficient than the if-branch algorithm for any number of notches.

     two   += static_cast<int>(turnover2[two]);
     three += static_cast<int>(turnover2[two]);
     two   += static_cast<int>(turnover1[one]);     // Must come last to enact double-stepping in the correct order

     ++one;

     three_minus_two = three - two;
     two_minus_one   = two - one;
}


void rotor_offsets::rotr_turnover1 ()
{
     turnover1 = (turnover1 >> 1) | (turnover1 << 25);
}



void rotor_offsets::rotr_turnover2 ()
{
     turnover2 = (turnover2 >> 1) | (turnover2 << 25);
}


// Enigma_optimize_unknown_positions -----------------------------------------------------------------------------------
EnigmaKey Enigma_optimize_unknown_positions::get_key () const
{
     EnigmaKey copy = key;
     copy.rotor1_pos = offsets.rotor1 + copy.ring1_pos;
     copy.rotor2_pos = offsets.rotor2 + copy.ring2_pos;
     copy.rotor3_pos = offsets.rotor3 + copy.ring3_pos;

     return copy;
}


void Enigma_optimize_unknown_positions::increment_ring1 ()
{
     // incrementing the ring decrements the rotor offset
     ++key.ring1_pos;
     --offsets.rotor1;
     offsets.rotr_turnover1();
}


void Enigma_optimize_unknown_positions::increment_ring2 ()
{
     ++key.ring2_pos;
     --offsets.rotor2;
     offsets.rotr_turnover2();
}


void Enigma_optimize_unknown_positions::increment_ring3 ()
{
     ++key.ring3_pos;
     --offsets.rotor3;
}


void Enigma_optimize_unknown_positions::increment_rotor1 ()     { ++offsets.rotor1; }
void Enigma_optimize_unknown_positions::increment_rotor2 ()     { ++offsets.rotor2; }
void Enigma_optimize_unknown_positions::increment_rotor3 ()     { ++offsets.rotor3; }


int Enigma_optimize_unknown_positions::encrypt_ordinal (int j)
{
     offsets.step();

     // plugboard and stator don't move
     j = plugboard_stator_forward[j] + offsets.one;

     // each rotor returns the range 26 <= i < 52, this avoids needing to modulo j
     j = rotor1_forward[j] + offsets.two_minus_one;
     j = rotor2_forward[j] + offsets.three_minus_two;
     j = rotor3_forward[j] - offsets.three;

     // reflector returns the range 0 <= i < 26
     j = reflector[j] + offsets.three;

     // each rotor returns the range 26 <= i < 52, this avoids needing to modulo j
     j = rotor3_reverse[j] - offsets.three_minus_two;
     j = rotor2_reverse[j] - offsets.two_minus_one;
     j = rotor1_reverse[j] - offsets.one;

     // stator and plugboard don't move
     return stator_plugboard_reverse[j];
}


// Enigma --------------------------------------------------------------------------------------------------------------
std::string Enigma::encrypt (std::string_view input)
{
     const std::vector<int> in = str_to_ordinals(input);
     std::vector<int> out(input.length());

     enigma.encrypt(in, out.begin());

     return ordinals_to_str(out);
}
