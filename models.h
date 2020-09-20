#pragma once

#include <string_view>
#include <vector>
#include "rotors.h"


// https://www.cryptomuseum.com/crypto/enigma/m3/index.htm
// https://en.wikipedia.org/wiki/Enigma_rotor_details

// M3 ------------------------------------------------------------------------------------------------------------------
Rotor m3_I    = {"I",    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q"};
Rotor m3_II   = {"II",   "AJDKSIRUXBLHWTMCQGZNPYFVOE", "E"};
Rotor m3_III  = {"III",  "BDFHJLCPRTXVZNYEIWGAKMUSQO", "V"};
Rotor m3_IV   = {"IV",   "ESOVPZJAYQUIRHXLNFTGKDCMWB", "J"};
Rotor m3_V    = {"V",    "VZBRGITYUPSDNHLXAWMJQOFECK", "Z"};
Rotor m3_VI   = {"VI",   "JPGVOUMFYQBENHZRDKASXLICTW", "ZM"};
Rotor m3_VII  = {"VII",  "NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM"};
Rotor m3_VIII = {"VIII", "FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM"};

Rotor m3_ETW = {"ETW",   "ABCDEFGHIJKLMNOPQRSTUVWXYZ", ""};

Rotor m3_UKWA = {"UKWA", "EJMZALYXVBWFCRQUONTSPIKHGD", ""};
Rotor m3_UKWB = {"UKWB", "YRUHQSLDPXNGOKMIEBFZCWVJAT", ""};
Rotor m3_UKWC = {"UKWC", "FVPJIAOYEDRZXWGCTKUQSBNMHL", ""};

EnigmaModel m3_model = {&m3_ETW,
                        {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V, &m3_VI, &m3_VII, &m3_VIII},
                        {&m3_UKWB, &m3_UKWC}};


// Railway -------------------------------------------------------------------------------------------------------------
Rotor railway_I   = {"I",   "JGDQOXUSCAMIFRVTPNEWKBLZYH", "N"};
Rotor railway_II  = {"II",  "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "E"};
Rotor railway_III = {"III", "JVIUBHTCDYAKEQZPOSGXNRMWFL", "Y"};

Rotor railway_ETW = {"ETW", "QWERTZUIOASDFGHJKPYXCVBNML", ""};
Rotor railway_UKW = {"UKW", "QYHOGNECVPUZTFDJAXWMKISRBL", ""};

EnigmaModel railway_model = {&railway_ETW,
                             {&railway_I, &railway_II, &railway_III},
                             {&railway_UKW}};


// CrypTool railway ----------------------------------------------------------------------------------------------------
// CrypTool turns over on the notch position on the wheel, when it should turnover on the turnover (window) position.
Rotor CrypTool_railway_I   = {"I",   "JGDQOXUSCAMIFRVTPNEWKBLZYH", "V"};
Rotor CrypTool_railway_II  = {"II",  "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "M"};
Rotor CrypTool_railway_III = {"III", "JVIUBHTCDYAKEQZPOSGXNRMWFL", "G"};

Rotor CrypTool_railway_ETW = {"ETW", "QWERTZUIOASDFGHJKPYXCVBNML", ""};
Rotor CrypTool_railway_UKW = {"UKW", "QYHOGNECVPUZTFDJAXWMKISRBL", ""};

EnigmaModel CrypTool_railway_model = {&CrypTool_railway_ETW,
                                      {&CrypTool_railway_I, &CrypTool_railway_II, &CrypTool_railway_III},
                                      {&CrypTool_railway_UKW}};
