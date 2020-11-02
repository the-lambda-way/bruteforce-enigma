#include "enigma.h"


// rotor_offsets -------------------------------------------------------------------------------------------------------
void rotor_offsets::reset ()
{
     one   = rotor1;
     two   = rotor2;
     three = rotor3;
}


// TODO: Test with a turnover offset that advances when hit. (E.g. cache the next turnover, possibly unroll loops.)
void rotor_offsets::step ()
{
     // Tested with a highly optimized switch statement. The if-branch algorithm was 20% more efficient than the switch.
     // The solution used below was 20% more efficient than the if-branch algorithm for any number of notches.

     three.add_unsafely(static_cast<int>(turnover2[two]));
     two.add_unsafely(static_cast<int>(turnover2[two]) + static_cast<int>(turnover1[one]));

     ++one;

     precalculate_deltas();
}


void rotor_offsets::rotr_turnover1 ()
{
     turnover1 = (turnover1 >> 1) | (turnover1 << 25);
}


void rotor_offsets::rotr_turnover2 ()
{
     turnover2 = (turnover2 >> 1) | (turnover2 << 25);
}


void rotor_offsets::precalculate_deltas ()
{
     // If we don't cast first, then the modular subtraction will be called. We've avoided the need for this modularity
     // check by making these deltas type int and the rotor array values large enough to handle the range.
     three_minus_two = static_cast<int>(three) - static_cast<int>(two);
     two_minus_one   = static_cast<int>(two) - static_cast<int>(one);
}


// rotor4_offsets -------------------------------------------------------------------------------------------------------
void rotor4_offsets::reset ()
{
     one   = rotor1;
     two   = rotor2;
     three = rotor3;
     four  = rotor4;
}


void rotor4_offsets::step ()
{
     three.add_unsafely(static_cast<int>(turnover2[two]));
     two.add_unsafely(static_cast<int>(turnover2[two]) + static_cast<int>(turnover1[one]));

     ++one;

     precalculate_deltas();
}


void rotor4_offsets::rotr_turnover1 ()
{
     turnover1 = (turnover1 >> 1) | (turnover1 << 25);
}


void rotor4_offsets::rotr_turnover2 ()
{
     turnover2 = (turnover2 >> 1) | (turnover2 << 25);
}


void rotor4_offsets::precalculate_deltas ()
{
     four_minus_three = static_cast<int>(four) - static_cast<int>(three);
     three_minus_two  = static_cast<int>(three) - static_cast<int>(two);
     two_minus_one    = static_cast<int>(two) - static_cast<int>(one);
}



// Enigma_optimize_unknown_positions<EnigmaKey> ------------------------------------------------------------------------
Enigma_optimize_unknown_positions<EnigmaKey>::Enigma_optimize_unknown_positions (
     const basic_enigma_base<EnigmaKey>& base,
     const Plugboard& plugboard,
     int rotor1_start,
     int rotor2_start,
     int rotor3_start,
     int ring1_start,
     int ring2_start,
     int ring3_start
)
: Enigma_optimize_unknown_positions {
     EnigmaKey {*base.stator, *base.rotor1, *base.rotor2, *base.rotor3, *base.reflector, plugboard,
                    rotor1_start, rotor2_start, rotor3_start, ring1_start, ring2_start, ring3_start}}
{}


Enigma_optimize_unknown_positions<EnigmaKey>::Enigma_optimize_unknown_positions (EnigmaKey key)
: key {std::move(key)}, offsets {this->key}
{
     // the plugboard_stator_forward should have the range 0 <= i < 26
     std::transform(
          std::execution::par_unseq,
          this->key.plug_ordinals->begin(), this->key.plug_ordinals->end(),
          plugboard_stator_forward.begin(),
          [this] (int o) { return this->key.stator->forward[o] - 26; }
     );

     // rotors keep the range 26 <= i < 52
     std::copy(std::execution::par_unseq,
               this->key.rotor1->forward.begin(), this->key.rotor1->forward.begin() + rotor1_forward.size(),
               rotor1_forward.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor2->forward.begin(), this->key.rotor2->forward.end(),
               rotor2_forward.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor3->forward.begin(), this->key.rotor3->forward.end(),
               rotor3_forward.begin());

     // the reflector should have the range 0 <= i < 26
     std::transform(
          std::execution::par_unseq,
          this->key.reflector->forward.begin(), this->key.reflector->forward.begin() + reflector.size(),
          reflector.begin(),
          [] (int o) { return o - 26; }
     );

     // rotors keep the range 26 <= i < 52
     std::copy(std::execution::par_unseq,
               this->key.rotor3->reverse.begin(), this->key.rotor3->reverse.begin() + rotor3_reverse.size(),
               rotor3_reverse.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor2->reverse.begin(), this->key.rotor2->reverse.end(),
               rotor2_reverse.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor1->reverse.begin(), this->key.rotor1->reverse.end(),
               rotor1_reverse.begin());

     // the plugboard_stator_forward should have the range 0 <= i < 26
     std::transform(
          std::execution::par_unseq,
          this->key.stator->reverse.begin(), this->key.stator->reverse.begin() + 26,
          stator_plugboard_reverse.begin(),
          [this] (int o) { return (*this->key.plug_ordinals)[o - 26]; }
     );

     std::copy(std::execution::par_unseq,
               stator_plugboard_reverse.begin(), stator_plugboard_reverse.begin() + 26,
               stator_plugboard_reverse.begin() + 26);
}


EnigmaKey Enigma_optimize_unknown_positions<EnigmaKey>::get_key () const
{
     EnigmaKey copy = key;
     copy.rotor1_pos = offsets.rotor1 + copy.ring1_pos;
     copy.rotor2_pos = offsets.rotor2 + copy.ring2_pos;
     copy.rotor3_pos = offsets.rotor3 + copy.ring3_pos;

     return copy;
}


void Enigma_optimize_unknown_positions<EnigmaKey>::increment_ring1 ()
{
     // incrementing the ring decrements the rotor offset
     ++key.ring1_pos;
     --offsets.rotor1;
     offsets.rotr_turnover1();
}


void Enigma_optimize_unknown_positions<EnigmaKey>::increment_ring2 ()
{
     ++key.ring2_pos;
     --offsets.rotor2;
     offsets.rotr_turnover2();
}


void Enigma_optimize_unknown_positions<EnigmaKey>::increment_ring3 ()
{
     ++key.ring3_pos;
     --offsets.rotor3;
}


void Enigma_optimize_unknown_positions<EnigmaKey>::increment_rotor1 ()     { ++offsets.rotor1; }
void Enigma_optimize_unknown_positions<EnigmaKey>::increment_rotor2 ()     { ++offsets.rotor2; }
void Enigma_optimize_unknown_positions<EnigmaKey>::increment_rotor3 ()     { ++offsets.rotor3; }


int Enigma_optimize_unknown_positions<EnigmaKey>::encrypt_ordinal (int j)
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


// Enigma_optimize_unknown_positions<Enigma4Key> -----------------------------------------------------------------------
Enigma_optimize_unknown_positions<Enigma4Key>::Enigma_optimize_unknown_positions (
     const basic_enigma_base<Enigma4Key>& base,
     const Plugboard& plugboard,
     int rotor1_start,
     int rotor2_start,
     int rotor3_start,
     int rotor4_start,
     int ring1_start,
     int ring2_start,
     int ring3_start,
     int ring4_start
)
: Enigma_optimize_unknown_positions {
     Enigma4Key {*base.stator, *base.rotor1, *base.rotor2, *base.rotor3, *base.rotor4, *base.reflector, plugboard,
                    rotor1_start, rotor2_start, rotor3_start, rotor4_start,
                    ring1_start, ring2_start, ring3_start, ring4_start}}
{}


Enigma_optimize_unknown_positions<Enigma4Key>::Enigma_optimize_unknown_positions (Enigma4Key key)
: key {std::move(key)}, offsets {this->key}
{
     // the plugboard_stator_forward should have the range 0 <= i < 26
     std::transform(
          std::execution::par_unseq,
          this->key.plug_ordinals->begin(), this->key.plug_ordinals->end(),
          plugboard_stator_forward.begin(),
          [this] (int o) { return this->key.stator->forward[o] - 26; }
     );

     // rotors keep the range 26 <= i < 52
     std::copy(std::execution::par_unseq,
               this->key.rotor1->forward.begin(), this->key.rotor1->forward.begin() + rotor1_forward.size(),
               rotor1_forward.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor2->forward.begin(), this->key.rotor2->forward.end(),
               rotor2_forward.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor3->forward.begin(), this->key.rotor3->forward.end(),
               rotor3_forward.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor4->forward.begin(), this->key.rotor4->forward.end(),
               rotor4_forward.begin());

     // the reflector should have the range 0 <= i < 26
     std::transform(
          std::execution::par_unseq,
          this->key.reflector->forward.begin(), this->key.reflector->forward.begin() + reflector.size(),
          reflector.begin(),
          [] (int o) { return o - 26; }
     );

     // rotors keep the range 26 <= i < 52
     std::copy(std::execution::par_unseq,
               this->key.rotor4->reverse.begin(), this->key.rotor4->reverse.begin() + rotor4_reverse.size(),
               rotor4_reverse.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor3->reverse.begin(), this->key.rotor3->reverse.end(),
               rotor3_reverse.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor2->reverse.begin(), this->key.rotor2->reverse.end(),
               rotor2_reverse.begin());
     std::copy(std::execution::par_unseq,
               this->key.rotor1->reverse.begin(), this->key.rotor1->reverse.end(),
               rotor1_reverse.begin());

     // the plugboard_stator_forward should have the range 0 <= i < 26
     std::transform(
          std::execution::par_unseq,
          this->key.stator->reverse.begin(), this->key.stator->reverse.begin() + 26,
          stator_plugboard_reverse.begin(),
          [this] (int o) { return (*this->key.plug_ordinals)[o - 26]; }
     );

     std::copy(std::execution::par_unseq,
               stator_plugboard_reverse.begin(), stator_plugboard_reverse.begin() + 26,
               stator_plugboard_reverse.begin() + 26);
}


Enigma4Key Enigma_optimize_unknown_positions<Enigma4Key>::get_key () const
{
     Enigma4Key copy = key;
     copy.rotor1_pos = offsets.rotor1 + copy.ring1_pos;
     copy.rotor2_pos = offsets.rotor2 + copy.ring2_pos;
     copy.rotor3_pos = offsets.rotor3 + copy.ring3_pos;
     copy.rotor4_pos = offsets.rotor4 + copy.ring4_pos;

     return copy;
}


void Enigma_optimize_unknown_positions<Enigma4Key>::increment_ring1 ()
{
     // incrementing the ring decrements the rotor offset
     ++key.ring1_pos;
     --offsets.rotor1;
     offsets.rotr_turnover1();
}


void Enigma_optimize_unknown_positions<Enigma4Key>::increment_ring2 ()
{
     ++key.ring2_pos;
     --offsets.rotor2;
     offsets.rotr_turnover2();
}


void Enigma_optimize_unknown_positions<Enigma4Key>::increment_ring3 ()
{
     ++key.ring3_pos;
     --offsets.rotor3;
}


void Enigma_optimize_unknown_positions<Enigma4Key>::increment_ring4 ()
{
     ++key.ring4_pos;
     --offsets.rotor4;
}


void Enigma_optimize_unknown_positions<Enigma4Key>::increment_rotor1 ()     { ++offsets.rotor1; }
void Enigma_optimize_unknown_positions<Enigma4Key>::increment_rotor2 ()     { ++offsets.rotor2; }
void Enigma_optimize_unknown_positions<Enigma4Key>::increment_rotor3 ()     { ++offsets.rotor3; }
void Enigma_optimize_unknown_positions<Enigma4Key>::increment_rotor4 ()     { ++offsets.rotor4; }


int Enigma_optimize_unknown_positions<Enigma4Key>::encrypt_ordinal (int j)
{
     offsets.step();

     // plugboard and stator don't move
     j = plugboard_stator_forward[j] + offsets.one;

     // each rotor returns the range 26 <= i < 52, this avoids needing to modulo j
     j = rotor1_forward[j] + offsets.two_minus_one;
     j = rotor2_forward[j] + offsets.three_minus_two;
     j = rotor3_forward[j] + offsets.four_minus_three;
     j = rotor4_forward[j] - offsets.four;

     // reflector returns the range 0 <= i < 26
     j = reflector[j] + offsets.four;

     // each rotor returns the range 26 <= i < 52, this avoids needing to modulo j
     j = rotor4_reverse[j] - offsets.four_minus_three;
     j = rotor3_reverse[j] - offsets.three_minus_two;
     j = rotor2_reverse[j] - offsets.two_minus_one;
     j = rotor1_reverse[j] - offsets.one;

     // stator and plugboard don't move
     return stator_plugboard_reverse[j];
}
