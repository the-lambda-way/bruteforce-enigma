#include "rotors.h"


Rotor::Rotor (std::string pretty_name,
              std::string_view str,
              std::string_view turnovers)
: pretty_name {pretty_name}
{
     for (int i = 0; i < 26; ++i)     forward[i] = str[i] - 'A';
     std::copy(forward, forward + 26, forward + 26);
     std::copy(forward, forward + 26, forward + 52);

     for (int i = 0; i < 26; ++i)     reverse[forward[i]] = i;
     std::copy(reverse, reverse + 26, reverse + 26);
     std::copy(reverse, reverse + 26, reverse + 52);

     turnoverA = turnovers.length() > 0 ? turnovers[0] - 'A' : -1;
     turnoverB = turnovers.length() > 1 ? turnovers[1] - 'A' : turnoverA;
}