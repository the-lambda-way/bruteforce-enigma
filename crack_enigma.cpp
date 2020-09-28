#include <iostream>
#include <future>
#include <string>
#include <string_view>
#include "bruteforce-enigma.h"
#include "stopwatch.h"

using namespace std;


// Formula for number of encryptions run:
//   num_of_encryptions = num_of_configurations * 26^3 + BestList_N * num_threads * 26^2


// smart_4_threads time approximations (85 characters):
// m3_extended: 9.50 seconds
// railway: 0.30 seconds

int main (int argc, char** argv)
{
     // string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
     //                    "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     // string ct = convert_to_ct(text);
     // string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";

     // string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. ";
     // string ct = convert_to_ct(text);
     // string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";

     // string_view text = "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     // string ct = convert_to_ct(text);
     // string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";

     string_view ct = "YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA"
                      "JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ"
                      "VMREGWSBFZBM";
     string_view plug = "ABCDEFGNUKJMLHPOQRSYIVWXTZ";


     Stopwatch sw;
     sw.click();

     // BestList<25> best = bf_decipher(m3_model, plug, ct;
     
     // BestList<25> best = bf_4_threads(m3_model, plug, ct, 10);     // quick test
     // BestList best = bf_4_threads<25>(m3_model, plug, ct);

     BestList best = smart_4_threads<25>(m3_model, plug, ct);

     sw.click();

     best.print();

     std::cout << "\n";
     sw.read();
}