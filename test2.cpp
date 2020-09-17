#include <iostream>
#include <string>
#include <string_view>
#include "bf_enigma.h"
#include "enigma.h"

using namespace std;


int main (int argc, char** argv)
{
     // CrypTool test
     {
          // Expected:
          // Rotors: II I III   Pos: 0 1 3   Ring: 3 1 2
          string expected_ct = "RCQIPVJHXVLDPLSONFHCEUHITPJUIGHQHZUDGSQXIEDHWUGBFLLFFLUZPCAGWRGKVZWDINYGZWQGEGNVMLFHX";
          string expected_pt = "IMCWNOVENGAYGHVRXVEBWLYGYHGTGAKTWQOILZSNGVNNTONILINKZTOMBYWUAXQPGAHMOSTXEISUTVAUSSIYV";


          string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
                             "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
          string ct = convert_to_ct(text);
          string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";


          Enigma enigma {CrypTool_railway_ETW,
                         CrypTool_railway_II, CrypTool_railway_I, CrypTool_railway_III,
                         CrypTool_railway_UKW,
                         plug,
                         3, 1, 2,
                         0, 1, 3};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);


          cout << "Random CrypTool\n"
               << "---------------\n";

          cout << "Plaintext:       " << pt << "\n";
          cout << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n";
          cout << "     " << (rt == expected_ct ? "good\n\n" : "bad\n\n");
     }


     // Practical Cryptography test 1
     {
          // Expected:
          // Rotors: IV V II   Pos: G W D   Ring: P A A
          // Note: rotors and rings are in reverse order compared to Practical Cryptography
          string expected_pt = "INTELLIGENCEPOINTSTOATTACKONTHEEASTWALLOFTHECASTLEATDAWN";

          string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
          string_view plug = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

          Enigma enigma {m3_ETW,
                         m3_IV, m3_V, m3_II,
                         m3_UKWB,
                         plug,
                         "PAA",
                         "GWD"};

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
          // Rotors: I V II   Pos: S A O   Ring: B F A
          // Note: rotors and rings are in reverse order compared to Practical Cryptography
          string expected_pt = "THEENIGMACIPHERWASAFIELDCIPHERUSEDBYTHEGERMANSDURINGWORLDWARIITHEENIGMAISONEOFT"
                               "HEBETTERKNOWNHISTORICALENCRYPTIONMACHINESANDITACTUALLYREFERSTOARANGEOFSIMILARCI"
                               "PHERMACHINES";

          string_view ct = "YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA"
                           "JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ"
                           "VMREGWSBFZBM";
          string_view plug = "ABCDEFGNUKJMLHPOQRSYIVWXTZ";


          // Test basic operation -------------------------------------------------------------------------------------------
          Enigma enigma {m3_ETW,
                         m3_I, m3_V, m3_II,
                         m3_UKWB,
                         plug,
                         "BFA",
                         "SAO"};

          string pt = enigma.encrypt(ct);
          string rt = enigma.encrypt(pt);

          cout << "Practical Cryptography 2\n"
               << "------------------------\n";

          cout << "Plaintext:       " << pt << "\n";
          cout << "     " << (pt == expected_pt ? "good\n" : "bad\n");

          cout << "Reciphered text: " << rt << "\n";
          cout << "     " << (rt == ct ? "good\n\n" : "bad\n\n");
     }
}