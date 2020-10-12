#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include "bruteforce-enigma.h"
#include "catch2/catch.hpp"

using namespace std;


TEST_CASE("Practical Cryptography test 1")
{
     string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";

     string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
     string plug    = convert_to_plug("");

     // Note: rotors and rings are in reverse order compared to Practical Cryptography
     Enigma enigma {ETW_ABCDEF, m3_IV, m3_V, m3_II, UKWB, plug, "GWD", "PAA"};

     string pt = enigma.encrypt(ct);
     string rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == ct);
}


TEST_CASE("Practical Cryptography test 2")
{
     string_view expected_pt = "THEENIGMACIPHERWASAFIELDCIPHERUSEDBYTHEGERMANSDURINGWORLDWARIITHEENIGMAISONEOFT"
                               "HEBETTERKNOWNHISTORICALENCRYPTIONMACHINESANDITACTUALLYREFERSTOARANGEOFSIMILARCI"
                               "PHERMACHINES";

     string_view ct = "YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA"
                      "JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ"
                      "VMREGWSBFZBM";
     string plug = convert_to_plug("PO ML IU KJ NH YT");

     // Note: rotors and rings are in reverse order compared to Practical Cryptography
     Enigma enigma {ETW_ABCDEF, m3_I, m3_V, m3_II, UKWB, plug, "SAO", "BFA"};

     string pt = enigma.encrypt(ct);
     string rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == ct);
}


TEST_CASE("CrypTool test")
{
     string_view expected_ct = "RCQIPVJHXVLDPLSONFHCEUHITPJUIGHQHZUDGSQXIEDHWUGBFLLFFLUZPCAGWRGKVZWDINYGZWQGEGNVMLFHX";
     string_view expected_pt = "IMCWNOVENGAYGHVRXVEBWLYGYHGTGAKTWQOILZSNGVNNTONILINKZTOMBYWUAXQPGAHMOSTXEISUTVAUSSIYV";

     string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
                        "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     string ct   = convert_to_ct(text);
     string plug = convert_to_plug("BY EW FZ GI MQ RV UX");

     Enigma enigma {ETW_QWERTZ, CrypTool_railway_II, CrypTool_railway_I, CrypTool_railway_III, railway_UKW,
                    plug, 0, 1, 3, 3, 1, 2};

     string pt = enigma.encrypt(ct);
     string rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == expected_ct);
}


TEST_CASE("CrypTool two notch test")
{
     string_view expected_pt = "SIDENOTEIGOTENIGMATOWORKWITHOTHERROTORSANDSUCHTESTEDITAGAINSTPRACTICALCRYPTOGRAPHY"
                               "IMWORKINGONTESTINGTHEBRUTEFORCEPORTIONAGAINSTAKNOWNCIPHERTHEOPTIMIZATIONSIUSEFOR"
                               "BRUTEFORCINGROTATETHEWHEELSDIFFERENTLYSOTHETESTINGISDIFFERENT";

     string_view ct = "EQJRHLEDDYDEIZLBDIYMCDJUXBXKBATSHXDOLOQDZQFZWRHYEUFNCXDEIXKPDNQHBESVHWIZTQEPWCKHJL"
                      "GDKUWWBJIRGIINLPSDMOZIBJLCUCEJVQLPKUAQGMMKOWKPWIAJDJMXFFBNDBEXGLTVLQPZFRKTQKRGCQ"
                      "RGXGFWLBHGUYKHDYESJUKZUUCRZMAKHTCJVMIUZJLBAXMCZLFOFXEGIIHMRLH";
     string plug = convert_to_plug("");

     Enigma enigma {ETW_ABCDEF, m3_V, m3_I, m3_VII, UKWA, plug, 0, 0, 0, 0, 0, 0};

     string pt = enigma.encrypt(ct);
     string rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == ct);
}


TEST_CASE("Smart decipher test")
{
     string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
     string plug    = convert_to_plug("");

     EnigmaKey expected {ETW_ABCDEF, m3_IV, m3_V, m3_II, UKWB, plug, "GWD", "PAA"};

     const auto scores = smart_4_threads<50>(m3_model, plug, ct).get_entries();

     bool solution_was_found = std::ranges::any_of(scores, [&] (const ScoreEntry& entry) {
          return entry.key == expected;
     });

     REQUIRE(solution_was_found);
}


TEST_CASE("Enigma M4 test")
{
     string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";

     string ct   = convert_to_ct("ntaa phjk csol ntpa irsx pyro ohal vsum qjqc rmjz qowc gkzf zfne nvih");
     string plug = convert_to_plug("bq cr di ej kw mt os px uz gh");

     Enigma4 enigma {ETW_ABCDEF, m3_I, m3_III, m3_II, m4_beta, m4_UKWB_thin, plug, "BLQA", "BECI"};

     string pt = enigma.encrypt(ct);
     string rt = enigma.encrypt(pt);

     REQUIRE(pt == expected_pt);
     REQUIRE(rt == ct);
}


TEST_CASE("Enigma M4->M3 test")
{
     // M4 using beta and Bthin should be the same as M3 using UKWB. Similarly with gamma, Cthin, UKWC.
     // Rotor/ring positions must both be A for this equivalence to hold.

     string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";
     string plug = convert_to_plug("bq cr di ej kw mt os px uz gh");


     SECTION("Beta")
     {
          string ct = convert_to_ct("odlio qsapd tbttn ueokz jbqzw hlynd mmbvn dlanx tvrny lvqmj kovre v");

          Enigma4 enigma4 {ETW_ABCDEF, m3_I, m3_III, m3_I, m4_beta, m4_UKWB_thin, plug, "BLQA", "BECA"};
          Enigma  enigma  {ETW_ABCDEF, m3_I, m3_III, m3_I, UKWB, plug, "BLQ", "BEC"};

          string pt4 = enigma4.encrypt(ct);
          string rt4 = enigma4.encrypt(pt4);
          string pt  = enigma.encrypt(ct);

          REQUIRE(pt4 == expected_pt);
          REQUIRE(pt4 == pt);
          REQUIRE(rt4 == ct);
     }


     SECTION("Gamma")
     {
          string ct = convert_to_ct("xwwap gzbvs iqlym zmemh tyhok ixjvz pyklq auysb awxmf bijpl nnuvk m");

          Enigma4 enigma4 {ETW_ABCDEF, m3_I, m3_III, m3_I, m4_gamma, m4_UKWC_thin, plug, "BLQA", "BECA"};
          Enigma  enigma  {ETW_ABCDEF, m3_I, m3_III, m3_I, UKWC, plug, "BLQ", "BEC"};

          string pt4 = enigma4.encrypt(ct);
          string rt4 = enigma4.encrypt(pt4);
          string pt  = enigma.encrypt(ct);

          REQUIRE(pt4 == expected_pt);
          REQUIRE(pt4 == pt);
          REQUIRE(rt4 == ct);
     }
}


// TEST_CASE("M4 smart decipher test")
// {
//      string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
//      string plug    = convert_to_plug("");

//      Enigma4Key expected {ETW_ABCDEF, m3_IV, m3_V, m3_II, m4_beta, m4_UKWB_thin, plug, "GWDA", "PAAA"};

//      const auto scores = m4_smart_4_threads<50>(m4_model, plug, ct).get_entries();

//      bool solution_was_found = std::ranges::any_of(scores, [&] (const Score4Entry& entry) {
//           return entry.key == expected;
//      });

//      REQUIRE(solution_was_found);
// }

