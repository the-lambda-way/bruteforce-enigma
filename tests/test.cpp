#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include "../include/bruteforce-enigma.h"

using namespace std;


int main (int argc, char** argv)
{
     // Practical Cryptography test 1
     {
          // Expected:
          // Model: m3
          // Rotors: IV V II   Pos: G W D   Ring: P A A
          // Note: rotors and rings are in reverse order compared to Practical Cryptography
          string expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";

          string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
          string_view plug = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

          Enigma enigma {ETW_ABCDEF,
                         m3_IV, m3_V, m3_II,
                         UKWB,
                         plug,
                         "GWD",
                         "PAA"};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);

          cout << "Practical Cryptography 1\n"
               << "------------------------\n";

          cout << "Plaintext:       " << pt << "\n";
          cout << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n";
          cout << "     " << (rt == ct ? "good\n\n" : "bad\n\n");

     }


     // Practical Cryptography test 2
     {
          // Expected:
          // Model: m3
          // Rotors: I V II   Pos: S A O   Ring: B F A   Reflector: UKWB
          // Note: rotors and rings are in reverse order compared to Practical Cryptography
          string expected_pt = "THEENIGMACIPHERWASAFIELDCIPHERUSEDBYTHEGERMANSDURINGWORLDWARIITHEENIGMAISONEOFT"
                               "HEBETTERKNOWNHISTORICALENCRYPTIONMACHINESANDITACTUALLYREFERSTOARANGEOFSIMILARCI"
                               "PHERMACHINES";

          string_view ct = "YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA"
                           "JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ"
                           "VMREGWSBFZBM";
          string_view plug = "ABCDEFGNUKJMLHPOQRSYIVWXTZ";

          Enigma enigma {ETW_ABCDEF,
                         m3_I, m3_V, m3_II,
                         UKWB,
                         plug,
                         "SAO",
                         "BFA"};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);

          cout << "Practical Cryptography 2\n"
               << "------------------------\n";

          cout << "Plaintext:       " << pt << "\n";
          cout << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n";
          cout << "     " << (rt == ct ? "good\n\n" : "bad\n\n");
     }


     // CrypTool test
     {
          // Expected:
          // Model: railway
          // Rotors: II I III   Pos: 0 1 3   Ring: 3 1 2
          string expected_ct = "RCQIPVJHXVLDPLSONFHCEUHITPJUIGHQHZUDGSQXIEDHWUGBFLLFFLUZPCAGWRGKVZWDINYGZWQGEGNVMLFHX";
          string expected_pt = "IMCWNOVENGAYGHVRXVEBWLYGYHGTGAKTWQOILZSNGVNNTONILINKZTOMBYWUAXQPGAHMOSTXEISUTVAUSSIYV";


          string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
                             "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
          string ct = convert_to_ct(text);
          string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";


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

          cout << "Plaintext:       " << pt << "\n";
          cout << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n";
          cout << "     " << (rt == expected_ct ? "good\n\n" : "bad\n\n");
     }


     // CrypTool two notch test
     {
          // Expected:
          // Model: m3
          // Rotors: V I VII   Pos: 0 0 0   Ring: 0 0 0   Reflector: UKWA
          string expected_pt = "SIDENOTEIGOTENIGMATOWORKWITHOTHERROTORSANDSUCHTESTEDITAGAINSTPRACTICALCRYPTOGRAPHY"
                               "IMWORKINGONTESTINGTHEBRUTEFORCEPORTIONAGAINSTAKNOWNCIPHERTHEOPTIMIZATIONSIUSEFOR"
                               "BRUTEFORCINGROTATETHEWHEELSDIFFERENTLYSOTHETESTINGISDIFFERENT";

          string_view ct   = "EQJRHLEDDYDEIZLBDIYMCDJUXBXKBATSHXDOLOQDZQFZWRHYEUFNCXDEIXKPDNQHBESVHWIZTQEPWCKHJL"
                             "GDKUWWBJIRGIINLPSDMOZIBJLCUCEJVQLPKUAQGMMKOWKPWIAJDJMXFFBNDBEXGLTVLQPZFRKTQKRGCQ"
                             "RGXGFWLBHGUYKHDYESJUKZUUCRZMAKHTCJVMIUZJLBAXMCZLFOFXEGIIHMRLH";
          string_view plug = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

          Enigma enigma {ETW_ABCDEF,
                         m3_V, m3_I, m4_VII,
                         UKWA,
                         plug,
                         0, 0, 0,
                         0, 0, 0};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);


          cout << "CrypTool 2-notch test\n"
               << "---------------------\n";

          cout << "Plaintext:       " << pt << "\n";
          cout << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n";
          cout << "     " << (rt == ct ? "good\n\n" : "bad\n\n");
     }


     // Smart decipher test
     {
          // Expected:
          // Model: m3
          // Rotors: IV V II   Pos: G W D   Ring: P A A   Reflector: UKWB
          string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
          string_view plug = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

          EnigmaConfiguration expected {ETW_ABCDEF, m3_IV, m3_V, m3_II, UKWB, plug, "GWD", "PAA"};

          cout << "smart_decipher test\n"
               << "-------------------\n";

          const auto& scores = smart_4_threads<50>(m3_model, plug, ct).get_entries();

          bool solution_was_found = std::any_of(scores.begin(), scores.end(), [&] (const ScoreEntry& entry) {
               return entry.config == expected;
          });

          cout << "\nSolution was found: " << (solution_was_found ? "yes\n\n" : "no\n\n");
     }
}