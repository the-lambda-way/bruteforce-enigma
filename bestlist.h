#pragma once

#include <cstdio>
#include <functional>    // std::greater
#include <string>
#include <queue>


template <class T, class F>
class VectorQueue : public std::priority_queue<T, std::vector<T>, F>
{
public:
    const std::vector<T>& base ()    { return this->c; }
}; // class VectorQueue


struct ScoreEntry
{
    ScoreEntry ()
    : ScoreEntry(-1e12, 0, 1, 2, 0, 0, 0, 1, 1, 1, "(Empty)")
    {}

    ScoreEntry(
        double score,
        int rotor1, int rotor2, int rotor3,
        int rotor1pos, int rotor2pos, int rotor3pos,
        int ring1pos, int ring2pos, int ring3pos,
        std::string text
    )
    : score {score},
      rotor1 {rotor1}, rotor2 {rotor2}, rotor3 {rotor3},
      rotor1pos {rotor1pos}, rotor2pos {rotor2pos}, rotor3pos {rotor3pos},
      ring1pos {ring1pos}, ring2pos {ring2pos}, ring3pos {ring3pos},
      text {text}
    {}

    double score;
    int rotor1, rotor2, rotor3;
    int rotor1pos, rotor2pos, rotor3pos;
    int ring1pos, ring2pos, ring3pos;
    std::string text;

    friend inline bool operator> (const ScoreEntry& a, const ScoreEntry& b)    { return a.score > b.score; }

    void print (int i) const
    {
        std::printf("%2i   % 8f   %s %s %s    %2d %2d %2d   %2d %2d %2d   %s\n",
                    i, score,
                    to_roman(rotor1), to_roman(rotor2), to_roman(rotor3),
                    rotor1pos, rotor2pos, rotor3pos,
                    ring1pos, ring2pos, ring3pos,
                    text.c_str());
    }


private:
    const char* roman_numerals[3] = {"I", "II", "III"};

    const char* to_roman (int num) const
    {
        return roman_numerals[num];
    }
};


template <int N>
class NBestList
{
public:
    NBestList ()
    {
        ScoreEntry default_entry {};
        for (int i = 0; i < N; ++i)    entries.push(default_entry);
    }

    // Must manually check that the score should be added first, to avoid needless copying of the settings.
    bool is_good_score (double score)    { return score > entries.top().score; }


    void add (
        double score,
        int rotor1, int rotor2, int rotor3,
        int rotor1pos, int rotor2pos, int rotor3pos,
        int ring1pos, int ring2pos, int ring3pos,
        std::string text)
    {
        for (auto entry : entries.base())
        {
            if (score == entry.score && text == entry.text)    return;
        }

        entries.emplace(score,
                        rotor1, rotor2, rotor3,
                        rotor1pos, rotor2pos, rotor3pos,
                        ring1pos, ring2pos, ring3pos,
                        text);
        entries.pop();

        // Hack to print during computation
        std::printf("% 8f   %s %s %s    %2d %2d %2d   %2d %2d %2d   %s\n",
                    score,
                    to_roman(rotor1), to_roman(rotor2), to_roman(rotor3),
                    rotor1pos, rotor2pos, rotor3pos,
                    ring1pos, ring2pos, ring3pos,
                    text.c_str());
    }


    void add (ScoreEntry e)
    {
        for (auto entry : entries.base())
        {
            if (e.score == entry.score && e.text == entry.text)    return;
        }

        entries.push(e);
        entries.pop();
    }


    void print ()
    {
        auto copy = entries;
        std::vector<ScoreEntry> ordered_entries;

        int i = 0;

        std::printf("#    Score         Rotor Ord   Rotor Pos  Ring Pos   Text  \n");

        while (!copy.empty())
        {
            ordered_entries.push_back(copy.top());
            copy.pop();
        }

        for (auto entry = ordered_entries.rbegin(); entry != ordered_entries.rend(); ++entry)
            entry->print(++i);
    }


private:
    VectorQueue<ScoreEntry, std::greater<>> entries;
    const char* roman_numerals[3] = {"I", "II", "III"};

    const char* to_roman (int num)
    {
        return roman_numerals[num];
    }

    template <int M, int SizeA, int SizeB>
    friend NBestList<M> combine_best (NBestList<SizeA> a, NBestList<SizeB> b);

    template <int M, int SizeA, int SizeB, class... List>
    friend NBestList<M> combine_best (NBestList<SizeA> a, NBestList<SizeB> b, List... rest);
}; // class NBestList




template <int N, int SizeA, int SizeB>
NBestList<N> combine_best (NBestList<SizeA> a, NBestList<SizeB> b)
{
    NBestList<N> out;

    while (!a.entries.empty())
    {
        out.add(a.entries.top());
        a.entries.pop();
    }

    while (!b.entries.empty())
    {
        out.add(b.entries.top());
        b.entries.pop();
    }

    return out;
}


template <int N, int SizeA, int SizeB, class... List>
NBestList<N> combine_best (NBestList<SizeA> a, NBestList<SizeB> b, List... rest)
{
    return combine_best<N>(combine_best<N>(a, b), rest...);
}
