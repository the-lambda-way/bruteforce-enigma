#include "bruteforce.h"



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


std::vector<Enigma4Base> all_configurations (const Enigma4Model& model)
{
     std::vector<Enigma4Base> out;
     std::vector<std::array<int, 3>> rotor3_permutations = triples_from_n(model.rotors.size());

     for (const Rotor* reflector : model.reflectors)
     for (const auto& permutation : rotor3_permutations)
     for (const Rotor* rotor4 : model.rotors4)
          out.emplace_back(*model.stator,
                           *model.rotors[permutation[0]], *model.rotors[permutation[1]], *model.rotors[permutation[2]],
                           *rotor4,
                           *reflector);

     return out;
}