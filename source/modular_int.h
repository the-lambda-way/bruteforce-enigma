#pragma once

// A class that represents a set of integers modulo a number N. For speed and simplicity, limits the size.

#include <cstddef>     // std::size_t
#include <cstdint>
#include <limits>
#include <type_traits>


template <int N, typename Dummy = std::enable_if_t<N * N <= std::numeric_limits<int>::max()>>
class modular_int
{
     using self_type = modular_int<N, Dummy>;

     int value = 0;

public:
     constexpr modular_int () = default;
     constexpr modular_int (const self_type&) = default;
     constexpr modular_int (self_type&&) = default;

     constexpr modular_int& operator= (const self_type&) = default;
     constexpr modular_int& operator= (self_type&&) = default;

     explicit constexpr modular_int (int x)
     : value {[&] { x %= N; return x < 0 ? x + N : x; }()}
     {}

     constexpr operator int () const     { return value; }


     constexpr self_type& operator+= (self_type x)
     {
          return add_unsafely(x.value);
     }


     constexpr self_type& operator-= (self_type x)
     {
          return subtract_unsafely(x.value);
     }


     constexpr self_type& operator*= (self_type x)
     {
          value *= x.value;
          value %= N;
          return *this;
     }


     constexpr self_type operator+ (self_type x) const     { return self_type {*this} += x; }
     constexpr self_type operator- (self_type x) const     { return self_type {*this} -= x; }
     constexpr self_type operator* (self_type x) const     { return self_type {*this} *= x; }


     constexpr self_type& operator++ ()
     {
          if (++value == N)     value = 0;
          return *this;
     }


     constexpr self_type operator++ (int)
     {
          auto copy = value;
          ++this;
          return copy;
     }


     constexpr self_type& operator-- ()
     {
          if (--value == -1)     value = N - 1;
          return *this;
     }


     constexpr self_type operator-- (int)
     {
          auto copy = value;
          --this;
          return copy;
     }


     constexpr bool operator==  (const self_type&) const = default;
     constexpr auto operator<=> (const self_type&) const = delete;


     // Avoids conversion (which uses modular division). WARNING: If this.value + x >= 2N this breaks the invariant.
     constexpr self_type& add_unsafely (int x)
     {
          value += x;
          if (value >= N)     value -= N;
          return *this;
     }


     // Avoids conversion (which uses modular division). WARNING: If this.value - x < -N this breaks the invariant.
     constexpr self_type& subtract_unsafely (int x)
     {
          value -= x;
          if (value < 0)     value += N;
          return *this;
     }

}; // modular_int
