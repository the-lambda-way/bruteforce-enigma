#pragma once

// Derived from Practical Cryptography scoreText.c

#include "qgr.h"
#include <span>


double score_by_Qgram (std::span<const int> ordinals);

// Not the actual IOC, but maintains the same IOC sort order.
double score_by_IOC_order (std::span<const int> ordinals);