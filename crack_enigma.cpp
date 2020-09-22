#include <iostream>
#include <future>
#include <string>
#include <string_view>
#include "bf_enigma.h"
#include "models.h"
#include "stopwatch.h"

using namespace std;


template <int N>
NBestList<N> bf_4_threads (EnigmaModel model, string_view plug, string_view ct, int ring_end = 25)
{
     int quarter = ring_end / 4 ;

     int a = 0 * quarter;
     int b = 1 * quarter;
     int c = 2 * quarter;
     int d = 3 * quarter;
     int e = ring_end;
     int max = ring_end;

     auto future1 = async(launch::async, [&] () { return bf_decipher<N / 2>(model, plug, ct, a,     b, max); });
     auto future2 = async(launch::async, [&] () { return bf_decipher<N / 2>(model, plug, ct, b + 1, c, max); });
     auto future3 = async(launch::async, [&] () { return bf_decipher<N / 2>(model, plug, ct, c + 1, d, max); });
     auto future4 = async(launch::async, [&] () { return bf_decipher<N / 2>(model, plug, ct, d + 1, e, max); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


template <int N>
NBestList<N> smart_4_threads (EnigmaModel model, string_view plug, string_view ct)
{
     auto future1 = async(launch::async, [&] () { return smart_decipher<N>(model, plug, ct, 0,  5); });
     auto future2 = async(launch::async, [&] () { return smart_decipher<N>(model, plug, ct, 6,  11); });
     auto future3 = async(launch::async, [&] () { return smart_decipher<N>(model, plug, ct, 12, 17); });
     auto future4 = async(launch::async, [&] () { return smart_decipher<N>(model, plug, ct, 18, 25); });

     return combine_best<N>(future1.get(), future2.get(), future3.get(), future4.get());
}


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

     // string_view ct = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
     // string_view plug = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";


     Stopwatch sw;
     sw.click();

     // NBestList<25> best = bf_decipher(m3_model, plug, ct;

     // NBestList<25> best = bf_4_threads(m3_model, plug, ct, 10);     // quick test
     // NBestList best = bf_4_threads<25>(m3_model, plug, ct);

     NBestList best = smart_4_threads<50>(m3_model, plug, ct);

     sw.click();

     best.print();
     sw.read();
}