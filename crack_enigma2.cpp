#include <iostream>
#include <future>
#include <string>
#include <string_view>
#include "bf_enigma2.h"
#include "enigma.h"
#include "enigma_models.h"
#include "stopwatch.h"

using namespace std;


NBestList<25> bf_4_threads (string_view ct, EnigmaModel model, string_view plug, int ring_end = 25)
{
     int quarter = ring_end / 4 ;

     int a = 0 * quarter;
     int b = 1 * quarter;
     int c = 2 * quarter;
     int d = 3 * quarter;
     int e = ring_end;
     int max = ring_end;

     auto future1 = async(launch::async, [&] () { return bf_decipher<15>(ct, plug, model, a,     b, max); });
     auto future2 = async(launch::async, [&] () { return bf_decipher<15>(ct, plug, model, b + 1, c, max); });
     auto future3 = async(launch::async, [&] () { return bf_decipher<15>(ct, plug, model, c + 1, d, max); });
     auto future4 = async(launch::async, [&] () { return bf_decipher<15>(ct, plug, model, d + 1, e, max); });

     return combine_best<25>(future1.get(), future2.get(), future3.get(), future4.get());
}


NBestList<25> bf_1_thread (string_view ct, string_view plug, int ring_end = 25)
{
     return bf_decipher<25>(ct, plug, CrypTool_railway_model, 0, ring_end, ring_end);
}


int main (int argc, char** argv)
{
     // string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
     //                    "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     // string ct = convert_to_ct(text);
     // string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";

     string_view ct = "YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA"
                      "JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ"
                      "VMREGWSBFZBM";
     string_view plug = "ABCDEFGNUKJMLHPOQRSYIVWXTZ";

     Stopwatch sw;
     sw.click();

     // NBestList<25> best = bf_1_thread(ct, plug, 10);
     // NBestList<25> best = bf_4_threads(ct, plug, 12);
     NBestList<25> best = bf_4_threads(ct, m3_model, plug);

     sw.click();

     best.print();
     sw.read();
}