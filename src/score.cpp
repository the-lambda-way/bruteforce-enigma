#include "score.h"
#include <execution>
#include <numeric>       // std::transform_reduce


double score_by_Qgram (std::span<const int> ordinals)
{
     int length = ordinals.size() - 3;
     int indices[length];

     for (int i = 0; i < length; ++i)
     {
          int a = ordinals[i + 0];
          int b = ordinals[i + 1];
          int c = ordinals[i + 2];
          int d = ordinals[i + 3];

          indices[i] = 17576 * a + 676 * b + 26 * c + d;
     }

     return std::transform_reduce(
          std::execution::par_unseq,
          indices, indices + length,
          0.0,
          std::plus{},
          [] (int index) -> double { return qgram[index]; }
     );
}


// Not the actual IOC, but maintains the same IOC sort order.
// Given the IOC forumula sum(0, 25, f[i]*(f[i] - 1)) / (N*(N-1)) for letter frequencies f and length of text N, returns
// -abs(sum(0, 25, f[i]*(f[i] - 1)) - 0.066*N*(N-1))
// which orders the IOC scores by closeness to the English expected IOC 0.066.

double score_by_IOC_order (std::span<const int> ordinals)
{
     int frequencies[26] = {0};

     for (int o : ordinals)     ++frequencies[o];

     double score = std::transform_reduce(
          std::execution::par_unseq,
          frequencies, frequencies + 26,
          0,
          std::plus{},
          [] (int freq) -> int { return freq * (freq - 1); }
     );

     score -= 0.066 * ordinals.size() * (ordinals.size() - 1);
     return score < 0 ? score : -score;
}


// TODO: Test using a sparse matrix
// TODO: Test other scoring methods



// ---------------------------------------------------------------------------------------------------------------------
// Original solution
// ---------------------------------------------------------------------------------------------------------------------
// 10-15% slower than current solution
// double score_by_Qgram (const int* ordinals, int length)
// {
//      double score = 0.0;

//      for (int i = 0; i < length - 3; ++i)
//      {
//           int a = ordinals[i + 0];
//           int b = ordinals[i + 1];
//           int c = ordinals[i + 2];
//           int d = ordinals[i + 3];

//           score += qgram[17576 * a + 676 * b + 26 * c + d];
//      }

//      return score;
// }


// ---------------------------------------------------------------------------------------------------------------------
// Failed attempts at optimizing, relative to original solution
// ---------------------------------------------------------------------------------------------------------------------
// Same speed
// ---------------------------------------------------------------------------------------------------------------------
// double score_by_Qgram2 (const int* ordinals, int length)
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
// #include <numeric>
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

// double score_by_Qgram3 (const int* ordinals, int length)
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


// 30% slower
// ---------------------------------------------------------------------------------------------------------------------
// #include <execution>
// #include <numeric>
// double score_by_Qgram_parallel (const int* ordinals, int length)
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
// double score_by_Qgram_parallel2 (const int* ordinals, int length)
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
