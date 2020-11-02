#include <algorithm>
#include <string_view>
#include "bruteforce-enigma.h"
#include "catch2/catch.hpp"

using namespace std;


TEST_CASE("Practical Cryptography test 1")
{
     string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";

     Ciphertext ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
     Plugboard plug = "";

     // Note: rotors and rings are in reverse order compared to Practical Cryptography
     Enigma enigma {ETW_ABCDEF, m3_IV, m3_V, m3_II, m3_UKWB, plug, "GWD", "PAA"};

     Ciphertext pt = enigma.encrypt(ct);
     Ciphertext rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == ct);
}


TEST_CASE("Practical Cryptography test 2")
{
     string_view expected_pt = "THEENIGMACIPHERWASAFIELDCIPHERUSEDBYTHEGERMANSDURINGWORLDWARIITHEENIGMAISONEOFT"
                               "HEBETTERKNOWNHISTORICALENCRYPTIONMACHINESANDITACTUALLYREFERSTOARANGEOFSIMILARCI"
                               "PHERMACHINES";

     Ciphertext ct = "YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA"
                     "JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ"
                     "VMREGWSBFZBM";
     Plugboard plug = "PO ML IU KJ NH YT";

     // Note: rotors and rings are in reverse order compared to Practical Cryptography
     Enigma enigma {ETW_ABCDEF, m3_I, m3_V, m3_II, m3_UKWB, plug, "SAO", "BFA"};

     Ciphertext pt = enigma.encrypt(ct);
     Ciphertext rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == ct);
}


TEST_CASE("CrypTool test")
{
     string_view expected_ct = "RCQIPVJHXVLDPLSONFHCEUHITPJUIGHQHZUDGSQXIEDHWUGBFLLFFLUZPCAGWRGKVZWDINYGZWQGEGNVMLFHX";
     string_view expected_pt = "IMCWNOVENGAYGHVRXVEBWLYGYHGTGAKTWQOILZSNGVNNTONILINKZTOMBYWUAXQPGAHMOSTXEISUTVAUSSIYV";

     Ciphertext ct = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
                     "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     Plugboard plug = "BY EW FZ GI MQ RV UX";

     Enigma enigma {CrypTool_railway_ETW, CrypTool_railway_II, CrypTool_railway_I, CrypTool_railway_III, railway_UKW,
                    plug, 0, 1, 3, 3, 1, 2};

     Ciphertext pt = enigma.encrypt(ct);
     Ciphertext rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == expected_ct);
}


TEST_CASE("CrypTool two notch test")
{
     string_view expected_pt = "SIDENOTEIGOTENIGMATOWORKWITHOTHERROTORSANDSUCHTESTEDITAGAINSTPRACTICALCRYPTOGRAPHY"
                               "IMWORKINGONTESTINGTHEBRUTEFORCEPORTIONAGAINSTAKNOWNCIPHERTHEOPTIMIZATIONSIUSEFOR"
                               "BRUTEFORCINGROTATETHEWHEELSDIFFERENTLYSOTHETESTINGISDIFFERENT";

     Ciphertext ct = "EQJRHLEDDYDEIZLBDIYMCDJUXBXKBATSHXDOLOQDZQFZWRHYEUFNCXDEIXKPDNQHBESVHWIZTQEPWCKHJL"
                     "GDKUWWBJIRGIINLPSDMOZIBJLCUCEJVQLPKUAQGMMKOWKPWIAJDJMXFFBNDBEXGLTVLQPZFRKTQKRGCQ"
                     "RGXGFWLBHGUYKHDYESJUKZUUCRZMAKHTCJVMIUZJLBAXMCZLFOFXEGIIHMRLH";
     Plugboard plug = "";

     Enigma enigma {ETW_ABCDEF, m3_V, m3_I, m3_VII, m1_UKWA, plug, 0, 0, 0, 0, 0, 0};

     Ciphertext pt = enigma.encrypt(ct);
     Ciphertext rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == ct);
}


TEST_CASE("Enigma M4 test")
{
     string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";

     Ciphertext ct = "ntaa phjk csol ntpa irsx pyro ohal vsum qjqc rmjz qowc gkzf zfne nvih";
     Plugboard plug = "bq cr di ej kw mt os px uz gh";

     Enigma4 enigma {ETW_ABCDEF, m3_I, m3_III, m3_II, m4_beta, m4_UKWB_thin, plug, "BLQA", "BECI"};

     Ciphertext pt = enigma.encrypt(ct);
     Ciphertext rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == ct);
}


TEST_CASE("Enigma M4->M3 test")
{
     // M4 using beta and Bthin should be the same as M3 using UKWB. Similarly with gamma, Cthin, UKWC.
     // Rotor/ring positions must both be A for this equivalence to hold.

     string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";
     Plugboard plug = "bq cr di ej kw mt os px uz gh";


     SECTION("Beta")
     {
          Ciphertext ct = "odlio qsapd tbttn ueokz jbqzw hlynd mmbvn dlanx tvrny lvqmj kovre v";

          Enigma4 enigma4 {ETW_ABCDEF, m3_I, m3_III, m3_I, m4_beta, m4_UKWB_thin, plug, "BLQA", "BECA"};
          Enigma  enigma  {ETW_ABCDEF, m3_I, m3_III, m3_I, m3_UKWB, plug, "BLQ", "BEC"};

          Ciphertext pt4 = enigma4.encrypt(ct);
          Ciphertext rt4 = enigma4.encrypt(pt4);
          Ciphertext pt  = enigma.encrypt(ct);

          REQUIRE(pt4 == expected_pt);
          REQUIRE(pt4 == pt);
          REQUIRE(rt4 == ct);
     }


     SECTION("Gamma")
     {
          Ciphertext ct = "xwwap gzbvs iqlym zmemh tyhok ixjvz pyklq auysb awxmf bijpl nnuvk m";

          Enigma4 enigma4 {ETW_ABCDEF, m3_I, m3_III, m3_I, m4_gamma, m4_UKWC_thin, plug, "BLQA", "BECA"};
          Enigma  enigma  {ETW_ABCDEF, m3_I, m3_III, m3_I, m3_UKWC, plug, "BLQ", "BEC"};

          Ciphertext pt4 = enigma4.encrypt(ct);
          Ciphertext rt4 = enigma4.encrypt(pt4);
          Ciphertext pt  = enigma.encrypt(ct);

          REQUIRE(pt4 == expected_pt);
          REQUIRE(pt4 == pt);
          REQUIRE(rt4 == ct);
     }
}
