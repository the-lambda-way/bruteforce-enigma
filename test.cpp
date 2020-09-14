#include <iostream>
#include <string>
#include <string_view>
#include "bf_enigma.h"
#include "EnigmaCore.h"

using namespace std;


// Expected:
// Rotors: II I III   Pos: 0 1 3   Ring: 4 2 3
string expected_ct = "RCQIPVJHXVLDPLSONFHCEUHITPJUIGHQHZUDGSQXIEDHWUGBFLLFFLUZPCAGWRGKVZWDINYGZWQGEGNVMLFHX";
string expected_pt = "IMCWNOVENGAYGHVRXVEBWLYGYHGTGAKTWQOILZSNGVNNTONILINKZTOMBYWUAXQPGAHMOSTXEISUTVAUSSIYV";


int main (int argc, char** argv)
{
     string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
                        "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";
     string ct = convert_to_ct(text);


     // Test basic operation --------------------------------------------------------------------------------------------
     EnigmaCore core {1, 0, 2, 4, 2, 3, plug};
     string pt = core.encrypt(0, 1, 3, ct);
     string dt = core.encrypt(0, 1, 3, pt);

     cout << "Plaintext:       " << pt << "\n";
     cout << "     " << (pt == expected_pt ? "good\n" : "bad\n");

     cout << "Deciphered text: " << dt << "\n";
     cout << "     " << (ct == expected_ct ? "good\n\n" : "bad\n\n");
}