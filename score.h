#pragma once

// Derived from Practical Cryptography scoreText.c

#include "qgr.h"


extern float qgram[];


// assumes that text consists only of uppercase letters (no punctuation or spaces)
double scoreTextQgram (const char* text, int length)
{
     char temp[4];
     double score = 0;

     for (int i = 0; i < length - 3; ++i)
     {
          temp[0] = text[i + 0] - 'A';
          temp[1] = text[i + 1] - 'A';
          temp[2] = text[i + 2] - 'A';
          temp[3] = text[i + 3] - 'A';

          score += qgram[17576 * temp[0] + 676 * temp[1] + 26 * temp[2] + temp[3]];
     }

     return score;
}


double scoreIntQgram (const int* ordinals, int length)
{
     int a, b, c, d;
     double score = 0;

     for (int i = 0; i < length - 3; ++i)
     {
          a = ordinals[i + 0];
          b = ordinals[i + 1];
          c = ordinals[i + 2];
          d = ordinals[i + 3];

          score += qgram[17576 * a + 676 * b + 26 * c + d];
     }

     return score;
}


// TODO: Test using a sparse matrix