#include "score.h"


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
     double score = 0.0;

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
