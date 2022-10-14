#pragma once

#include "../source/bruteforce.h"
#include "../source/models.h"


// ---------------------------------------------------------------------------------------------------------------------
// Enigma
// ---------------------------------------------------------------------------------------------------------------------
template <class KeyType> class basic_enigma {};


template <>
class basic_enigma<EnigmaKey>
{
public:
     basic_enigma (EnigmaKey key)
     : enigma {std::move(key)}
     {}

     basic_enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          const Plugboard& plugboard,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0
     )
     : basic_enigma {EnigmaKey {stator, rotor1, rotor2, rotor3, reflector, plugboard,
                                rotor1_pos, rotor2_pos, rotor3_pos, ring1_pos, ring2_pos, ring3_pos}}
     {}

     basic_enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& reflector,     // UKW
          const Plugboard& plugboard,
          const char* rotor_positions,
          const char* ring_positions
     )
     : basic_enigma {EnigmaKey {stator, rotor1, rotor2, rotor3, reflector, plugboard, rotor_positions, ring_positions}}
     {}

     Ciphertext encrypt (const Ciphertext& input)
     {
          std::vector<int> out_ordinals(input.ordinals.size());

          enigma.encrypt(input.ordinals, out_ordinals.begin());

          std::string out_text = ordinals_to_plaintext(input.text, out_ordinals);

          return {std::move(out_text), std::move(out_ordinals)};
     }

private:
     Enigma_optimize_unknown_positions<EnigmaKey> enigma;

}; // class Enigma<EnigmaKey>


template <>
class basic_enigma<Enigma4Key>
{
public:
     basic_enigma (Enigma4Key key)
     : enigma {std::move(key)}
     {}

     basic_enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& rotor4,
          const Rotor& reflector,     // UKW
          const Plugboard& plugboard,
          int rotor1_pos = 0,
          int rotor2_pos = 0,
          int rotor3_pos = 0,
          int rotor4_pos = 0,
          int ring1_pos  = 0,
          int ring2_pos  = 0,
          int ring3_pos  = 0,
          int ring4_pos  = 0
     )
     : basic_enigma {Enigma4Key {stator, rotor1, rotor2, rotor3, rotor4, reflector, plugboard,
                                 rotor1_pos, rotor2_pos, rotor3_pos, rotor4_pos,
                                 ring1_pos, ring2_pos, ring3_pos, ring4_pos}}
     {}

     basic_enigma (
          const Rotor& stator,        // ETW
          const Rotor& rotor1,
          const Rotor& rotor2,
          const Rotor& rotor3,
          const Rotor& rotor4,
          const Rotor& reflector,     // UKW
          const Plugboard& plugboard,
          const char* rotor_positions,
          const char* ring_positions
     )
     : basic_enigma {Enigma4Key {stator, rotor1, rotor2, rotor3, rotor4, reflector,
                                 plugboard, rotor_positions, ring_positions}}
     {}

     Ciphertext encrypt (const Ciphertext& input)
     {
          std::vector<int> out_ordinals(input.ordinals.size());

          enigma.encrypt(input.ordinals, out_ordinals.begin());

          std::string out_text = ordinals_to_plaintext(input.text, out_ordinals);

          return {std::move(out_text), std::move(out_ordinals)};
     }

private:
     Enigma_optimize_unknown_positions<Enigma4Key> enigma;

}; // class Enigma<Enigma4Key>


using Enigma  = basic_enigma<EnigmaKey>;
using Enigma4 = basic_enigma<Enigma4Key>;


// ---------------------------------------------------------------------------------------------------------------------
// Scoring
// ---------------------------------------------------------------------------------------------------------------------
template <int N, class KeyType> class HighScores {};


template <int N>
struct HighScores<N, EnigmaKey>
{
     Ciphertext ct;
     Leaderboard<N, EnigmaKey> high_scores;

     void print ()
     {
          std::printf("#    Score         Refl     Rotor Order         Ring Pos   Rotor Pos  Text  \n");

          int i = 0;
          for (const auto& entry : high_scores.get_entries())
          {
               const auto& key = entry.id;
               Ciphertext pt = Enigma(key).encrypt(ct);

               std::printf("%2i   % 8f   %-6s   %-5s %-5s %-5s   %2d %2d %2d   %2d %2d %2d   %.*s\n",
                           ++i, entry.score,
                           key.reflector->pretty_name.data(),
                           key.rotor1->pretty_name.data(), key.rotor2->pretty_name.data(), key.rotor3->pretty_name.data(),
                           static_cast<int>(key.ring1_pos), static_cast<int>(key.ring2_pos), static_cast<int>(key.ring3_pos),
                           static_cast<int>(key.rotor1_pos), static_cast<int>(key.rotor2_pos), static_cast<int>(key.rotor3_pos),
                           static_cast<int>(pt.text.length()), pt.text.c_str());
          }
     }

     std::deque<ScoreEntry<EnigmaKey>> get_entries () const
     {
          return high_scores.get_entries();
     }
};


template <int N>
struct HighScores<N, Enigma4Key>
{
     Ciphertext ct;
     Leaderboard<N, Enigma4Key> high_scores;

     void print ()
     {
          std::printf("#    Score         Refl     Rotor Order               Ring Pos      Rotor Pos     Text  \n");

          int i = 0;
          for (const auto& entry : high_scores.get_entries())
          {
               const auto& key = entry.id;
               Ciphertext pt = Enigma4(key).encrypt(ct);

               std::printf("%2i   % 8f   %-6s   %-5s %-5s %-5s %-5s   %2d %2d %2d %2d   %2d %2d %2d %2d   %.*s\n",
                           ++i, entry.score,
                           key.reflector->pretty_name.data(),
                           key.rotor1->pretty_name.data(), key.rotor2->pretty_name.data(),
                           key.rotor3->pretty_name.data(), key.rotor4->pretty_name.data(),
                           static_cast<int>(key.ring1_pos), static_cast<int>(key.ring2_pos),
                           static_cast<int>(key.ring3_pos), static_cast<int>(key.ring4_pos),
                           static_cast<int>(key.rotor1_pos), static_cast<int>(key.rotor2_pos),
                           static_cast<int>(key.rotor3_pos), static_cast<int>(key.rotor4_pos),
                           static_cast<int>(pt.text.length()), pt.text.c_str());
          }
     }

     std::deque<ScoreEntry<Enigma4Key>> get_entries () const
     {
          return high_scores.get_entries();
     }
};


// ---------------------------------------------------------------------------------------------------------------------
// Bruteforce decipher
// ---------------------------------------------------------------------------------------------------------------------
template <class KeyType, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, KeyType> bruteforce_decipher (
     const basic_enigma_base<KeyType>& base, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     std::puts("Breaking enigma...");

     auto bf_pos = [&] (int rotor_start, int rotor_end) {
          return bruteforce_positions<std::vector<int>, N, F>(
               base, plugboard, ct.ordinals, scoring_function, rotor_start, rotor_end
          );
     };

     auto future1 = std::async(std::launch::async, bf_pos, 0,  5);
     auto future2 = std::async(std::launch::async, bf_pos, 6,  11);
     auto future3 = std::async(std::launch::async, bf_pos, 12, 17);
     auto future4 = std::async(std::launch::async, bf_pos, 18, 26);

     return {ct, combine_leaderboards<N>(future1.get(), future2.get(), future3.get(), future4.get())};
}


template <int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, EnigmaKey> bruteforce_decipher (
     const basic_enigma_base<EnigmaKey>& base, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return bruteforce_decipher<EnigmaKey, N>(base, plugboard, ct, scoring_function);
}


template <int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, Enigma4Key> bruteforce_decipher (
     const basic_enigma_base<Enigma4Key>& base, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return bruteforce_decipher<Enigma4Key, N>(base, plugboard, ct, scoring_function);
}


template <class KeyType, std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R>, basic_enigma_base<KeyType>>
HighScores<N, KeyType> bruteforce_decipher (
     const R& bases, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     std::puts("Breaking enigma...");

     int quarter = std::ranges::size(bases) / 4 ;

     auto begin = std::ranges::begin(bases);
     auto mid1  = begin + 1 * quarter;
     auto mid2  = begin + 2 * quarter;
     auto mid3  = begin + 3 * quarter;
     auto end   = std::ranges::end(bases);

     auto bf_pos = [&] (auto first, auto last) {
          return bruteforce_positions<KeyType, decltype(std::ranges::subrange(first, last)), std::vector<int>, N, F>(
               std::ranges::subrange(first, last), plugboard, ct.ordinals, scoring_function
          );
     };

     auto future1 = std::async(std::launch::async, bf_pos, begin, mid1);
     auto future2 = std::async(std::launch::async, bf_pos, mid1,  mid2);
     auto future3 = std::async(std::launch::async, bf_pos, mid2,  mid3);
     auto future4 = std::async(std::launch::async, bf_pos, mid3,  end);

     return {ct, combine_leaderboards<N>(future1.get(), future2.get(), future3.get(), future4.get())};
}


template <std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R>, basic_enigma_base<EnigmaKey>>
HighScores<N, EnigmaKey> bruteforce_decipher (
     const R& bases, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return bruteforce_decipher<EnigmaKey, N>(bases, plugboard, ct, scoring_function);
}


template <std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R>, basic_enigma_base<Enigma4Key>>
HighScores<N, Enigma4Key> bruteforce_decipher (
     const R& bases, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return bruteforce_decipher<Enigma4Key, N>(bases, plugboard, ct, scoring_function);
}


template <class KeyType, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, KeyType> bruteforce_decipher (
     const basic_enigma_model<KeyType>& model, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return bruteforce_decipher<KeyType, std::vector<basic_enigma_base<KeyType>>, N, F>(
          all_configurations(model), plugboard, ct, scoring_function
     );
}


template <int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, EnigmaKey> bruteforce_decipher (
     const basic_enigma_model<EnigmaKey>& model, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return bruteforce_decipher<EnigmaKey, N>(model, plugboard, ct, scoring_function);
}


template <int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, Enigma4Key> bruteforce_decipher (
     const basic_enigma_model<Enigma4Key>& model, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return bruteforce_decipher<Enigma4Key, N>(model, plugboard, ct, scoring_function);
}


// ---------------------------------------------------------------------------------------------------------------------
// Smart decipher
// ---------------------------------------------------------------------------------------------------------------------
template <class KeyType, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, KeyType> smart_decipher (
     const basic_enigma_base<KeyType>& base, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     std::puts("Breaking enigma...");
     return {ct, smart_decipher_positions<KeyType, std::vector<int>, N, F>(
          base, plugboard, ct.ordinals, scoring_function
     )};
}


template <int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, EnigmaKey> smart_decipher (
     const basic_enigma_base<EnigmaKey>& base, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return smart_decipher<EnigmaKey, N>(base, plugboard, ct, scoring_function);
}


template <int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, Enigma4Key> smart_decipher (
     const basic_enigma_base<Enigma4Key>& base, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return smart_decipher<Enigma4Key, N>(base, plugboard, ct, scoring_function);
}


template <class KeyType, std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R>, basic_enigma_base<KeyType>>
HighScores<N, KeyType> smart_decipher (
     const R& bases, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     std::puts("Breaking enigma...");

     int quarter = std::ranges::size(bases) / 4 ;

     auto begin = std::ranges::begin(bases);
     auto mid1  = begin + 1 * quarter;
     auto mid2  = begin + 2 * quarter;
     auto mid3  = begin + 3 * quarter;
     auto end   = std::ranges::end(bases);

     auto smart_pos = [&] (auto first, auto last) {
          return smart_decipher_positions<KeyType, decltype(std::ranges::subrange(first, last)), std::vector<int>, N, F>(
               std::ranges::subrange(first, last), plugboard, ct.ordinals, scoring_function
          );
     };

     auto future1 = std::async(std::launch::async, smart_pos, begin, mid1);
     auto future2 = std::async(std::launch::async, smart_pos, mid1,  mid2);
     auto future3 = std::async(std::launch::async, smart_pos, mid2,  mid3);
     auto future4 = std::async(std::launch::async, smart_pos, mid3,  end);

     return {ct, combine_leaderboards<N>(future1.get(), future2.get(), future3.get(), future4.get())};
}


template <std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R>, basic_enigma_base<EnigmaKey>>
HighScores<N, EnigmaKey> smart_decipher (
     const R& bases, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return smart_decipher<EnigmaKey, N>(bases, plugboard, ct, scoring_function);
}


template <std::ranges::forward_range R, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
     requires std::same_as<std::ranges::range_value_t<R>, basic_enigma_base<Enigma4Key>>
HighScores<N, Enigma4Key> smart_decipher (
     const R& bases, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return smart_decipher<Enigma4Key, N>(bases, plugboard, ct, scoring_function);
}


template <class KeyType, int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, KeyType> smart_decipher (
     const basic_enigma_model<KeyType>& model, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return smart_decipher<KeyType, std::vector<basic_enigma_base<KeyType>>, N, F>(
          all_configurations(model), plugboard, ct, scoring_function
     );
}


template <int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, EnigmaKey> smart_decipher (
     const basic_enigma_model<EnigmaKey>& model, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return smart_decipher<EnigmaKey, N>(model, plugboard, ct, scoring_function);
}


template <int N = 10, class F = decltype(score_by_Qgram<std::vector<int>>)>
HighScores<N, Enigma4Key> smart_decipher (
     const basic_enigma_model<Enigma4Key>& model, const Plugboard& plugboard, const Ciphertext& ct,
     F scoring_function = score_by_Qgram<std::vector<int>>
)
{
     return smart_decipher<Enigma4Key, N>(model, plugboard, ct, scoring_function);
}