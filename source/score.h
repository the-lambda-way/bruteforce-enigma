#pragma once

#include <execution>
#include <iterator>
#include <numeric>       // std::transform_reduce
#include <ranges>
#include <span>
#include "qgr.h"


// Derived from Practical Cryptography scoreText.c
template <std::ranges::random_access_range R>
     requires std::same_as<std::ranges::range_value_t<R>, int> && std::ranges::sized_range<R>
double score_by_Qgram (const R& ordinals)
{
     int length = std::ranges::size(ordinals) - 3;
     auto first = std::ranges::begin(ordinals);

     int indices[length];

     for (int i = 0; i < length; ++i)
     {
          int a = first[i + 0];
          int b = first[i + 1];
          int c = first[i + 2];
          int d = first[i + 3];

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
// Returns -abs(sum(0, 25, f[i]*f[i]) - N - 0.066*N*(N-1)) for letter frequencies f and length of text N, which orders
// the IOC scores by closeness to the English expected IOC 0.066.

template <std::ranges::input_range R>
     requires std::same_as<std::ranges::range_value_t<R>, int> && std::ranges::sized_range<R>
double score_by_IOC_order (const R& ordinals)
{
     std::array<int, 26> freq = {0};
     for (int o : ordinals)     ++freq[o];

     double score = std::transform_reduce(std::execution::par_unseq, freq.begin(), freq.end(), freq.begin(), 0);

     int n = std::ranges::size(ordinals);
     score -= 0.066 * n * (n - 1) + n;
     return score < 0 ? score : -score;
}


// TODO: test

// The value sum(0, 25, f[i]*f[i]) can be algebraically simplified. The naive algorithm takes 2n - 1 additions and n
//      multiplications. This single-pass incremental solution takes 3n - 1 additions.
// The square of a number n is equal to the sum of odds from 1 to 2n - 1. For instance, the square of 5 is equal to
//      1 + 3 + 5 + 7 + 9. Thus, a square can be incrementally calculated as a number's frequency count is tabulated.
// template <std::size_t N>
// double score_by_IOC_order (std::span<const int, N> ordinals)
// {
//      std::array<int, 52> freq =
//           {0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1,
//            0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1, 0, -1};

//      for (int o : ordinals)
//      {
//          freq[2*o] += 2;                 // last value
//          freq[2*o + 1] += freq[2*o];     // accumulator
//      }

//      double score = 0.0;
//      for (int i = 1; i < 52; i += 2)     score += freq[i];

//      score -= 0.066 * N * (N - 1) + N;
//      return score < 0 ? score : -score;
// }


// template <std::size_t N>
// double score_by_IOC_order2 (std::span<const int, N> ordinals)
// {
//      std::array<std::pair<int, int>, 26> freq =
//           {{{0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1},
//             {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1},
//             {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}, {0, -1}}};

//      for (int o : ordinals)
//      {
//          freq[o].first += 2;                  // last value
//          freq[o].second += freq[o].first;     // accumulator
//      }

//      double score = std::reduce(
//           std::execution::par_unseq,
//           freq.begin(), freq.end(),
//           std::pair<int, int>{0, 0},
//           [] (const auto& p1, const auto& p2)
//           {
//                return std::pair<int, int>{0, p1.second + p2.second};
//           }).second;

//      score -= 0.066 * N * (N - 1) + N;
//      return score < 0 ? score : -score;
// }


// TODO: Test using a sparse matrix
// TODO: Test other scoring methods (including known plaintext)
// TODO: Allow for use of any qgram array.

// ---------------------------------------------------------------------------------------------------------------------
// Better than original solution
// ---------------------------------------------------------------------------------------------------------------------
// Size of ordinals doesn't change during a program, so it is more efficient to use a static size.
// template <std::size_t N>
// double score_by_IOC_order (std::span<const int, N> ordinals)
// {
//      std::array<int, 26> freq = {0};
//      for (int o : ordinals)     ++freq[o];

//      double score = std::transform_reduce(std::execution::par_unseq, freq.begin(), freq.end(), freq.begin(), 0});

//      score -= 0.066 * N * (N - 1) + N;
//      return score < 0 ? score : -score;
// }


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
