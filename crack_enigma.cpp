#include <iostream>
#include <future>
#include <string>
#include <string_view>
#include "bf_enigma.h"
#include "stopwatch.h"

using namespace std;


NBestList<25> bf_4_threads (std::string_view ct, std::string_view plug, int ring_end = 26)
{
     const array<array<int, 3>, 3> rotorsA {{{0, 1, 2}, {0, 2, 1}, {1, 0, 2}}};
     const array<array<int, 3>, 3> rotorsB {{{1, 2, 0}, {2, 0, 1}, {2, 1, 0}}};

     const int mid = ring_end / 2;
     const int end = ring_end;
     const int max = ring_end;

     auto future1 = async(launch::async, [&] () { return bf_decipher<15>(ct, plug, rotorsA, 1,       mid, max); });
     auto future2 = async(launch::async, [&] () { return bf_decipher<15>(ct, plug, rotorsA, mid + 1, end, max); });
     auto future3 = async(launch::async, [&] () { return bf_decipher<15>(ct, plug, rotorsB, 1,       mid, max); });
     auto future4 = async(launch::async, [&] () { return bf_decipher<15>(ct, plug, rotorsB, mid + 1, end, max); });

     return combine_best<25>(future1.get(), future2.get(), future3.get(), future4.get());
}


NBestList<25> bf_1_thread (std::string_view ct, std::string_view plug, int ring_end = 26)
{
     const array<array<int, 3>, 3> rotorsA {{{0, 1, 2}, {0, 2, 1}, {1, 0, 2}}};
     const array<array<int, 3>, 3> rotorsB {{{1, 2, 0}, {2, 0, 1}, {2, 1, 0}}};

     const int mid = ring_end / 2;
     const int end = ring_end;
     const int max = ring_end;

     auto result1 = bf_decipher<15>(ct, plug, rotorsA, 1,       mid, max);
     auto result2 = bf_decipher<15>(ct, plug, rotorsB, mid + 1, end, max);

     return combine_best<25>(result1, result2);
}


int main (int argc, char** argv)
{
     string_view text = "Rc qipv jhx vld plson fhceuh itp jui gh qhzu dg sq xie dhw. "
                        "U gbfl lf fluz pcag wrgkv zw, dinyg zw, qge gnvm L fhx.";
     string_view plug = "AYCDWZIHGJKLQNOPMVSTXREUBF";
     string ct = convert_to_ct(text);

     Stopwatch sw;
     sw.click();

     // NBestList<25> best = bf_1_thread(ct, plug, 10);
     NBestList<25> best = bf_4_threads(ct, plug, 10);
     // NBestList<25> best = bf_4_threads(ct, plug);

     sw.click();

     best.print();
     sw.read();
}