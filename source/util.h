#pragma once

#include <algorithm>
#include <array>
#include <cctype>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <vector>


constexpr auto is_alpha = [] (char c) { return std::isalpha(static_cast<unsigned char>(c)); };
constexpr auto is_lower = [] (char c) { return std::islower(static_cast<unsigned char>(c)); };
constexpr auto is_upper = [] (char c) { return std::isupper(static_cast<unsigned char>(c)); };
constexpr auto is_space = [] (char c) { return std::isspace(static_cast<unsigned char>(c)); };
constexpr auto to_upper = [] (char c) { return static_cast<char>(std::toupper(static_cast<unsigned char>(c))); };


template <std::size_t N>
constexpr void str_to_ordinals (std::string_view in, std::array<int, N> out)
{
     std::ranges::transform(in, out.begin(), [] (char c) { return to_upper(c) - 'A'; });
}

std::vector<int> str_to_ordinals (std::string_view str);


std::string ordinals_to_str (std::span<const int> ordinals);


// Replace only the letters in original with the letters represented by the ordinals in pt_ordinals
std::string ordinals_to_plaintext (std::string_view original, std::span<const int> pt_ordinals);


constexpr int to_ordinal (char c)
{
     return to_upper(c) - 'A';
}


// Only accepts unique lower or upper alphabetical characters.
// Returns 0 if string is valid.
// Returns 1 if a non-alphabetical symbol is found.
// Returns 2 if a duplicate character is found.
constexpr int validate_unique_letters (std::string_view str)
{
     std::array<bool, 26> was_seen = {false, false, false, false, false, false, false, false, false, false,
                                      false, false, false, false, false, false, false, false, false, false,
                                      false, false, false, false, false, false};

     for (char c : str | std::views::transform(to_ordinal))
     {
          if (c < 0 || c > 25)     return 1;
          if (was_seen[c])         return 2;

          was_seen[c] = true;
     }

     return 0;
}
