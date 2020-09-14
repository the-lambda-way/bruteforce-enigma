#pragma once

#include <iostream>
#include <future>
#include <string>
#include <string_view>
#include "bestlist.h"
#include "EnigmaCore.h"
#include "scoreText.h"

using namespace std;


string convert_to_ct (string_view in)
{
     string s;

     for (char c : in)
     {
          if      ('a' <= c && c <= 'z')     s += (c - 32);
          else if ('A' <= c && c <= 'Z')     s += c;
     }

     return s;
}


// Is set up to make it easy to divide into 4 threads
template <int N = 10>
NBestList<N> bf_decipher (
     string_view ct, string_view plugboard,
     array<array<int, 3>, 3> rotor_combos, int ring1_start, int ring1_end, int ring_max = 26)
{
     const int length = ct.length();

     EnigmaCore core {0, 1, 2, 1, 1, 1, plugboard};
     string pt;
     double score;
     NBestList<N> best;


     // TODO: Determine which settings are equivalent, and skip


     for (auto rotors : rotor_combos)
     for (int ring1pos = ring1_start; ring1pos < ring1_end + 1; ++ring1pos)
     for (int ring2pos = 1;           ring2pos < ring_max + 1;  ++ring2pos)
     for (int ring3pos = 1;           ring3pos < ring_max + 1;  ++ring3pos)
     {
          core.reinit(rotors[0], rotors[1], rotors[2], ring1pos, ring2pos, ring3pos);

          for (int rotor1pos = 0; rotor1pos < ring_max; ++rotor1pos)
          for (int rotor2pos = 0; rotor2pos < ring_max; ++rotor2pos)
          for (int rotor3pos = 0; rotor3pos < ring_max; ++rotor3pos)
          {
               pt = core.encrypt(rotor1pos, rotor2pos, rotor3pos, ct);
               score = scoreTextQgram(pt.c_str(), length);

               if (best.is_good_score(score))
                    best.add(score,
                             rotors[0], rotors[1], rotors[2],
                             rotor1pos, rotor2pos, rotor3pos,
                             ring1pos, ring2pos, ring3pos,
                             pt);
          }
     }

     return best;
}