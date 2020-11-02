#include "datatypes.h"
#include "models.h"

// Useful: https://en.wikipedia.org/wiki/Enigma_rotor_details

// NOTE: Rotors could be constexpr. However, to support shallow comparison of Rotor pointers, they should be left const.


// Common --------------------------------------------------------------------------------------------------------------
const Rotor ETW_ABCDEF = {"ABCDEF", "ABCDEFGHIJKLMNOPQRSTUVWXYZ", ""};
const Rotor ETW_QWERTZ = {"QWERTZ", "JWULCMNOHPQZYXIRADKEGVBTSF", ""};


// M3 ------------------------------------------------------------------------------------------------------------------
// https://www.cryptomuseum.com/crypto/enigma/m3/index.htm

const Rotor m3_I    = {"I",    "EKMFLGDQVZNTOWYHXUSPAIBRCJ", "Q"};
const Rotor m3_II   = {"II",   "AJDKSIRUXBLHWTMCQGZNPYFVOE", "E"};
const Rotor m3_III  = {"III",  "BDFHJLCPRTXVZNYEIWGAKMUSQO", "V"};
const Rotor m3_IV   = {"IV",   "ESOVPZJAYQUIRHXLNFTGKDCMWB", "J"};
const Rotor m3_V    = {"V",    "VZBRGITYUPSDNHLXAWMJQOFECK", "Z"};
const Rotor m3_VI   = {"VI",   "JPGVOUMFYQBENHZRDKASXLICTW", "ZM"};
const Rotor m3_VII  = {"VII",  "NZJHGRCXMYSWBOUFAIVLPEKQDT", "ZM"};
const Rotor m3_VIII = {"VIII", "FKQHTLXOCBJSPDZRAMEWNIUYGV", "ZM"};

const Rotor m3_UKWB = {"UKWB", "YRUHQSLDPXNGOKMIEBFZCWVJAT", ""};
const Rotor m3_UKWC = {"UKWC", "FVPJIAOYEDRZXWGCTKUQSBNMHL", ""};

// Combinations: 250
const EnigmaModel m3_model = {
     .stator     = &ETW_ABCDEF,
     .rotors     = {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V},
     .reflectors = {&m3_UKWB, &m3_UKWC}
};

// Combinations: 1024
const EnigmaModel kriegsmarine_model = {
     .stator     = &ETW_ABCDEF,
     .rotors     = {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V, &m3_VI, &m3_VII, &m3_VIII},
     .reflectors = {&m3_UKWB, &m3_UKWC}
};


// M4 ------------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor m4_beta      = {"beta",  "LEYJVCNIXWPBQMDRTAKZGFUHOS", ""};
const Rotor m4_gamma     = {"gamma", "FSOKANUERHMBTIYCWLQPZXVGJD", ""};
const Rotor m4_UKWB_thin = {"Bthin", "ENKQAUYWJICOPBLMDXZVFTHRGS", ""};
const Rotor m4_UKWC_thin = {"Cthin", "RDOBJNTKVEHMLFCWZAXGYIPSUQ", ""};

// Combinations: 2048
const Enigma4Model m4_model = {
     .stator     = &ETW_ABCDEF,
     .rotors     = {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V, &m3_VI, &m3_VII, &m3_VIII},
     .rotors4    = {&m4_beta, &m4_gamma},
     .reflectors = {&m4_UKWB_thin, &m4_UKWC_thin}
};


// Enigma I ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor m1_UKWA = {"UKWA", "EJMZALYXVBWFCRQUONTSPIKHGD", ""};

// Combinations: 375
const EnigmaModel enigma_I_model = {
     .stator     = &ETW_ABCDEF,
     .rotors     = {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V},
     .reflectors = {&m1_UKWA, &m3_UKWB, &m3_UKWC}
};


// Enigma I Norenigma --------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor norenigma_I   = {"I",   "WTOKASUYVRBXJHQCPZEFMDINLG", "Q"};
const Rotor norenigma_II  = {"II",  "GJLPUBSWEMCTQVHXAOFZDRKYNI", "E"};
const Rotor norenigma_III = {"III", "JWFMHNBPUSDYTIXVZGRQLAOEKC", "V"};
const Rotor norenigma_IV  = {"IV",  "ESOVPZJAYQUIRHXLNFTGKDCMWB", "J"};
const Rotor norenigma_V   = {"V",   "HEJXQOTZBVFDASCILWPGYNMURK", "Z"};

const Rotor norenigma_UKW = {"UKW", "MOWJYPUXNDSRAIBFVLKZGQCHET", ""};

// Combinations: 125
const EnigmaModel norenigma_model = {
     .stator     = &ETW_ABCDEF,
     .rotors     = {&norenigma_I, &norenigma_II, &norenigma_III, &norenigma_IV, &norenigma_V},
     .reflectors = {&norenigma_UKW}
};


// Enigma I Sondermaschine ---------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js
const Rotor sondermaschine_I   = {"I",   "VEOSIRZUJDQCKGWYPNXAFLTHMB", "Q"};
const Rotor sondermaschine_II  = {"II",  "UEMOATQLSHPKCYFWJZBGVXINDR", "E"};
const Rotor sondermaschine_III = {"III", "TZHXMBSIPNURJFDKEQVCWGLAOY", "V"};

const Rotor sondermaschine_UKW = {"UKW", "CIAGSNDRBYTPZFULVHEKOQXWJM", ""};

// Combinations: 27
const EnigmaModel sondermaschine_model = {
     .stator     = &ETW_ABCDEF,
     .rotors     = {&sondermaschine_I, &sondermaschine_II, &sondermaschine_III},
     .reflectors = {&sondermaschine_UKW}
};


// Railway -------------------------------------------------------------------------------------------------------------
// https://www.cryptomuseum.com/crypto/enigma/k/railway.htm

const Rotor railway_I   = {"I",   "JGDQOXUSCAMIFRVTPNEWKBLZYH", "N"};
const Rotor railway_II  = {"II",  "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "E"};
const Rotor railway_III = {"III", "JVIUBHTCDYAKEQZPOSGXNRMWFL", "Y"};

const Rotor railway_UKW = {"UKW", "QYHOGNECVPUZTFDJAXWMKISRBL", ""};

// Combinations: 27
const EnigmaModel railway_model = {
     .stator     = &ETW_QWERTZ,
     .rotors     = {&railway_I, &railway_II, &railway_III},
     .reflectors = {&railway_UKW}
};


// CrypTool railway ----------------------------------------------------------------------------------------------------
// Portal to source code: https://www.cryptool.org/en/ct2-documentation

// CrypTool turns over on the notch position on the wheel, when it should turnover on the indicator position.

const Rotor CrypTool_railway_ETW = {"QWERTZ", "QWERTZUIOASDFGHJKPYXCVBNML", ""};

const Rotor CrypTool_railway_I   = {"I",   "JGDQOXUSCAMIFRVTPNEWKBLZYH", "V"};
const Rotor CrypTool_railway_II  = {"II",  "NTZPSFBOKMWRCJDIVLAEYUXHGQ", "M"};
const Rotor CrypTool_railway_III = {"III", "JVIUBHTCDYAKEQZPOSGXNRMWFL", "G"};

// Combinations: 27
const EnigmaModel CrypTool_railway_model = {
     .stator     = &CrypTool_railway_ETW,
     .rotors     = {&CrypTool_railway_I, &CrypTool_railway_II, &CrypTool_railway_III},
     .reflectors = {&railway_UKW}
};


// CrypTool M3 ---------------------------------------------------------------------------------------------------------
// Portal to source code: https://www.cryptool.org/en/ct2-documentation

// CrypTool includes UKWA in its m3 model.

// Combinations: 2048
const EnigmaModel CrypTool_m3_model = {
     .stator     = &ETW_ABCDEF,
     .rotors     = {&m3_I, &m3_II, &m3_III, &m3_IV, &m3_V, &m3_VI, &m3_VII, &m3_VIII},
     .reflectors = {&m1_UKWA, &m3_UKWB, &m3_UKWC}
};


// Enigma D ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor commercial_I   = {"I",   "LPGSZMHAEOQKVXRFYBUTNICJDW", "Y"};
const Rotor commercial_II  = {"II",  "SLVGBTFXJQOHEWIRZYAMKPCNDU", "E"};
const Rotor commercial_III = {"III", "CJGDPSHKTURAWZXFMYNQOBVLIE", "N"};

const Rotor commercial_UKW = {"comm", "IMETCGFRAYSQBZXWLHKDVUPOJN", ""};

// Combinations: 27
const EnigmaModel commercial_model = {
     .stator     = &ETW_QWERTZ,
     .rotors     = {&commercial_I, &commercial_II, &commercial_III},
     .reflectors = {&commercial_UKW}
};


// Enigma T ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor tirpitz_ETW  = {"ETW", "ILXRZTKGJYAMWVDUFCPQEONSHB", ""};

const Rotor tirpitz_I    = {"I",    "KPTYUELOCVGRFQDANJMBSWHZXI", "WZEKQ"};
const Rotor tirpitz_II   = {"II",   "UPHZLWEQMTDJXCAKSOIGVBYFNR", "WZFLR"};
const Rotor tirpitz_III  = {"III",  "QUDLYRFEKONVZAXWHMGPJBSICT", "WZEKQ"};
const Rotor tirpitz_IV   = {"IV",   "CIWTBKXNRESPFLYDAGVHQUOJZM", "WZFLR"};
const Rotor tirpitz_V    = {"V",    "UAXGISNJBVERDYLFZWTPCKOHMQ", "YCFKR"};
const Rotor tirpitz_VI   = {"VI",   "XFUZGALVHCNYSEWQTDMRBKPIOJ", "XEIMQ"};
const Rotor tirpitz_VII  = {"VII",  "BJVFTXPLNAYOZIKWGDQERUCHSM", "YCFKR"};
const Rotor tirpitz_VIII = {"VIII", "YMTPNZHWKODAJXELUQVGCBISFR", "XEIMQ"};

const Rotor tirpitz_UKW  = {"UKW", "GEKPBTAUMOCNILJDXZYFHWVQSR", ""};

// Combinations: 343
const EnigmaModel tirpitz_model = {
    .stator     = &tirpitz_ETW,
    .rotors     = {&tirpitz_I, &tirpitz_II, &tirpitz_III, &tirpitz_IV, &tirpitz_V, &tirpitz_VII, &tirpitz_VIII},
    .reflectors = {&tirpitz_UKW}
};


// Enigma K ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

// NOTE: Simulator does not simulate Swiss K. Rotors included here for use with the standard Enigma design.

const Rotor swissK_I   = {"I",   "PEZUOHXSCVFMTBGLRINQJWAYDK", "Y"};
const Rotor swissK_II  = {"II",  "ZOUESYDKFWPCIQXHMVBLGNJRAT", "E"};
const Rotor swissK_III = {"III", "EHRVXGAOBQUSIMZFLYNWKTPDJC", "N"};

// Combinations: 27
const EnigmaModel swissK_model = {
    .stator     = &ETW_QWERTZ,
    .rotors     = {&swissK_I, &swissK_II, &swissK_III},
    .reflectors = {&commercial_UKW}
};


// Enigma KD -----------------------------------------------------------------------------------------------------------
// https://www.cryptomuseum.com/crypto/enigma/wiring.htm

// NOTE: The actual Enigma KD uses a rewireable UKW, which this simulator does not support.

const Rotor KD_I   = {"I",   "VEZIOJCXKYDUNTWAPLQGBHSFMR", "SUYAEHLNQ"};
const Rotor KD_II  = {"II",  "HGRBSJZETDLVPMQYCXAOKINFUW", "SUYAEHLNQ"};
const Rotor KD_III = {"III", "NWLHXGRBYOJSAZDVTPKFQMEUIC", "SUYAEHLNQ"};
const Rotor KD_UKW = {"UKW", "NSUOMKLIHZFGEADVXWBYCPRQTJ", ""};

// Combinations: 27
const EnigmaModel KD_model = {
    .stator     = &ETW_QWERTZ,
    .rotors     = {&KD_I, &KD_II, &KD_III},
    .reflectors = {&commercial_UKW}
};


// Enigma Z ------------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor zahlwerk_I   = {"I",   "LPGSZMHAEOQKVXRFYBUTNICJDW", "SUVWZABCEFGIKLOPQ"};
const Rotor zahlwerk_II  = {"II",  "SLVGBTFXJQOHEWIRZYAMKPCNDU", "STVYZACDFGHKMNQ"};
const Rotor zahlwerk_III = {"III", "CJGDPSHKTURAWZXFMYNQOBVLIE", "UWXAEFHKMNR"};

// Combinations: 27
const EnigmaModel zahlwerk_model = {
    .stator     = &ETW_QWERTZ,
    .rotors     = {&zahlwerk_I, &zahlwerk_II, &zahlwerk_III},
    .reflectors = {&commercial_UKW}
};


// Enigma G111 ---------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor g111_I  = {"I",  "WLRHBQUNDKJCZSEXOTMAGYFPVI", "SUVWZABCEFGIKLOPQ"};
const Rotor g111_II = {"II", "TFJQAZWMHLCUIXRDYGOEVBNSKP", "STVYZACDFGHKMNQ"};
const Rotor g111_V  = {"V",  "QTPIXWVDFRMUSLJOHCANEZKYBG", "SWZFHMQ"};

// Combinations: 27
const EnigmaModel g111_model = {
    .stator     = &ETW_QWERTZ,
    .rotors     = {&g111_I, &g111_II, &g111_V},
    .reflectors = {&commercial_UKW}
};


// Enigma G312 ---------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor g312_I   = {"I",   "DMTWSILRUYQNKFEJCAZBPGXOHV", "SUVWZABCEFGIKLOPQ"};
const Rotor g312_II  = {"II",  "HQZGPJTMOBLNCIFDYAWVEUSRKX", "STVYZACDFGHKMNQ"};
const Rotor g312_III = {"III", "UQNTLSZFMREHDPXKIBVYGJCWOA", "UWXAEFHKMNR"};

const Rotor g312_UKW = {"UKW", "RULQMZJSYGOCETKWDAHNBXPVIF", ""};

// Combinations: 27
const EnigmaModel g312_model = {
    .stator     = &ETW_QWERTZ,
    .rotors     = {&g312_I, &g312_II, &g312_III},
    .reflectors = {&g312_UKW}
};


// Enigma G260 ---------------------------------------------------------------------------------------------------------
// https://github.com/cryptii/cryptii/blob/master/src/Encoder/Enigma.js

const Rotor g260_I   = {"I",   "RCSPBLKQAUMHWYTIFZVGOJNEXD", "SUVWZABCEFGIKLOPQ"};
const Rotor g260_II  = {"II",  "WCMIBVPJXAROSGNDLZKEYHUFQT", "STVYZACDFGHKMNQ"};
const Rotor g260_III = {"III", "FVDHZELSQMAXOKYIWPGCBUJTNR", "UWXAEFHKMNR"};

// Combinations: 27
const EnigmaModel g260_model = {
    .stator     = &ETW_QWERTZ,
    .rotors     = {&g260_I, &g260_II, &g260_III},
    .reflectors = {&commercial_UKW}
};
