#pragma once

#include "rotors.h"


// Useful: https://en.wikipedia.org/wiki/Enigma_rotor_details


// Common --------------------------------------------------------------------------------------------------------------
Rotor ETW_ABCDEF = {"ETW", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", ""};
Rotor ETW_QWERTZ = {"ETW", "QWERTZUIOASDFGHJKPYXCVBNML", ""};

Rotor UKWA           = {"UKWA", "EJMZALYXVBWFCRQUONTSPIKHGD", ""};
Rotor UKWB           = {"UKWB", "YRUHQSLDPXNGOKMIEBFZCWVJAT", ""};
Rotor UKWC           = {"UKWC", "FVPJIAOYEDRZXWGCTKUQSBNMHL", ""};
Rotor commercial_UKW = {"com",  "IMETCGFRAYSQBZXWLHKDVUPOJN", ""};


// M3 ------------------------------------------------------------------------------------------------------------------
// https://www.cryptomuseum.com/crypto/enigma/m3/index.htm

Rotor m3_I    = {"I",    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q"};
Rotor m3_II   = {"II",   "AJDKSIRUXBLHWTMCQGZNPYFVOE", "E"};
Rotor m3_III  = {"III",  "BDFHJLCPRTXVZNYEIWGAKMUSQO", "V"};
Rotor m3_IV   = {"IV",   "ESOVPZJAYQUIRHXLNFTGKDCMWB", "J"};
Rotor m3_V    = {"V",    "VZBRGITYUPSDNHLXAWMJQOFECK", "Z"};
Rotor m4_VI   = {"VI",   "JPGVOUMFYQBENHZRDKASXLICTW", "ZM"};
Rotor m4_VII  = {"VII",  "NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM"};
Rotor m4_VIII = {"VIII", "FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM"};

EnigmaModel m3_model = {.stator     = &ETW_ABCDEF,
                        .rotors     = {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V},
                        .reflectors = {&UKWB, &UKWC}};

EnigmaModel m3_extended_model = {.stator     = &ETW_ABCDEF,
                                 .rotors     = {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V, &m4_VI, &m4_VII, &m4_VIII},
                                 .reflectors = {&UKWB, &UKWC}};


// M4 ------------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

// Note: Simulator currently only supports three rotors.

Rotor m4_UKWB_thin = {"Bthin", "ENKQAUYWJICOPBLMDXZVFTHRGS", ""};
Rotor m4_UKWC_thin = {"Cthin", "RDOBJNTKVEHMLFCWZAXGYIPSUQ", ""};
Rotor m4_beta      = {"beta",  "LEYJVCNIXWPBQMDRTAKZGFUHOS", ""};
Rotor m4_gamma     = {"gamma", "FSOKANUERHMBTIYCWLQPZXVGJD", ""};

EnigmaModel m4_model = {.stator     = &ETW_ABCDEF,
                        .rotors     = {&m4_beta, &m4_gamma, &m3_I, &m3_II, &m3_III, &m3_IV, &m3_V, &m4_VI, &m4_VII, &m4_VIII},
                        .reflectors = {&m4_UKWB_thin, &m4_UKWC_thin}};


// Railway -------------------------------------------------------------------------------------------------------------
// https://www.cryptomuseum.com/crypto/enigma/k/railway.htm

Rotor railway_I   = {"I",   "JGDQOXUSCAMIFRVTPNEWKBLZYH", "N"};
Rotor railway_II  = {"II",  "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "E"};
Rotor railway_III = {"III", "JVIUBHTCDYAKEQZPOSGXNRMWFL", "Y"};

Rotor railway_UKW = {"UKW", "QYHOGNECVPUZTFDJAXWMKISRBL", ""};

EnigmaModel railway_model = {.stator     = &ETW_QWERTZ,
                             .rotors     = {&railway_I, &railway_II, &railway_III},
                             .reflectors = {&railway_UKW}};


// CrypTool railway ----------------------------------------------------------------------------------------------------
// CrypTool turns over on the notch position on the wheel, when it should turnover on the indicator position.
Rotor CrypTool_railway_I   = {"I",   "JGDQOXUSCAMIFRVTPNEWKBLZYH", "V"};
Rotor CrypTool_railway_II  = {"II",  "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "M"};
Rotor CrypTool_railway_III = {"III", "JVIUBHTCDYAKEQZPOSGXNRMWFL", "G"};

EnigmaModel CrypTool_railway_model = {.stator     = &ETW_QWERTZ,
                                      .rotors     = {&CrypTool_railway_I, &CrypTool_railway_II, &CrypTool_railway_III},
                                      .reflectors = {&railway_UKW}};


// Enigma I ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

EnigmaModel enigmaI_model = {.stator     = &ETW_ABCDEF,
                             .rotors     = {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V},
                             .reflectors = {&UKWA, &UKWB, &UKWC}};


// Enigma I Norenigma --------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

Rotor norenigma_I   = {"I",   "WTOKASUYVRBXJHQCPZEFMDINLG", "Q"};
Rotor norenigma_II  = {"II",  "GJLPUBSWEMCTQVHXAOFZDRKYNI", "E"};
Rotor norenigma_III = {"III", "JWFMHNBPUSDYTIXVZGRQLAOEKC", "V"};
Rotor norenigma_IV  = {"IV",  "ESOVPZJAYQUIRHXLNFTGKDCMWB", "J"};
Rotor norenigma_V   = {"V",   "HEJXQOTZBVFDASCILWPGYNMURK", "Z"};

Rotor norenigma_UKW = {"UKW", "MOWJYPUXNDSRAIBFVLKZGQCHET", ""};

EnigmaModel enigma_n_model = {.stator     = &ETW_ABCDEF,
                              .rotors     = {&norenigma_I, &norenigma_II, &norenigma_III, &norenigma_IV, &norenigma_V},
                              .reflectors = {&norenigma_UKW}};


// Enigma I Sondermaschine ---------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

Rotor sondermaschine_I   = {"I",   "VEOSIRZUJDQCKGWYPNXAFLTHMB", "Q"};
Rotor sondermaschine_II  = {"II",  "UEMOATQLSHPKCYFWJZBGVXINDR", "E"};
Rotor sondermaschine_III = {"III", "TZHXMBSIPNURJFDKEQVCWGLAOY", "V"};

Rotor sondermaschine_UKW = {"UKW", "CIAGSNDRBYTPZFULVHEKOQXWJM", ""};

EnigmaModel enigma_s_model = {.stator     = &ETW_ABCDEF,
                              .rotors     = {&sondermaschine_I, &sondermaschine_II, &sondermaschine_III},
                              .reflectors = {&sondermaschine_UKW}};


// Enigma D ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

Rotor commercial_I   = {"I",   "LPGSZMHAEOQKVXRFYBUTNICJDW", "Y"};
Rotor commercial_II  = {"II",  "SLVGBTFXJQOHEWIRZYAMKPCNDU", "E"};
Rotor commercial_III = {"III", "CJGDPSHKTURAWZXFMYNQOBVLIE", "N"};

EnigmaModel commercial_model = {.stator     = &ETW_QWERTZ,
                                .rotors     = {&commercial_I, &commercial_II, &commercial_III},
                                .reflectors = {&commercial_UKW}};


// Enigma T ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

// Note: Doesn't yet support more than two notches.

// Rotor tirpitz_ETW  = {"ETW", "ILXRZTKGJYAMWVDUFCPQEONSHB", ""};

// Rotor tirpitz_I    = {"I",    "KPTYUELOCVGRFQDANJMBSWHZXI", "WZEKQ"};
// Rotor tirpitz_II   = {"II",   "UPHZLWEQMTDJXCAKSOIGVBYFNR", "WZFLR"};
// Rotor tirpitz_III  = {"III",  "QUDLYRFEKONVZAXWHMGPJBSICT", "WZEKQ"};
// Rotor tirpitz_IV   = {"IV",   "CIWTBKXNRESPFLYDAGVHQUOJZM", "WZFLR"};
// Rotor tirpitz_V    = {"V",    "UAXGISNJBVERDYLFZWTPCKOHMQ", "YCFKR"};
// Rotor tirpitz_VI   = {"VI",   "XFUZGALVHCNYSEWQTDMRBKPIOJ", "XEIMQ"};
// Rotor tirpitz_VII  = {"VII",  "BJVFTXPLNAYOZIKWGDQERUCHSM", "YCFKR"};
// Rotor tirpitz_VIII = {"VIII", "YMTPNZHWKODAJXELUQVGCBISFR", "XEIMQ"};

// Rotor tirpitz_UKW  = {"UKW", "GEKPBTAUMOCNILJDXZYFHWVQSR", ""};

// EnigmaModel tirpitz_model = {.stator     = &tirpitz_ETW,
//                              .rotors     = {&tirpitz_I, &tirpitz_II, &tirpitz_III, &tirpitz_IV, &tirpitz_V, &tirpitz_VII, &tirpitz_VIII},
//                              .reflectors = {&tirpitz_UKW}};


// Enigma K ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

// Note: Doesn't yet support more than two notches.

// Rotor swissK_I   = {"I",   "PEZUOHXSCVFMTBGLRINQJWAYDK", "Y"};
// Rotor swissK_II  = {"II",  "ZOUESYDKFWPCIQXHMVBLGNJRAT", "E"};
// Rotor swissK_III = {"III", "EHRVXGAOBQUSIMZFLYNWKTPDJC", "N"};

// EnigmaModel swissK_model = {.stator     = &ETW_QWERTZ,
//                             .rotors     = {&swissK_I, &swissK_II, &swissK_III},
//                             .reflectors = {&commercial_UKW}};


// Enigma Z ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

// Note: Doesn't yet support more than two notches.

// Rotor zahlwerk_I   = {"I",   "LPGSZMHAEOQKVXRFYBUTNICJDW", "SUVWZABCEFGIKLOPQ"};
// Rotor zahlwerk_II  = {"II",  "SLVGBTFXJQOHEWIRZYAMKPCNDU", "STVYZACDFGHKMNQ"};
// Rotor zahlwerk_III = {"III", "CJGDPSHKTURAWZXFMYNQOBVLIE", "UWXAEFHKMNR"};

// EnigmaModel zahlwerk_model = {.stator     = &ETW_QWERTZ,
//                               .rotors     = {&zahlwerk_I, &zahlwerk_II, &zahlwerk_III},
//                               .reflectors = {&commercial_UKW}};


// Enigma G111 ---------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

// Note: Doesn't yet support more than two notches.

// Rotor g111_I  = {"I",  "WLRHBQUNDKJCZSEXOTMAGYFPVI", "SUVWZABCEFGIKLOPQ"};
// Rotor g111_II = {"II", "TFJQAZWMHLCUIXRDYGOEVBNSKP", "STVYZACDFGHKMNQ"};
// Rotor g111_V  = {"V",  "QTPIXWVDFRMUSLJOHCANEZKYBG", "SWZFHMQ"};

// EnigmaModel abwehr_model = {.stator     = &ETW_QWERTZ,
//                             .rotors     = {&g111_I, &g111_II, &g111_V},
//                             .reflectors = {&commercial_UKW}};


// Enigma G312 ---------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

// Note: Doesn't yet support more than two notches.

// Rotor g312_I   = {"I",   "DMTWSILRUYQNKFEJCAZBPGXOHV", "SUVWZABCEFGIKLOPQ"};
// Rotor g312_II  = {"II",  "HQZGPJTMOBLNCIFDYAWVEUSRKX", "STVYZACDFGHKMNQ"};
// Rotor g312_III = {"III", "UQNTLSZFMREHDPXKIBVYGJCWOA", "UWXAEFHKMNR"};

// Rotor g312_UKW = {"UKW", "RULQMZJSYGOCETKWDAHNBXPVIF", ""};

// EnigmaModel abwehr_model = {.stator     = &ETW_QWERTZ,
//                             .rotors     = {&g312_I, &g312_II, &g312_III},
//                             .reflectors = {&g312_UKW}};


// Enigma G260 ---------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

// Note: Doesn't yet support more than two notches.

// Rotor g260_I   = {"I",   "RCSPBLKQAUMHWYTIFZVGOJNEXD", "SUVWZABCEFGIKLOPQ"};
// Rotor g260_II  = {"II",  "WCMIBVPJXAROSGNDLZKEYHUFQT", "STVYZACDFGHKMNQ"};
// Rotor g260_III = {"III", "FVDHZELSQMAXOKYIWPGCBUJTNR", "UWXAEFHKMNR"};

// EnigmaModel abwehr_model = {.stator     = &ETW_QWERTZ,
//                             .rotors     = {&g260_I, &g260_II, &g260_III},
//                             .reflectors = {&commercial_UKW}};
