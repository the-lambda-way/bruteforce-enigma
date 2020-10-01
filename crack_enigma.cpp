#include <iostream>
#include <future>
#include <string>
#include <string_view>
#include "bruteforce-enigma.h"
#include "stopwatch.h"

using namespace std;


// Formula for number of encryptions run on smart_decipher:
//   num_of_encryptions = num_of_configurations * 26^3 + BestList_N * num_threads * 26^2


// bf_4_threads time approximations (40 characters):
// CrypTool_m3: 58 min 32 seconds
// CrypTool_railway: 1 min 4 seconds

// smart_4_threads time approximations (85 characters):
// m3_extended: 9.50 seconds
// railway: 0.30 seconds

int main (int argc, char** argv)
{
     string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
                        "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     string ct = convert_to_ct(text);
     string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";

     // string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. ";
     // string ct = convert_to_ct(text);
     // string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";

     // string_view text = "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     // string ct = convert_to_ct(text);
     // string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";

     // string_view text = "WPEEN ZDYPG OTHVE KCLXA GNKGN AVGVF AWTMA XTWFW";
     // string ct = convert_to_ct(text);
     // string_view plug = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
     // EnigmaBase rotors {ETW_ABCDEF, m3_III, m3_II, m3_I, UKWB};


     Stopwatch sw;
     sw.click();

     // BestList<25> best = bf_decipher(m3_model, plug, ct;

     // BestList best = bf_4_threads<25>(rotors, plug, ct, 13);     // quick test
     // BestList best = bf_4_threads<25>(railway_model, plug, ct);

     BestList best = smart_4_threads<25>(CrypTool_m3_model, plug, ct);

     sw.click();

     best.print();

     std::cout << "\n";
     sw.read();
}