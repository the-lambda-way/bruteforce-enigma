#pragma once

// Derived from Practical Cryptography scoreText.c

#include <execution>
#include "qgr.h"


extern float qgram[];


// assumes that text consists only of uppercase letters (no punctuation or spaces)
double scoreTextQgram (const char* text, int length)
{
     double score = 0;

     for (int i = 0; i < length - 3; ++i)
     {
          int a = text[i + 0] - 'A';
          int b = text[i + 1] - 'A';
          int c = text[i + 2] - 'A';
          int d = text[i + 3] - 'A';

          score += qgram[17576 * a + 676 * b + 26 * c + d];
     }

     return score;
}


double scoreIntQgram (const int* ordinals, int length)
{
     double score = 0;

     for (int i = 0; i < length - 3; ++i)
     {
          int a = ordinals[i + 0];
          int b = ordinals[i + 1];
          int c = ordinals[i + 2];
          int d = ordinals[i + 3];

          score += qgram[17576 * a + 676 * b + 26 * c + d];
     }

     return score;
}


// Tested 35% faster than scoreIntQgram
double scoreIntQgram_parallel (const int* ordinals, int length)
{
     int indices1[length - 3];
     int indices2[length - 3];
     int indices3[length - 3];
     int score = 0;

     std::transform(
          std::execution::par_unseq,
          ordinals, ordinals + length - 3,
          ordinals + 1,
          indices1,
          [] (int a, int b) { return 17576 * a + 676 * b; }
     );

     std::transform(
          std::execution::par_unseq,
          indices1, indices1 + length - 3,
          ordinals + 2,
          indices2,
          [] (int a, int c) { return a + 26 * c; }
     );

     std::transform(
          std::execution::par_unseq,
          indices2, indices2 + length - 3,
          ordinals + 3,
          indices3,
          [] (int a, int d) { return a + d; }
     );

     std::transform_reduce(
          std::execution::par_unseq,
          indices3, indices3 + length - 3,
          score,
          std::plus{},
          [] (int index) { return qgram[index]; }
     );

     return score;
}


// TODO: Test using a sparse matrix