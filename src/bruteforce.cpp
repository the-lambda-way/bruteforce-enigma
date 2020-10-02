#include "bruteforce.h"


std::string convert_to_ct (std::string_view in)
{
     std::string s;

     for (char c : in)
     {
          if      ('a' <= c && c <= 'z')     s += (c - 32);
          else if ('A' <= c && c <= 'Z')     s += c;
     }

     return s;
}


std::vector<std::array<int, 3>> three_permutations_of_n (int n)
{
     std::vector<std::array<int, 3>> out;
     out.reserve(n * (n - 1) * (n - 2));

     int i, j, k;
     int* permutations[6][3] = {{&i, &j, &k}, {&i, &k, &j}, {&j, &i, &k}, {&j, &k, &i}, {&k, &i, &j}, {&k, &j, &i}};

     for (i = 0;     i < n - 2; ++i)
     for (j = i + 1; j < n - 1; ++j)
     for (k = j + 1; k < n;     ++k)
     for (auto p : permutations)
          // Note: emplace_back doesn't work with array types, which require brace-enclosed initialization
          out.push_back(std::move(std::array<int, 3> {*p[0], *p[1], *p[2]}));

     return out;
}


std::vector<std::array<int, 3>> triples_from_n (int n)
{
     std::vector<std::array<int, 3>> out;
     out.reserve(n * n * n);

     for (int i = 0; i < n; ++i)
     for (int j = 0; j < n; ++j)
     for (int k = 0; k < n; ++k)
          // Note: emplace_back doesn't work with array types, which require brace-enclosed initialization
          out.push_back(std::move(std::array<int, 3> {i, j, k}));

     return out;
}


std::vector<EnigmaBase> all_configurations_no_replace (const EnigmaModel& model)
{
     std::vector<EnigmaBase> out;
     std::vector<std::array<int, 3>> rotor_permutations = three_permutations_of_n(model.rotors.size());

     for (const Rotor* reflector : model.reflectors)
     for (const auto& permutation : rotor_permutations)
          out.emplace_back(*model.stator,
                           *model.rotors[permutation[0]], *model.rotors[permutation[1]], *model.rotors[permutation[2]],
                           *reflector);

     return out;
}


std::vector<EnigmaBase> all_configurations (const EnigmaModel& model)
{
     std::vector<EnigmaBase> out;
     std::vector<std::array<int, 3>> rotor_permutations = triples_from_n(model.rotors.size());

     for (const Rotor* reflector : model.reflectors)
     for (const auto& permutation : rotor_permutations)
          out.emplace_back(*model.stator,
                           *model.rotors[permutation[0]], *model.rotors[permutation[1]], *model.rotors[permutation[2]],
                           *reflector);

     return out;
}
