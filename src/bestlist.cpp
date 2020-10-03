#include "bestlist.h"

#include <cstdio>
#include <string>
#include "enigma.h"


bool operator> (const ScoreEntry& a, const ScoreEntry& b)     { return a.score > b.score; }


void ScoreEntry::print (int i, std::string_view ct) const
{
     std::string pt = Enigma(key).encrypt(ct);

     std::printf("%2i   % 8f   %-4s   %-4s %-4s %-4s   %2d %2d %2d   %2d %2d %2d   %.*s\n",
                 i, score,
                 key.reflector->pretty_name.data(),
                 key.rotor1->pretty_name.data(), key.rotor2->pretty_name.data(), key.rotor3->pretty_name.data(),
                 key.ring1_pos, key.ring2_pos, key.ring3_pos,
                 key.rotor1_pos, key.rotor2_pos, key.rotor3_pos,
                 int(pt.length()), pt.c_str());
}
