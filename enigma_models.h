#pragma once

#include <string_view>
#include <vector>


struct Rotor
{
     std::string_view pretty_name;
     std::string_view forward;
     std::string_view reverse;
     std::string_view turnovers;
};


struct EnigmaModel
{
     Rotor              stator;         // ETW
     std::vector<Rotor> rotors;
     std::vector<Rotor> reflectors;     // UKW
};


struct EnigmaConfiguration
{
     const Rotor& stator;        // ETW
     const Rotor& rotor1;
     const Rotor& rotor2;
     const Rotor& rotor3;
     const Rotor& reflector;     // UKW

     EnigmaConfiguration (const Rotor& stator,
                          const Rotor& rotor1, const Rotor& rotor2, const Rotor& rotor3,
                          const Rotor& reflector)
     : stator {stator}, rotor1 {rotor1}, rotor2 {rotor2}, rotor3 {rotor3}, reflector {reflector}
     {}

     EnigmaConfiguration (const EnigmaConfiguration& c)
     : stator {c.stator}, rotor1 {c.rotor1}, rotor2 {c.rotor2}, rotor3 {c.rotor3}, reflector {c.reflector}
     {}

     EnigmaConfiguration (EnigmaConfiguration&& c)
     : stator {c.stator}, rotor1 {c.rotor1}, rotor2 {c.rotor2}, rotor3 {c.rotor3}, reflector {c.reflector}
     {}
};


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
