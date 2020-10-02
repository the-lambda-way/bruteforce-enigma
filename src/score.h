#pragma once

// Derived from Practical Cryptography scoreText.c

#include "qgr.h"
#include <span>
#include <string_view>


// assumes that text consists only of uppercase letters (no punctuation or spaces)
double scoreTextQgram (std::string_view text);
double scoreIntQgram (std::span<const int> ordinals);