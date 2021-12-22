#ifndef AVEL_AVEL_HPP
#define AVEL_AVEL_HPP

#include <climits>
#include <limits>

static_assert(CHAR_BIT == 8, "x86 bytes are 8 bits. Compiling for non x86 architecture");
static_assert(std::numeric_limits<float>::is_iec559, "IEEE 754 floats required");
static_assert(sizeof(float) == 4, "Size of floats should be 32 bits");
static_assert(sizeof(double) == 8, "Size of doubles should be 64 bits");

#include "impl/Capabilities.hpp"
#include "Vector.hpp"

#endif
