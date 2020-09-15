#pragma once

#include <array>
#include <string>
#include <string_view>


template <size_t N>
std::array<int, N> str_to_indices (const char* s)
{
     std::array<int, N> out;

     for (int i = 0; i < N; ++i)
          out[i] = s[i] - 'A';

     return out;
}


class EnigmaCore
{
public:
     EnigmaCore ()
     : rotor1 {0}, rotor2 {1}, rotor3 {2},
       ring1pos {1}, ring2pos {1}, ring3pos {1},
       plugboard {str_to_indices<26>("ABCDEFGHIJKLMNOPQRSTUVWXYZ")}
     {
          rotor1For = rotorsFor[rotor1];
          rotor2For = rotorsFor[rotor2];
          rotor3For = rotorsFor[rotor3];

          rotor1Rev = rotorsRev[rotor1];
          rotor2Rev = rotorsRev[rotor2];
          rotor3Rev = rotorsRev[rotor3];

          rotor1notch = char_to_ordinal(notches[rotor1]);
          rotor2notch = char_to_ordinal(notches[rotor2]);
     }


     // ring positions start at 1
     EnigmaCore (
          int rotor1, int rotor2, int rotor3,
          int ring1pos, int ring2pos, int ring3pos,
          std::string_view plugboard)

     : rotor1 {rotor1}, rotor2 {rotor2}, rotor3 {rotor3},
       ring1pos {ring1pos}, ring2pos {ring2pos}, ring3pos {ring3pos},
       plugboard {str_to_indices<26>(plugboard.data())}
     {
          rotor1For = rotorsFor[rotor1];
          rotor2For = rotorsFor[rotor2];
          rotor3For = rotorsFor[rotor3];

          rotor1Rev = rotorsRev[rotor1];
          rotor2Rev = rotorsRev[rotor2];
          rotor3Rev = rotorsRev[rotor3];

          rotor1notch = calculate_offset(char_to_ordinal(notches[rotor1]), ring1pos);
          rotor2notch = calculate_offset(char_to_ordinal(notches[rotor2]), ring2pos);
     }


     void reinit (
          int rotor1, int rotor2, int rotor3,
          int ring1pos, int ring2pos, int ring3pos)
     {
          this->rotor1 = rotor1;
          this->rotor2 = rotor2;
          this->rotor2 = rotor3;
          this->ring1pos = ring1pos;
          this->ring2pos = ring2pos;
          this->ring3pos = ring3pos;

          rotor1For = rotorsFor[rotor1];
          rotor2For = rotorsFor[rotor2];
          rotor3For = rotorsFor[rotor3];

          rotor1Rev = rotorsRev[rotor1];
          rotor2Rev = rotorsRev[rotor2];
          rotor3Rev = rotorsRev[rotor3];

          rotor1notch = calculate_offset(char_to_ordinal(notches[rotor1]), ring1pos);
          rotor2notch = calculate_offset(char_to_ordinal(notches[rotor2]), ring2pos);
     }


     std::string encrypt (
          int rotor1pos, int rotor2pos, int rotor3pos,
          std::string_view input)
     {
          this->rotor1pos = calculate_offset(rotor1pos, ring1pos);
          this->rotor2pos = calculate_offset(rotor2pos, ring2pos);
          this->rotor3pos = calculate_offset(rotor3pos, ring3pos);

          // can maybe optimize this further
          std::string s = "";
          s.reserve(input.length());

          for (auto c : input)     s += encrypt_letter(c);

          return s;
     }


private:
     const std::array<int, 78> rotorsFor[3] = {
          /* I   */ str_to_indices<78>("JGDQOXUSCAMIFRVTPNEWKBLZYHJGDQOXUSCAMIFRVTPNEWKBLZYHJGDQOXUSCAMIFRVTPNEWKBLZYH"),
          /* II  */ str_to_indices<78>("NTZPSFBOKMWRCJDIVLAEYUXHGQNTZPSFBOKMWRCJDIVLAEYUXHGQNTZPSFBOKMWRCJDIVLAEYUXHGQ"),
          /* III */ str_to_indices<78>("JVIUBHTCDYAKEQZPOSGXNRMWFLJVIUBHTCDYAKEQZPOSGXNRMWFLJVIUBHTCDYAKEQZPOSGXNRMWFL")
     };

     const std::array<int, 78> rotorsRev[3] = {
          str_to_indices<78>("JVICSMBZLAUWKREQDNHPGOTFYXJVICSMBZLAUWKREQDNHPGOTFYXJVICSMBZLAUWKREQDNHPGOTFYX"),
          str_to_indices<78>("SGMOTFYXPNIRJAHDZLEBVQKWUCSGMOTFYXPNIRJAHDZLEBVQKWUCSGMOTFYXPNIRJAHDZLEBVQKWUC"),
          str_to_indices<78>("KEHIMYSFCALZWUQPNVRGDBXTJOKEHIMYSFCALZWUQPNVRGDBXTJOKEHIMYSFCALZWUQPNVRGDBXTJO")
     };

     // Note: CryptTool turns over on the notch position on the wheel, when it should turnover on the turnover (window)
     // position.
     std::string_view notches = "VMG";     // pawl positions
     // std::string_view notches = "NEY";     // actual window turnovers

     int rotor1;
     int rotor2;
     int rotor3;
     int rotor1pos;
     int rotor2pos;
     int rotor3pos;
     int ring1pos;
     int ring2pos;
     int ring3pos;

     std::array<int, 26> plugboard;
     std::array<int, 78> statorFor = str_to_indices<78>("QWERTZUIOASDFGHJKPYXCVBNMLQWERTZUIOASDFGHJKPYXCVBNMLQWERTZUIOASDFGHJKPYXCVBNML"); // ETW
     std::array<int, 78> rotor1For;
     std::array<int, 78> rotor2For;
     std::array<int, 78> rotor3For;
     std::array<int, 78> reflector = str_to_indices<78>("QYHOGNECVPUZTFDJAXWMKISRBLQYHOGNECVPUZTFDJAXWMKISRBLQYHOGNECVPUZTFDJAXWMKISRBL"); // UKW
     std::array<int, 78> rotor3Rev;
     std::array<int, 78> rotor2Rev;
     std::array<int, 78> rotor1Rev;
     std::array<int, 78> statorRev = str_to_indices<78>("JWULCMNOHPQZYXIRADKEGVBTSFJWULCMNOHPQZYXIRADKEGVBTSFJWULCMNOHPQZYXIRADKEGVBTSF");
     char alpha[79] = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";

     int rotor1notch;
     int rotor2notch;


     int calculate_offset (int rotor_pos, int ring_pos) const
     {
          return (rotor_pos - (ring_pos - 1) + 26) % 26;
     }


     int char_to_ordinal (char c) const     { return c - 'A'; }


     void step_positions ()
     {
          // Every step should make a new cache

          if (rotor2notch == rotor2pos)
          {
               rotor3pos = (rotor3pos + 1) % 26;
               rotor2pos = (rotor2pos + 1) % 26;
          }

          if (rotor1notch == rotor1pos)
               rotor2pos = (rotor2pos + 1) % 26;

          rotor1pos = (rotor1pos + 1) % 26;
     }


     int run_rotor (const std::array<int, 78>& wheel, int offset, int i) const
     {
          // permutation modulo offset, plus 26 to mod negative values
          return wheel[i + offset] - offset + 26;
     }


     char encrypt_letter (char keypressed)
     {
          step_positions();

          int i = char_to_ordinal(keypressed);

          i = plugboard[i];

          i = statorFor[i];
          i = run_rotor(rotor1For, rotor1pos, i);
          i = run_rotor(rotor2For, rotor2pos, i);
          i = run_rotor(rotor3For, rotor3pos, i);
          i = reflector[i];
          i = run_rotor(rotor3Rev, rotor3pos, i);
          i = run_rotor(rotor2Rev, rotor2pos, i);
          i = run_rotor(rotor1Rev, rotor1pos, i);
          i = statorRev[i];

          i = plugboard[i];

          return alpha[i];
     }

}; // EnigmaCore

