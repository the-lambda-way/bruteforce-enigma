#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include "bruteforce-enigma.h"

using namespace std;


// TODO: Replace with Catch, to help with interface design.


int main (int argc, char** argv)
{
     // Practical Cryptography test 1
     {
          // Expected:
          // Model: m3
          // Rotors: IV V II   Pos: G W D   Ring: P A A
          // Note: rotors and rings are in reverse order compared to Practical Cryptography
          string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";

          string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
          string plug    = convert_to_plug("");

          Enigma enigma {ETW_ABCDEF, m3_IV, m3_V, m3_II, UKWB, plug, "GWD", "PAA"};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);


          cout << "Practical Cryptography 1\n"
               << "------------------------\n";

          cout << "Plaintext:       " << pt << "\n"
               << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n"
               << "     " << (rt == ct ? "good\n\n" : "bad\n\n");

     }


     // Practical Cryptography test 2
     {
          // Expected:
          // Model: m3
          // Rotors: I V II   Pos: S A O   Ring: B F A   Reflector: UKWB
          // Note: rotors and rings are in reverse order compared to Practical Cryptography
          string_view expected_pt = "THEENIGMACIPHERWASAFIELDCIPHERUSEDBYTHEGERMANSDURINGWORLDWARIITHEENIGMAISONEOFT"
                                    "HEBETTERKNOWNHISTORICALENCRYPTIONMACHINESANDITACTUALLYREFERSTOARANGEOFSIMILARCI"
                                    "PHERMACHINES";

          string_view ct = "YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA"
                           "JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ"
                           "VMREGWSBFZBM";
          string plug = convert_to_plug("PO ML IU KJ NH YT");

          Enigma enigma {ETW_ABCDEF, m3_I, m3_V, m3_II, UKWB, plug, "SAO", "BFA"};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);


          cout << "Practical Cryptography 2\n"
               << "------------------------\n";

          cout << "Plaintext:       " << pt << "\n"
               << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n"
               << "     " << (rt == ct ? "good\n\n" : "bad\n\n");
     }


     // CrypTool test
     {
          // Expected:
          // Model: railway
          // Rotors: II I III   Pos: 0 1 3   Ring: 3 1 2
          string_view expected_ct = "RCQIPVJHXVLDPLSONFHCEUHITPJUIGHQHZUDGSQXIEDHWUGBFLLFFLUZPCAGWRGKVZWDINYGZWQGEGNVMLFHX";
          string_view expected_pt = "IMCWNOVENGAYGHVRXVEBWLYGYHGTGAKTWQOILZSNGVNNTONILINKZTOMBYWUAXQPGAHMOSTXEISUTVAUSSIYV";


          string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
                             "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
          string ct   = convert_to_ct(text);
          string plug = convert_to_plug("BY EW FZ GI MQ RV UX");


          Enigma enigma {ETW_QWERTZ,
                         CrypTool_railway_II, CrypTool_railway_I, CrypTool_railway_III,
                         railway_UKW,
                         plug,
                         0, 1, 3,
                         3, 1, 2};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);


          cout << "Random CrypTool\n"
               << "---------------\n";

          cout << "Plaintext:       " << pt << "\n"
               << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n"
               << "     " << (rt == expected_ct ? "good\n\n" : "bad\n\n");
     }


     // CrypTool two notch test
     {
          // Expected:
          // Model: m3
          // Rotors: V I VII   Pos: 0 0 0   Ring: 0 0 0   Reflector: UKWA
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


          cout << "CrypTool 2-notch test\n"
               << "---------------------\n";

          cout << "Plaintext:       " << pt << "\n"
               << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n"
               << "     " << (rt == ct ? "good\n\n" : "bad\n\n");
     }


     // Smart decipher test
     {
          // Expected:
          // Model: m3
          // Rotors: IV V II   Pos: G W D   Ring: P A A   Reflector: UKWB
          string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
          string plug    = convert_to_plug("");

          EnigmaKey expected {ETW_ABCDEF, m3_IV, m3_V, m3_II, UKWB, plug, "GWD", "PAA"};


          cout << "smart_decipher test\n"
               << "-------------------\n";

          const auto scores = smart_4_threads<50>(m3_model, plug, ct).get_entries();

          bool solution_was_found = std::ranges::any_of(scores, [&] (const ScoreEntry& entry) {
               return entry.key == expected;
          });

          cout << "\nSolution was found: " << (solution_was_found ? "yes\n\n" : "no\n\n");
     }


     // Enigma M4 test
     {
          // Expected:
          // Model: m4
          // Rotors: I III II beta   Pos: A L Q A   Ring: A E C I   Reflector: UKWB thin
          string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";

          string ct   = convert_to_ct("ntaa phjk csol ntpa irsx pyro ohal vsum qjqc rmjz qowc gkzf zfne nvih");
          string plug = convert_to_plug("bq cr di ej kw mt os px uz gh");

          Enigma4 enigma {ETW_ABCDEF, m3_I, m3_III, m3_II, m4_beta, m4_UKWB_thin, plug, "BLQA", "BECI"};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);


          cout << "Enigma M4 test\n"
               << "--------------\n";

          cout << "Plaintext:       " << pt << "\n"
               << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n"
               << "     " << (rt == ct ? "good\n\n" : "bad\n\n");
     }

     // Enigma M4->M3 test
     {
          // M4 using beta and Bthin should be the same as M3 using UKWB. Similarly with gamma, Cthin, UKWC.
          // Rotor/ring positions must both be A for this equivalence to hold.

          cout << "Enigma M4->M3 test\n"
               << "------------------\n";

          string_view expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";
          string plug = convert_to_plug("bq cr di ej kw mt os px uz gh");


          // beta
          string ctB = convert_to_ct("odlio qsapd tbttn ueokz jbqzw hlynd mmbvn dlanx tvrny lvqmj kovre v");

          Enigma4 enigma4B {ETW_ABCDEF, m3_I, m3_III, m3_I, m4_beta, m4_UKWB_thin, plug, "BLQA", "BECA"};
          Enigma  enigmaB  {ETW_ABCDEF, m3_I, m3_III, m3_I, UKWB, plug, "BLQ", "BEC"};

          string pt4B = enigma4B.encrypt(ctB);
          string rt4B = enigma4B.encrypt(pt4B);
          string ptB  = enigmaB.encrypt(ctB);


          cout << "beta/UKWB plaintexts:  " << pt4B << "     " << ptB << "\n"
               << "     " << (pt4B == expected_pt && pt4B == ptB ? "good\n" : "bad\n");


          // gamma
          string ctC = convert_to_ct("xwwap gzbvs iqlym zmemh tyhok ixjvz pyklq auysb awxmf bijpl nnuvk m");

          Enigma4 enigma4C {ETW_ABCDEF, m3_I, m3_III, m3_I, m4_gamma, m4_UKWC_thin, plug, "BLQA", "BECA"};
          Enigma  enigmaC  {ETW_ABCDEF, m3_I, m3_III, m3_I, UKWC, plug, "BLQ", "BEC"};

          string pt4C = enigma4C.encrypt(ctC);
          string ptC  = enigmaC.encrypt(ctC);


          cout << "gamma/UKWC plaintexts: " << pt4C << "     " << ptC << "\n"
               << "     " << (pt4C == expected_pt && pt4C == ptC ? "good\n" : "bad\n");

          cout << "Reciphered text:       " << rt4B << "\n"
               << "     " << (rt4B == ctB ? "good\n\n" : "bad\n\n");
     }
}