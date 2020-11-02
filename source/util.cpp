#include "util.h"


std::vector<int> str_to_ordinals (std::string_view str)
{
     std::vector<int> out;
     out.reserve(str.length());

     for (char c : str)     out.push_back(to_upper(c) - 'A');

     return out;
}


std::string ordinals_to_str (std::span<const int> ordinals)
{
     std::string out;
     out.reserve(ordinals.size());

     for (int o : ordinals)     out.push_back(o + 'A');

     return out;
}


// Replace only the letters in original with the letters represented by the ordinals in pt_ordinals
std::string ordinals_to_plaintext (std::string_view original, std::span<const int> pt_ordinals)
{
     std::string out;
     out.reserve(original.length());

     int i = 0;
     for (char c : original)
     {
          if (i >= pt_ordinals.size())     out.push_back(c);

          else if (is_lower(c))     out.push_back(pt_ordinals[i++] + 'a');
          else if (is_upper(c))     out.push_back(pt_ordinals[i++] + 'A');
          else                      out.push_back(c);
     }

     return out;
}

