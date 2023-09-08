#ifndef AVEL_SCALAR64U_HPP
#define AVEL_SCALAR64U_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    std::uint64_t countl_one(std::uint64_t x);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::uint64_t set_bits<std::uint64_t>(bool x) {
        return -std::uint64_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t popcount(std::uint64_t x) {
        #if defined(AVEL_POPCNT)
        return _mm_popcnt_u64(x);

        #elif defined(AVEL_ARM) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
        //TODO: Perform manual optimization for this
        return __builtin_popcountl(x);

        #else
        x = x - ((x >> 1) & 0x5555555555555555);
        x = ((x >> 2) & 0x3333333333333333) + (x & 0x3333333333333333);
        x = ((x >> 4) + x) & 0x0F0F0F0F0F0F0F0F;
        x = ((x >> 8) + x) & 0x00FF00FF00FF00FF;
        x = ((x >> 16) + x) & 0x0000FFFF0000FFFF;
        x = ((x >> 32) + x) & 0x00000000FFFFFFFF;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t byteswap(std::uint64_t x) {
        #if defined(AVEL_X86)
        return _bswap64(x);

        #else
        std::uint64_t ret = 0x00;
        ret |= (x >> 56);
        ret |= (x >> 40) & (0x000000000000FF00ul);
        ret |= (x >> 24) & (0x0000000000FF0000ul);
        ret |= (x >>  8) & (0x00000000FF000000ul);
        ret |= (x <<  8) & (0x000000FF00000000ul);
        ret |= (x << 24) & (0x0000FF0000000000ul);
        ret |= (x << 40) & (0x00FF000000000000ul);
        ret |= (x << 56);
        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t countl_zero(std::uint64_t x) {
        #if defined(AVEL_LZCNT)
        return _lzcnt_u64(x);

        #elif defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
        if (x) {
            return __builtin_clzl(x);
        } else {
            return 64;
        }

        #elif defined(AVEL_ICX) && defined(AVEL_LZCNT)
        if (x) {
            return _lzcnt_u64(x);
        } else {
            return 64;
        }

        #else
        x = ~x;
        //TODO: Utilize lookup table
        std::uint64_t sum = (x == 0xFFFFFFFFFFFFFFFFul);

        bool b0 = (0xFFFFFFFF00000000u & x) == 0xFFFFFFFF00000000u;
        sum += (b0 * 32);
        x <<= (b0 * 32);

        bool b1 = (0xFFFF000000000000u & x) == 0xFFFF000000000000u;
        sum += (b1 * 16);
        x <<= (b1 * 16);

        bool b2 = (0xFF00000000000000u & x) == 0xFF00000000000000u;
        sum += (b2 * 8);
        x <<= (b2 * 8);

        bool b3 = (0xF000000000000000u & x) == 0xF000000000000000u;
        sum += (b3 * 4);
        x <<= (b3 * 4);

        bool b4 = (0xC000000000000000u & x) == 0xC000000000000000u;
        sum += (b4 * 2);
        x <<= (b4 * 2);

        bool b5 = (0x8000000000000000u & x) == 0x8000000000000000u;
        sum += (b5 * 1);

        return sum;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t countl_one(std::uint64_t x) {
        #if defined(AVEL_X86)
        return countl_zero(~x);
        #else
        //TODO: Utilize lookup table
        std::uint64_t sum = (x == 0xFFFFFFFFFFFFFFFFul);

        bool b0 = (0xFFFFFFFF00000000u & x) == 0xFFFFFFFF00000000u;
        sum += (b0 * 32);
        x <<= (b0 * 32);

        bool b1 = (0xFFFF000000000000u & x) == 0xFFFF000000000000u;
        sum += (b1 * 16);
        x <<= (b1 * 16);

        bool b2 = (0xFF00000000000000u & x) == 0xFF00000000000000u;
        sum += (b2 * 8);
        x <<= (b2 * 8);

        bool b3 = (0xF000000000000000u & x) == 0xF000000000000000u;
        sum += (b3 * 4);
        x <<= (b3 * 4);

        bool b4 = (0xC000000000000000u & x) == 0xC000000000000000u;
        sum += (b4 * 2);
        x <<= (b4 * 2);

        bool b5 = (0x8000000000000000u & x) == 0x8000000000000000u;
        sum += (b5 * 1);

        return sum;
        #endif
    }
    [[nodiscard]]
    AVEL_FINL std::uint64_t countr_zero(std::uint64_t x) {
        #if defined(AVEL_BMI)
        return __tzcnt_u64(x);

        #elif defined(AVEL_X86)
        if (x == 0) {
            return 64;
        } else {
            return __builtin_ctzll(x);
        }

        #else
        std::uint64_t ret = 0x00;
        std::uint64_t b = 0x00;

        if (x == 0x00) {
            return 64;
        }

        x &= -x;

        b = bool(x & 0xAAAAAAAAAAAAAAAAull);
        ret |= b;
        b = bool(x & 0xCCCCCCCCCCCCCCCCull);
        ret |= (b << 1);
        b = bool(x & 0xF0F0F0F0F0F0F0F0ull);
        ret |= (b << 2);
        b = bool(x & 0xFF00FF00FF00FF00ull);
        ret |= (b << 3);
        b = bool(x & 0xFFFF0000FFFF0000ull);
        ret |= (b << 4);
        b = bool(x & 0xFFFFFFFF00000000ull);
        ret |= (b << 5);

        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t countr_one(std::uint64_t x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t bit_width(std::uint64_t x) {
        return 64 - countl_zero(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t bit_floor(std::uint64_t x) {
        #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG))
        if (x == 0) {
            return 0;
        }

        return std::uint64_t{1} << (63 - __builtin_clzll(x));

        #else
        //TODO: Consider using lookup table
        if (x == 0) {
            return 0;
        }

        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        x = x | (x >> 32);
        return x - (x >> 1);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t bit_ceil(std::uint64_t x) {
        #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        auto sh = -_lzcnt_u64(x - 1);
        auto result = _lrotl(x <= 0x8000000000000000ull, sh);
        return result;

        #elif defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        if (x == 0) {
            return 1;
        }

        auto tmp = std::uint64_t{1} << (63 - __builtin_clzll(x));
        return tmp << (tmp != x);

        #else
        if (x == 0) {
            return 1;
        }

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        x |= x >> 32;
        ++x;

        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::uint64_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t rotl(std::uint64_t x, long long s) {
        #if defined(AVEL_X86)
        return _lrotl(x, s);
        #else
        s &= 0x3F;
        if (s == 0) {
            return x;
        }

        return (x << s) | (x >> (64 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t rotr(std::uint64_t x, long long s) {
        #if defined(AVEL_X86)
        return _lrotr(x, s);
        #else
        s &= 0x3F;
        if (s == 0) {
            return x;
        }

        return (x >> s) | (x << (64 - s));
        #endif
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int64_t neg_abs(std::uint64_t x) {
        std::int64_t y = x;
        if (y < 0) {
            return y;
        } else {
            return -y;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t keep(bool m, std::uint64_t x) {
        if (m) {
            return x;
        } else {
            return 0;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t clear(bool m, std::uint64_t x) {
        if (m) {
            return 0;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t blend(bool m, std::uint64_t a, std::uint64_t b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t min(std::uint64_t a, std::uint64_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t max(std::uint64_t a, std::uint64_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::uint64_t, 2> minmax(std::uint64_t a, std::uint64_t b) {
        if (a < b) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t clamp(std::uint64_t x, std::uint64_t lo, std::uint64_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t midpoint(std::uint64_t a, std::uint64_t b) {
        std::uint64_t t0 = a & b & std::uint64_t{0x1};
        std::uint64_t t1 = (a | b) & std::uint64_t{0x1} & avel::set_bits<std::uint64_t>(a > b);
        std::uint64_t t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL std::uint64_t average(std::uint64_t a, std::uint64_t b) {
        return (a >> 1) + (b >> 1) + (a & b & std::uint64_t{0x1});
    }

}

#endif //AVEL_SCALAR64U_HPP
