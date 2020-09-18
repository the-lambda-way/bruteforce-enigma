#pragma once

#include <functional>         // std::ref
#include <string_view>
#include <vector>


// ---------------------------------------------------------------------------------------------------------------------
// Structures
// ---------------------------------------------------------------------------------------------------------------------
struct Rotor
{
     std::string_view pretty_name;
     std::string_view forward;
     std::string_view reverse;
     std::string_view turnovers;
};


// Assumes all uppercase and length 26
std::string inverse_rotor_string (std::string_view forward)
{
     std::string inverse(26, '-');

     for (int i = 0; i < 26; ++i)
     {
          inverse[forward[i] - 'A'] = 'A' + i;
     }

     return inverse;
}


// Useful for creating permutations of different settings.
struct EnigmaModel
{
     using container = std::vector<std::reference_wrapper<const Rotor>>;

     const Rotor& stator;         // ETW
     container    rotors;
     container    reflectors;     // UKW
};


// Useful when cracking rotor and ring positions or plugboards.
struct EnigmaBase
{
     const Rotor& stator;        // ETW
     const Rotor& rotor1;
     const Rotor& rotor2;
     const Rotor& rotor3;
     const Rotor& reflector;     // UKW


     EnigmaBase (const Rotor& stator,
                 const Rotor& rotor1, const Rotor& rotor2, const Rotor& rotor3,
                 const Rotor& reflector)
     : stator {stator}, rotor1 {rotor1}, rotor2 {rotor2}, rotor3 {rotor3}, reflector {reflector}
     {}


     EnigmaBase (const EnigmaBase& c)
     : stator {c.stator}, rotor1 {c.rotor1}, rotor2 {c.rotor2}, rotor3 {c.rotor3}, reflector {c.reflector}
     {}


     EnigmaBase (EnigmaBase&& c)
     : stator {c.stator}, rotor1 {c.rotor1}, rotor2 {c.rotor2}, rotor3 {c.rotor3}, reflector {c.reflector}
     {}
};


// Useful for passing in settings, or retaining notable results.
struct EnigmaConfiguration
{
     const Rotor& stator;        // ETW
     const Rotor& rotor1;
     const Rotor& rotor2;
     const Rotor& rotor3;
     const Rotor& reflector;     // UKW
     std::string_view plugboard;
     int ring1_pos;
     int ring2_pos;
     int ring3_pos;
     int rotor1_pos;
     int rotor2_pos;
     int rotor3_pos;


     EnigmaConfiguration (
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
     : stator {stator}, rotor1 {rotor1}, rotor2 {rotor2}, rotor3 {rotor3}, reflector {reflector},
       plugboard {plugboard}, ring1_pos {ring1_pos}, ring2_pos {ring2_pos}, ring3_pos {ring3_pos},
       rotor1_pos {rotor1_pos}, rotor2_pos {rotor2_pos}, rotor3_pos {rotor3_pos}
     {}


     EnigmaConfiguration (const EnigmaConfiguration& c)
     : EnigmaConfiguration(c.stator, c.rotor1, c.rotor2, c.rotor3, c.reflector, c.plugboard,
                           c.ring1_pos, c.ring2_pos, c.ring3_pos, c.rotor1_pos, c.rotor2_pos, c.rotor3_pos)
     {}


     EnigmaConfiguration (EnigmaConfiguration&& c)
     : EnigmaConfiguration(c.stator, c.rotor1, c.rotor2, c.rotor3, c.reflector, c.plugboard,
                           c.ring1_pos, c.ring2_pos, c.ring3_pos, c.rotor1_pos, c.rotor2_pos, c.rotor3_pos)
     {}


     EnigmaConfiguration (
          const EnigmaBase& config,
          std::string_view plugboard,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0
     )
     : EnigmaConfiguration(config.stator, config.rotor1, config.rotor2, config.rotor3, config.reflector,
                           plugboard, ring1_pos, ring2_pos, ring3_pos, rotor1_pos, rotor2_pos, rotor3_pos)
     {}


     EnigmaConfiguration (const EnigmaBase& config,
                          std::string_view plugboard,
                          std::string_view ring_positions,
                          std::string_view rotor_positions
     )
     : EnigmaConfiguration(config.stator, config.rotor1, config.rotor2, config.rotor3, config.reflector,
                           plugboard,
                           ring_positions[0] - 'A', ring_positions[1] - 'A', ring_positions[2] - 'A',
                           rotor_positions[0] - 'A', rotor_positions[1] - 'A', rotor_positions[2] - 'A')
     {}
};


// ---------------------------------------------------------------------------------------------------------------------
// Models
// ---------------------------------------------------------------------------------------------------------------------
// https://www.cryptomuseum.com/crypto/enigma/m3/index.htm
// https://en.wikipedia.org/wiki/Enigma_rotor_details

// M3
Rotor m3_I    = {"I",    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", "UWYGADFPVZBECKMTHXSLRINQOJ", "Q"};
Rotor m3_II   = {"II",   "AJDKSIRUXBLHWTMCQGZNPYFVOE", "AJPCZWRLFBDKOTYUQGENHXMIVS", "E"};
Rotor m3_III  = {"III",  "BDFHJLCPRTXVZNYEIWGAKMUSQO", "TAGBPCSDQEUFVNZHYIXJWLRKOM", "V"};
Rotor m3_IV   = {"IV",   "ESOVPZJAYQUIRHXLNFTGKDCMWB", "HZWVARTNLGUPXQCEJMBSKDYOIF", "J"};
Rotor m3_V    = {"V",    "VZBRGITYUPSDNHLXAWMJQOFECK", "QCYLXWENFTZOSMVJUDKGIARPHB", "Z"};
Rotor m3_VI   = {"VI",   "JPGVOUMFYQBENHZRDKASXLICTW", "SKXQLHCNWARVGMEBJPTYFDZUIO", "ZM"};
Rotor m3_VII  = {"VII",  "NZJHGRCXMYSWBOUFAIVLPEKQDT", "QMGYVPEDRCWTIANUXFKZOSLHJB", "ZM"};
Rotor m3_VIII = {"VIII", "FKQHTLXOCBJSPDZRAMEWNIUYGV", "QJINSAYDVKBFRUHMCPLEWZTGXO", "ZM"};

Rotor m3_ETW = {"ETW",   "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", ""};

Rotor m3_UKWA = {"UKWA", "EJMZALYXVBWFCRQUONTSPIKHGD", "", ""};
Rotor m3_UKWB = {"UKWB", "YRUHQSLDPXNGOKMIEBFZCWVJAT", "", ""};
Rotor m3_UKWC = {"UKWC", "FVPJIAOYEDRZXWGCTKUQSBNMHL", "", ""};

EnigmaModel m3_model = {m3_ETW,
                        {m3_I, m3_II, m3_III, m3_IV, m3_V},
                        {m3_UKWB, m3_UKWC}};


// Railway
Rotor railway_I   = {"I",   "JGDQOXUSCAMIFRVTPNEWKBLZYH", "JVICSMBZLAUWKREQDNHPGOTFYX", "N"};
Rotor railway_II  = {"II",  "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "SGMOTFYXPNIRJAHDZLEBVQKWUC", "E"};
Rotor railway_III = {"III", "JVIUBHTCDYAKEQZPOSGXNRMWFL", "KEHIMYSFCALZWUQPNVRGDBXTJO", "Y"};

Rotor railway_ETW = {"ETW", "QWERTZUIOASDFGHJKPYXCVBNML", "JWULCMNOHPQZYXIRADKEGVBTSF", ""};
Rotor railway_UKW = {"UKW", "QYHOGNECVPUZTFDJAXWMKISRBL", "", ""};

EnigmaModel railway_model = {railway_ETW,
                             {railway_I, railway_II, railway_III},
                             {railway_UKW}};


// CrypTool railway
// CrypTool turns over on the notch position on the wheel, when it should turnover on the turnover (window) position.
Rotor CrypTool_railway_I   = {"I",   "JGDQOXUSCAMIFRVTPNEWKBLZYH", "JVICSMBZLAUWKREQDNHPGOTFYX", "V"};
Rotor CrypTool_railway_II  = {"II",  "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "SGMOTFYXPNIRJAHDZLEBVQKWUC", "M"};
Rotor CrypTool_railway_III = {"III", "JVIUBHTCDYAKEQZPOSGXNRMWFL", "KEHIMYSFCALZWUQPNVRGDBXTJO", "G"};

Rotor CrypTool_railway_ETW = {"ETW", "QWERTZUIOASDFGHJKPYXCVBNML", "JWULCMNOHPQZYXIRADKEGVBTSF", ""};
Rotor CrypTool_railway_UKW = {"UKW", "QYHOGNECVPUZTFDJAXWMKISRBL", "", ""};

EnigmaModel CrypTool_railway_model = {CrypTool_railway_ETW,
                                      {CrypTool_railway_I, CrypTool_railway_II, CrypTool_railway_III},
                                      {CrypTool_railway_UKW}};
