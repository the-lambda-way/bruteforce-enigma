#include "bestlist.h"

#include <cstdio>
#include <string>
#include "enigma.h"


bool operator> (const ScoreEntry& a, const ScoreEntry& b) const     { return a.score > b.score; }


void ScoreEntry::print (int i, std::string_view ct) const
{
     std::string pt = Enigma(config).encrypt(ct);

     std::printf("%2i   % 8f   %-4s   %-4s %-4s %-4s   %2d %2d %2d   %2d %2d %2d   %.*s\n",
                 i, score,
                 config.reflector->pretty_name.data(),
                 config.rotor1->pretty_name.data(), config.rotor2->pretty_name.data(), config.rotor3->pretty_name.data(),
                 config.ring1_pos, config.ring2_pos, config.ring3_pos,
                 config.rotor1_pos, config.rotor2_pos, config.rotor3_pos,
                 int(pt.length()), pt.c_str());
}
