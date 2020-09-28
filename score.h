#pragma once

// Derived from Practical Cryptography scoreText.c

#include "qgr.h"


// assumes that text consists only of uppercase letters (no punctuation or spaces)
double scoreTextQgram (const char* text, int length);
double scoreIntQgram (const int* ordinals, int length);


// TODO: Test using a sparse matrix
// TODO: Test other scoring methods


// ---------------------------------------------------------------------------------------------------------------------
// Failed attempts at optimizing
// ---------------------------------------------------------------------------------------------------------------------

// Same speed
// ---------------------------------------------------------------------------------------------------------------------
// double scoreIntQgram3 (const int* ordinals, int length)
// {
//      double score = 0.0;
//      int a = 17576 * ordinals[0] + 676 * ordinals[1] + 26 * ordinals[2];
//      int b = 17576 * ordinals[1] + 676 * ordinals[2];
//      int c = 17576 * ordinals[2];

//      for (int i = 3; i < length; ++i)
//      {
//           score += qgram[a + ordinals[i]];

//           a = b + 26 * ordinals[i];
//           b = c + 676 * ordinals[i];
//           c = 17576 * ordinals[i];
//      }

//     return score;
// }


// Same speed
// ---------------------------------------------------------------------------------------------------------------------
// struct ScoreState
// {
//     double score;
//     int a, b, c;
// };

// ScoreState update_state (const ScoreState& state, int ordinal)
// {
//     return {state.score + qgram[state.a + ordinal],     // score
//             state.b + 26 * ordinal,                     // a
//             state.c + 676 * ordinal,                    // b
//             17576 * ordinal};                           // c
// }

// double scoreIntQgram4 (const int* ordinals, int length)
// {
//      int a = 17576 * ordinals[0] + 676 * ordinals[1] + 26 * ordinals[2];
//      int b = 17576 * ordinals[1] + 676 * ordinals[2];
//      int c = 17576 * ordinals[2];

//      return std::accumulate(
//          ordinals + 3, ordinals + length,
//          ScoreState {0.0, a, b, c},
//          update_state
//      ).score;
// }


// 10% slower
// ---------------------------------------------------------------------------------------------------------------------
// #include <execution>
// double scoreIntQgram2 (const int* ordinals, int length)
// {
//      int indices[length - 3];

//      for (int i = 0; i < length - 3; ++i)
//      {
//           int a = ordinals[i + 0];
//           int b = ordinals[i + 1];
//           int c = ordinals[i + 2];
//           int d = ordinals[i + 3];

//           indices[i] = 17576 * a + 676 * b + 26 * c + d;
//      }

//      return std::transform_reduce(
//           std::execution::par_unseq,
//           indices, indices + length - 3,
//           0.0,
//           std::plus{},
//           [] (int index) -> double { return qgram[index]; }
//      );
// }


// 30% slower
// ---------------------------------------------------------------------------------------------------------------------
// #include <execution>
// double scoreIntQgram_parallel (const int* ordinals, int length)
// {
//      int indices1[length - 3];

//      std::transform(
//           std::execution::par_unseq,
//           ordinals, ordinals + length - 3,
//           ordinals + 1,
//           indices1,
//           [] (int a, int b) { return 17576 * a + 676 * b; }
//      );


//      int indices2[length - 3];

//      std::transform(
//           std::execution::par_unseq,
//           indices1, indices1 + length - 3,
//           ordinals + 2,
//           indices2,
//           [] (int a, int c) { return a + 26 * c; }
//      );


//      int indices3[length - 3];

//      std::transform(
//           std::execution::par_unseq,
//           indices2, indices2 + length - 3,
//           ordinals + 3,
//           indices3,
//           [] (int a, int d) { return a + d; }
//      );


//      return std::transform_reduce(
//           std::execution::par_unseq,
//           indices3, indices3 + length - 3,
//           0.0,
//           std::plus{},
//           [] (int index) -> double { return qgram[index]; }
//      );
// }


// 43% slower
// ---------------------------------------------------------------------------------------------------------------------
// #include <execution>
// #include <numeric>     // std::iota
// double scoreIntQgram_parallel2 (const int* ordinals, int length)
// {
//      int indices[length - 3];
//      std::iota(indices, indices + length - 3, 0);

//      return std::transform_reduce(
//           std::execution::par_unseq,
//           indices, indices + length - 3,
//           0.0,
//           std::plus{},
//           [&] (int i) -> double {
//                int a = ordinals[i + 0];
//                int b = ordinals[i + 1];
//                int c = ordinals[i + 2];
//                int d = ordinals[i + 3];

//                return qgram[17576 * a + 676 * b + 26 * c + d];
//           }
//      );
// }
