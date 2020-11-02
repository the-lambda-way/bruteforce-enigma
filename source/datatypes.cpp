#include "datatypes.h"


// ---------------------------------------------------------------------------------------------------------------------
// Text validation
// ---------------------------------------------------------------------------------------------------------------------
Plugboard::Plugboard (const char* pairs)
: Plugboard {std::string_view{pairs}}
{}

Plugboard::Plugboard (std::string_view pairs)
: ordinals {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}
{
     if (pairs.empty())     return;

     int i = 0;
     while (is_space(pairs[i]))     ++i;

     while (i != pairs.length())
     {
          if (!is_alpha(pairs[i]))
               throw std::domain_error("Plugboard pairs can only have alphabetical characters.");

          int a = to_ordinal(pairs[i]);

          if (ordinals[a] != a)
               throw std::invalid_argument("Plugboard pair characters must all be unique.");

          if (i + 1 == pairs.length())
               throw std::invalid_argument("Unpaired character in plugboard pairs.");

          if (!is_alpha(pairs[i + 1]))
               throw std::domain_error("Plugboard pairs can only have alphabetical characters.");

          int b = to_ordinal(pairs[i + 1]);

          if (ordinals[b] != b)
               throw std::invalid_argument("Plugboard pair characters must all be unique.");

          std::swap(ordinals[a], ordinals[b]);

          i += 2;
          while (is_space(pairs[i]))     ++i;
     }
}


std::string Plugboard::to_string () const
{
     std::array<bool, 26> was_seen = {false, false, false, false, false, false, false, false, false, false,
                                      false, false, false, false, false, false, false, false, false, false,
                                      false, false, false, false, false, false};

     std::string out = "";

     for (int i = 0; i < 26; ++i)
     {
          if (was_seen[i])     continue;

          was_seen[i]           = true;
          was_seen[ordinals[i]] = true;

          char a = i + 'A';
          char b = ordinals[i] + 'A';

          if (a <= b)
          {
               out.push_back(a);
               out.push_back(b);
          }
          else
          {
               out.push_back(b);
               out.push_back(a);
          }

          out.push_back(' ');
     }

     if (!out.empty())     out.pop_back();     // remove extra space at the end

     return out;
}


std::ostream& operator<< (std::ostream& os, const Plugboard& plug)
{
     os << plug.to_string();
     return os;
}


Ciphertext::Ciphertext (std::string text, std::vector<int> ordinals)
: text {std::move(text)}, ordinals {std::move(ordinals)}
{}


Ciphertext::Ciphertext (const char* text)
: Ciphertext {std::string_view{text}}
{}


Ciphertext::Ciphertext (std::string_view text)
: text {text}
{
     for (char c : text | std::views::filter(is_alpha))
          ordinals.push_back(to_ordinal(c));
}


bool Ciphertext::operator== (const Ciphertext& rhs) const
{
     return ordinals == rhs.ordinals;
}

bool Ciphertext::operator== (std::string_view rhs) const
{
     return operator==(Ciphertext{rhs});
}


std::ostream& operator<< (std::ostream& os, const Ciphertext& ct)
{
     os << ct.text;
     return os;
}


// ---------------------------------------------------------------------------------------------------------------------
// Enigma Keys
// ---------------------------------------------------------------------------------------------------------------------
EnigmaKey::EnigmaKey ()
: EnigmaKey {default_rotor, default_rotor, default_rotor, default_rotor, default_rotor, default_plugboard}
{}


EnigmaKey::EnigmaKey (
     const Rotor& stator,        // ETW
     const Rotor& rotor1,
     const Rotor& rotor2,
     const Rotor& rotor3,
     const Rotor& reflector,     // UKW
     const Plugboard& plugboard,
     int rotor1_pos,
     int rotor2_pos,
     int rotor3_pos,
     int ring1_pos,
     int ring2_pos,
     int ring3_pos
)
: stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, reflector {&reflector},
  plug_ordinals {&plugboard.ordinals},
  rotor1_pos {modular_int<26>{rotor1_pos}}, rotor2_pos {modular_int<26>{rotor2_pos}},
  rotor3_pos {modular_int<26>{rotor3_pos}}, ring1_pos {modular_int<26>{ring1_pos}},
  ring2_pos {modular_int<26>{ring2_pos}}, ring3_pos {modular_int<26>{ring3_pos}}
{}


EnigmaKey::EnigmaKey (
     const Rotor& stator,        // ETW
     const Rotor& rotor1,
     const Rotor& rotor2,
     const Rotor& rotor3,
     const Rotor& reflector,     // UKW
     const Plugboard& plugboard,
     const char* rotor_positions,
     const char* ring_positions
)
: stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, reflector {&reflector},
  plug_ordinals {&plugboard.ordinals}
{
     RotorPositions<3> rotors {rotor_positions};
     rotor1_pos = rotors.positions[0];
     rotor2_pos = rotors.positions[1];
     rotor3_pos = rotors.positions[2];

     RingPositions<3> rings {ring_positions};
     ring1_pos = rings.positions[0];
     ring2_pos = rings.positions[1];
     ring3_pos = rings.positions[2];
}


Enigma4Key::Enigma4Key ()
: Enigma4Key {default_rotor, default_rotor, default_rotor, default_rotor,
              default_rotor, default_rotor, default_plugboard}
{}


Enigma4Key::Enigma4Key (
     const Rotor& stator,        // ETW
     const Rotor& rotor1,
     const Rotor& rotor2,
     const Rotor& rotor3,
     const Rotor& rotor4,
     const Rotor& reflector,     // UKW
     const Plugboard& plugboard,
     int rotor1_pos,
     int rotor2_pos,
     int rotor3_pos,
     int rotor4_pos,
     int ring1_pos,
     int ring2_pos,
     int ring3_pos,
     int ring4_pos
)
: stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, rotor4 {&rotor4}, reflector {&reflector},
  plug_ordinals {&plugboard.ordinals},
  rotor1_pos {modular_int<26>{rotor1_pos}}, rotor2_pos {modular_int<26>{rotor2_pos}},
  rotor3_pos {modular_int<26>{rotor3_pos}}, rotor4_pos {modular_int<26>{rotor4_pos}},
  ring1_pos {modular_int<26>{ring1_pos}}, ring2_pos {modular_int<26>{ring2_pos}},
  ring3_pos {modular_int<26>{ring3_pos}}, ring4_pos {modular_int<26>{ring4_pos}}
{}


Enigma4Key::Enigma4Key (
     const Rotor& stator,        // ETW
     const Rotor& rotor1,
     const Rotor& rotor2,
     const Rotor& rotor3,
     const Rotor& rotor4,
     const Rotor& reflector,     // UKW
     const Plugboard& plugboard,
     const char* rotor_positions,
     const char* ring_positions
)
: stator {&stator}, rotor1 {&rotor1}, rotor2 {&rotor2}, rotor3 {&rotor3}, rotor4 {&rotor4}, reflector {&reflector},
  plug_ordinals {&plugboard.ordinals}
{
     RotorPositions<4> rotors {rotor_positions};
     rotor1_pos = rotors.positions[0];
     rotor2_pos = rotors.positions[1];
     rotor3_pos = rotors.positions[2];
     rotor4_pos = rotors.positions[3];

     RingPositions<4> rings {ring_positions};
     ring1_pos = rings.positions[0];
     ring2_pos = rings.positions[1];
     ring3_pos = rings.positions[2];
     ring4_pos = rings.positions[3];
}