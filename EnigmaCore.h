#pragma once

#include <array>
#include <string>
#include <string_view>

using namespace std;


// =====================================================================================================================
// Parts
// =====================================================================================================================
struct Rotor
{
     string forward;
     string reverse;
     string turnover;
};


struct EnigmaConfiguration
{
     string_view  plugboard;
     string_view  stator;        // ETW
     const Rotor& rotor1;
     const Rotor& rotor2;
     const Rotor& rotor3;
     string_view  reflector;     // UKW
};


// https://www.cryptomuseum.com/crypto/enigma/m3/index.htm
// https://en.wikipedia.org/wiki/Enigma_rotor_details

// M3
Rotor m3_I    = {"EKMFLGDQVZNTOWYHXUSPAIBRCJ", "UWYGADFPVZBECKMTHXSLRINQOJ", "Q"};
Rotor m3_II   = {"AJDKSIRUXBLHWTMCQGZNPYFVOE", "AJPCZWRLFBDKOTYUQGENHXMIVS", "E"};
Rotor m3_III  = {"BDFHJLCPRTXVZNYEIWGAKMUSQO", "TAGBPCSDQEUFVNZHYIXJWLRKOM", "V"};
Rotor m3_IV   = {"ESOVPZJAYQUIRHXLNFTGKDCMWB", "HZWVARTNLGUPXQCEJMBSKDYOIF", "J"};
Rotor m3_V    = {"VZBRGITYUPSDNHLXAWMJQOFECK", "QCYLXWENFTZOSMVJUDKGIARPHB", "Z"};
Rotor m3_VI   = {"JPGVOUMFYQBENHZRDKASXLICTW", "SKXQLHCNWARVGMEBJPTYFDZUIO", "ZM"};
Rotor m3_VII  = {"NZJHGRCXMYSWBOUFAIVLPEKQDT", "QMGYVPEDRCWTIANUXFKZOSLHJB", "ZM"};
Rotor m3_VIII = {"FKQHTLXOCBJSPDZRAMEWNIUYGV", "QJINSAYDVKBFRUHMCPLEWZTGXO", "ZM"};

string_view m3_ETW = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string_view UKWA = "EJMZALYXVBWFCRQUONTSPIKHGD";
string_view UKWB = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
string_view UKWC = "FVPJIAOYEDRZXWGCTKUQSBNMHL";

Rotor m3_rotors[] = {m3_I, m3_II, m3_III, m3_IV, m3_V, m3_VI, m3_VII, m3_VIII};


// Railway
Rotor railway_I   = {"JGDQOXUSCAMIFRVTPNEWKBLZYH", "JVICSMBZLAUWKREQDNHPGOTFYX", "N"};
Rotor railway_II  = {"NTZPSFBOKMWRCJDIVLAEYUXHGQ", "SGMOTFYXPNIRJAHDZLEBVQKWUC", "E"};
Rotor railway_III = {"JVIUBHTCDYAKEQZPOSGXNRMWFL", "KEHIMYSFCALZWUQPNVRGDBXTJO", "Y"};

string_view railway_ETW = "QWERTZUIOASDFGHJKPYXCVBNML";
string_view railway_UKM = "QYHOGNECVPUZTFDJAXWMKISRBL";


// CryptTool railway
// CryptTool turns over on the notch position on the wheel, when it should turnover on the turnover (window) position.
Rotor CryptTool_railway_I   = {"JGDQOXUSCAMIFRVTPNEWKBLZYH", "JVICSMBZLAUWKREQDNHPGOTFYX", "V"};
Rotor CryptTool_railway_II  = {"NTZPSFBOKMWRCJDIVLAEYUXHGQ", "SGMOTFYXPNIRJAHDZLEBVQKWUC", "M"};
Rotor CryptTool_railway_III = {"JVIUBHTCDYAKEQZPOSGXNRMWFL", "KEHIMYSFCALZWUQPNVRGDBXTJO", "G"};



// =====================================================================================================================
// Core
// =====================================================================================================================



template <size_t N>
array<int, N> str_to_indices (const char* s)
{
     array<int, N> out;

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
          string_view plugboard)

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

          rotor1notch = char_to_ordinal(notches[rotor1]);
          rotor2notch = char_to_ordinal(notches[rotor2]);
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

          rotor1notch = char_to_ordinal(notches[rotor1]);
          rotor2notch = char_to_ordinal(notches[rotor2]);
     }


     string encrypt (
          int rotor1pos, int rotor2pos, int rotor3pos,
          string_view input)
     {
          this->rotor1pos = calculate_offset(rotor1pos, ring1pos);
          this->rotor2pos = calculate_offset(rotor2pos, ring2pos);
          this->rotor3pos = calculate_offset(rotor3pos, ring3pos);

          rotor1notch = calculate_offset(rotor1notch, ring1pos);
          rotor2notch = calculate_offset(rotor2notch, ring2pos);

          // can maybe optimize this further
          string s = "";
          s.reserve(input.length());

          for (auto c : input)     s += encrypt_letter(c);

          return s;
     }


     const array<int, 78> rotorsFor[3] = {
          /* I   */ str_to_indices<78>("JGDQOXUSCAMIFRVTPNEWKBLZYHJGDQOXUSCAMIFRVTPNEWKBLZYHJGDQOXUSCAMIFRVTPNEWKBLZYH"),
          /* II  */ str_to_indices<78>("NTZPSFBOKMWRCJDIVLAEYUXHGQNTZPSFBOKMWRCJDIVLAEYUXHGQNTZPSFBOKMWRCJDIVLAEYUXHGQ"),
          /* III */ str_to_indices<78>("JVIUBHTCDYAKEQZPOSGXNRMWFLJVIUBHTCDYAKEQZPOSGXNRMWFLJVIUBHTCDYAKEQZPOSGXNRMWFL")
     };

     const array<int, 78> rotorsRev[3] = {
          str_to_indices<78>("JVICSMBZLAUWKREQDNHPGOTFYXJVICSMBZLAUWKREQDNHPGOTFYXJVICSMBZLAUWKREQDNHPGOTFYX"),
          str_to_indices<78>("SGMOTFYXPNIRJAHDZLEBVQKWUCSGMOTFYXPNIRJAHDZLEBVQKWUCSGMOTFYXPNIRJAHDZLEBVQKWUC"),
          str_to_indices<78>("KEHIMYSFCALZWUQPNVRGDBXTJOKEHIMYSFCALZWUQPNVRGDBXTJOKEHIMYSFCALZWUQPNVRGDBXTJO")
     };

     // Note: CryptTool turns over on the notch position on the wheel, when it should turnover on the turnover (window)
     // position.
     string_view notches = "VMG";     // pawl positions
     // string_view notches = "NEY";     // actual window turnovers


private:
     int rotor1;
     int rotor2;
     int rotor3;
     int rotor1pos;
     int rotor2pos;
     int rotor3pos;
     int ring1pos;
     int ring2pos;
     int ring3pos;



     a = rotor1pos;
     b = rotor2pos;
     c = rotor3pos;


     // plugboard[statorRev[rotor1Rev[rotor2Rev[rotor3Rev[reflector[rotor3For[rotor2For[rotor1For[statorFor[plugboard[i]] + a] - a + 26 + b] - b + 26 + c] - c + 26] + c] - c + 26 + b] - b + 26 + a] - a + 26]];


     // Let rotor3pos be the left side. Let x = some constant


     // caches
     f(c) = return [] (a, b) { return rotor3Rev[reflector[rotor3For[g(b) + 26 + c] - c + 26] + c] - c; };
     g(b) = return [] (a)    { return rotor2For[rotor1For[statorFor[plugboard[i]] + a] - a + 26 + b] - b; };
     h(b) = return [] (c)    { return rotor2Rev[f(c) + 26 + b] - b; };





     i = plugboard[statorRev[rotor1Rev[h(b) + 26 + a] - a + 26]];






     f(c)    = [] (i) { return rotor3For[reflector[rotor3Rev[i + c] - c + 26] + c] - c + 26; };
     g(b, c) = [] (i) { return rotor2Rev[f(c)(rotor2For[i + b] - b + 26) + b] - b + 26; };



     i = plugboard[i];

     i = statorFor[i];
     i = run_rotor(rotor1For, a, i);
     i = g(b, c)(i);
     i = run_rotor(rotor1Rev, a, i);
     i = statorRev[i];

     i = plugboard[i];

     return alpha[i];






     const array<int, 26> plugboard;
     array<int, 78> statorFor = str_to_indices<78>("QWERTZUIOASDFGHJKPYXCVBNMLQWERTZUIOASDFGHJKPYXCVBNMLQWERTZUIOASDFGHJKPYXCVBNML"); // ETW
     array<int, 78> rotor1For;
     array<int, 78> rotor2For;
     array<int, 78> rotor3For;
     array<int, 78> reflector = str_to_indices<78>("QYHOGNECVPUZTFDJAXWMKISRBLQYHOGNECVPUZTFDJAXWMKISRBLQYHOGNECVPUZTFDJAXWMKISRBL"); // UKW
     array<int, 78> rotor3Rev;
     array<int, 78> rotor2Rev;
     array<int, 78> rotor1Rev;
     array<int, 78> statorRev = str_to_indices<78>("JWULCMNOHPQZYXIRADKEGVBTSFJWULCMNOHPQZYXIRADKEGVBTSFJWULCMNOHPQZYXIRADKEGVBTSF");
     const array<int, 26> plugboard;
     char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ";

     int rotor1notch;
     int rotor2notch;


     int calculate_offset (int rotor_pos, int ring_pos) const
     {
          return (rotor_pos - (ring_pos - 1) + 26) % 26;
     }


     int char_to_ordinal (char c) const     { return c - 'A'; }
     char ordinal_to_char (int i) const     { return i + 'A'; }


     void step_positions ()
     {
          if (rotor2notch == rotor2pos)
          {
               rotor3pos = (rotor3pos + 1) % 26;
               rotor2pos = (rotor2pos + 1) % 26;
          }

          if (rotor1notch == rotor1pos)
               rotor2pos = (rotor2pos + 1) % 26;

          rotor1pos = (rotor1pos + 1) % 26;
     }


     int run_rotor (const array<int, 78>& wheel, int offset, int i) const
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

