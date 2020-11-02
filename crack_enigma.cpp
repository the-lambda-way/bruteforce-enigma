#include <iostream>
#include <future>
#include <string>
#include <string_view>
#include "bruteforce-enigma.h"
#include "stopwatch.h"


/*
Helpful tips:

Formula for number of encryptions run on smart_decipher:
     num_of_encryptions = num_of_configurations * (26^3 + HighScores_N * 26^2)

Formula for number of encryptions run on bruteforce_decipher:
     num_of_encryptions = num_of_configurations * 26^5

For m4, use 26^4 instead of 26^3, and 26^6 instead of 26^5.


Core i7-6500U, 2.50 Ghz
Speed using Qgram scoring: 207.16 x 10^6 characters/sec
Speed using IOC scoring: 252.30 x 10^6 characters/sec
*/


int main (int argc, char** argv)
{
     // Ciphertext ct   = "NPNKANVHWKPXORCDDTRJRXSJFLCIUAIIBUNQIUQFTHLOZOIMENDNGPCB";
     // Plugboard  plug = "";

     Ciphertext ct = "YXBMXADQBDBAAYIMKDODAYIXNBDQZFJKOLFVEEQBCLUUXDFVQYGKEYBVRHONJKPJMKUNLYLZUKBKJOA"
                     "JTWVWMOMDPGVXEPUKXBVSGHROFOSBCNKEHEHAKWKOGWTBZFXSYCGSUUPPIZTRTFVCXZVCXTFLMTPTAQ"
                     "VMREGWSBFZBM";
     Plugboard plug = "PO ML IU KJ NH YT";

     Stopwatch sw;
     sw.click();

     // HighScores best = bruteforce_decipher(m3_model, plug, ct);
     HighScores best = smart_decipher(m3_model, plug, ct);

     sw.click();

     best.print();
     std::cout << "\n";
     sw.read();
}