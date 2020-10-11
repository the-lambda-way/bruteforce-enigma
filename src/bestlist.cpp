#include "bestlist.h"

#include <cstdio>
#include <string>
#include "enigma.h"


bool operator> (const ScoreEntry& a, const ScoreEntry& b)     { return a.score > b.score; }


void ScoreEntry::print (int i, std::string_view ct) const
{
     std::string pt = Enigma(key).encrypt(ct);

     std::printf("%2i   % 8f   %-6s   %-5s %-5s %-5s   %2d %2d %2d   %2d %2d %2d   %.*s\n",
                 i, score,
                 key.reflector->pretty_name.data(),
                 key.rotor1->pretty_name.data(), key.rotor2->pretty_name.data(), key.rotor3->pretty_name.data(),
                 static_cast<int>(key.ring1_pos), static_cast<int>(key.ring2_pos), static_cast<int>(key.ring3_pos),
                 static_cast<int>(key.rotor1_pos), static_cast<int>(key.rotor2_pos), static_cast<int>(key.rotor3_pos),
                 static_cast<int>(pt.length()), pt.c_str());
}


bool operator> (const m4_ScoreEntry& a, const m4_ScoreEntry& b)     { return a.score > b.score; }


void m4_ScoreEntry::print (int i, std::string_view ct) const
{
     std::string pt = Enigma4(key).encrypt(ct);

     std::printf("%2i   % 8f   %-6s   %-5s %-5s %-5s %-5s   %2d %2d %2d %2d   %2d %2d %2d %2d   %.*s\n",
                 i, score,
                 key.reflector->pretty_name.data(),
                 key.rotor1->pretty_name.data(), key.rotor2->pretty_name.data(),
                 key.rotor3->pretty_name.data(), key.rotor4->pretty_name.data(),
                 static_cast<int>(key.ring1_pos), static_cast<int>(key.ring2_pos),
                 static_cast<int>(key.ring3_pos), static_cast<int>(key.ring4_pos),
                 static_cast<int>(key.rotor1_pos), static_cast<int>(key.rotor2_pos),
                 static_cast<int>(key.rotor3_pos), static_cast<int>(key.rotor4_pos),
                 static_cast<int>(pt.length()), pt.c_str());
}