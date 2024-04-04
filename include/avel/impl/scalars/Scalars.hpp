#ifndef AVEL_SCALARS_HPP
#define AVEL_SCALARS_HPP

#include <cmath>

namespace avel {

    template<class T>
    T set_bits(bool m);

}

//#include "Scalar_conversions.hpp"

#include "../Capabilities.hpp"
#include "../Constants.hpp"

#include "Scalar8u.hpp"
#include "Scalar8i.hpp"

#include "Scalar16u.hpp"
#include "Scalar16i.hpp"

#include "Scalar32u.hpp"
#include "Scalar32i.hpp"

#include "Scalar64u.hpp"
#include "Scalar64i.hpp"

#include "Scalar32f.hpp"
#include "Scalar64f.hpp"

namespace avel {

    [[nodiscard]]
    AVEL_FINL std::uint64_t div_64uhi_by_64u(std::uint64_t x, std::uint64_t y) {
        #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
        std::uint64_t quot;
        std::uint64_t rem;
        __asm__("divq %[v]" : "=a"(quot), "=d"(rem) : [v] "r"(y), "a"(0), "d"(x));
        return quot;

        #elif defined(AVEL_X86) && defined(AVEL_ICPX)
        std::uint64_t quot;
        std::uint64_t rem;
        __asm__("div %[v]" : "=a"(quot), "=d"(rem) : [v] "r"(y), "a"(0), "d"(x));
        return quot;

        #else
        // Simplified version of LibDivide's libdivide_128_div_64_to_64 
        // under assumption that numlo is 0.
        // https://github.com/ridiculousfish/libdivide/blob/master/libdivide.h
        std::uint64_t num_hi = x;
        std::uint64_t den = y;

        const uint64_t b = std::uint64_t{1} << 32;

        int shift = avel::countl_zero(den);
        den <<= shift;
        num_hi <<= shift;

        std::uint32_t den1 = std::uint32_t(den >> 32);
        std::uint32_t den0 = std::uint32_t(den & 0xFFFFFFFFu);

        std::uint64_t qhat = num_hi / den1;
        std::uint64_t rhat = num_hi % den1;

        std::uint64_t c1 = qhat * den0;
        std::uint64_t c2 = rhat * b;

        if (c1 > c2) {
            qhat -= (c1 - c2 > den) ? 2 : 1;
        }

        std::uint32_t q1 = (uint32_t)qhat;

        std::uint64_t rem = num_hi * b - q1 * den;

        qhat = rem / den1;
        rhat = rem % den1;

        c1 = qhat * den0;
        c2 = rhat * b;

        if (c1 > c2) {
            qhat -= (c1 - c2 > den) ? 2 : 1;
        }

        std::uint32_t q0 = (uint32_t)qhat;

        return (std::uint64_t(q1) << 32) | q0;

        #endif
    }

}

#endif //AVEL_SCALARS_HPP
