#ifndef AVEL_SCALAR32U_HPP
#define AVEL_SCALAR32U_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    std::uint32_t countl_one(std::uint32_t x);

    std::uint32_t min(std::uint32_t a, std::uint32_t b);

    std::uint32_t max(std::uint32_t a, std::uint32_t b);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::uint32_t set_bits<std::uint32_t>(bool x) {
        return -std::uint32_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t popcount(std::uint32_t x) {
        #if defined(AVEL_POPCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        return _popcnt32(x);

        #elif defined(AVEL_POPCNT) && defined(AVEL_MSVC)
        return __popcnt(x);

        #else
        //TODO: Consider using per nibble lookup table as alternative
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & 0x55555555u);
        x = (x & 0x33333333u) + ((x >> 2) & 0x33333333u);
        x = ((x + (x >> 4) & 0x0F0F0F0Fu) * 0x01010101u) >> 24;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t byteswap(std::uint32_t x) {
        #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        return _bswap(x);

        #elif defined(AVEL_MSVC)
        return _byteswap_ulong(x);

        #else
        std::uint32_t ret = 0x00;
        ret |= x >> 24;
        ret |= (x >> 8) & (0x0000FF00);
        ret |= (x << 8) & (0x00FF0000);
        ret |= x << 24;
        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t countl_zero(std::uint32_t x) {
        #if defined(AVEL_LZCNT) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        return _lzcnt_u32(x);

        #elif defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        if (x) {
            return 31 - _bit_scan_reverse(x);
        } else {
            return 32;
        }

        #elif defined(AVEL_MSVC)
        unsigned long bsr;
        if (_BitScanReverse(&bsr, x)) {
            return 31 - bsr;
        } else {
            return 32;
        }

        #else
        return countl_one(~x);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t countl_one(std::uint32_t x) {
        #if defined(AVEL_X86)
        return countl_zero(~x);

        #else
        //TODO: Consider using lookup table

        // Divide and conquer implementation
        std::uint32_t sum = (x == 0xFFFFFFFF);

        bool b0 = (0xFFFF0000u & x) == 0xFFFF0000u;
        sum += (b0 * 16);
        x <<= (b0 * 16);

        bool b1 = (0xFF000000u & x) == 0xFF000000u;
        sum += (b1 * 8);
        x <<= (b1 * 8);

        bool b2 = (0xF0000000u & x) == 0xF0000000u;
        sum += (b2 * 4);
        x <<= (b2 * 4);

        bool b3 = (0xC0000000u & x) == 0xC0000000u;
        sum += (b3 * 2);
        x <<= (b3 * 2);

        bool b4 = (0x80000000u & x) == 0x80000000u;
        sum += (b4 * 1);

        return sum;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t countr_zero(std::uint32_t x) {
        #if defined(AVEL_BMI) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        return __tzcnt_u32(x);

        #elif defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        if (x) {
            return _bit_scan_forward(x);
        } else {
            return 32;
        }

        #elif defined(AVEL_MSVC)
        unsigned long bsr;
        if (_BitScanForward(&bsr, x)) {
            return bsr;
        } else {
            return 32;
        }

        #else
        //TODO: Consider using lookup table
        //Divide and conquer implementation
        std::uint32_t ret = 0x00;

        if (x == 0x00) {
            return 32;
        }

        x &= -x;

        std::uint32_t b = 0x00;
        b = bool(x & 0xAAAAAAAAu);
        ret |= b;
        b = bool(x & 0xCCCCCCCCu);
        ret |= (b << 1);
        b = bool(x & 0xF0F0F0F0u);
        ret |= (b << 2);
        b = bool(x & 0xFF00FF00u);
        ret |= (b << 3);
        b = bool(x & 0xFFFF0000u);
        ret |= (b << 4);

        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t countr_one(std::uint32_t x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t bit_width(std::uint32_t x) {
        #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        if (x == 0) {
            return 0;
        } else {
            return _bit_scan_reverse(x) + 1;
        }

        #elif defined(AVEL_MSVC)
        unsigned long bsr;
        if (_BitScanReverse(&bsr, x)) {
            return bsr + 1;
        } else {
            return 0;
        }

        #else
        //TODO: Consider using lookup table
        if (x == 0) {
            return 0;
        }

        std::uint32_t ret = 0x00;

        bool b0 = x & 0xFFFF0000;
        ret += b0 * 16;
        x >>= (b0 * 16);

        bool b1 = x & 0xFFFFFF00;
        ret += b1 * 8;
        x >>= (b1 * 8);

        bool b2 = x & 0xFFFFFFF0;
        ret += b2 * 4;
        x >>= (b2 * 4);

        bool b3 = x & 0xFFFFFFFC;
        ret += b3 * 2;
        x >>= (b3 * 2);

        bool b4 = x & 0xFFFFFFFE;
        ret += b4 * 1;

        return ret + 1;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t bit_floor(std::uint32_t x) {
        #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        if (x == 0) {
            return 0;
        }

        return 1 << _bit_scan_reverse(x);

        #elif defined(AVEL_MSVC)
        unsigned long bsr;
        if (_BitScanReverse(&bsr, x)) {
            return 1 << bsr;
        } else {
            return 0;
        }

        #else
        //TODO: Consider using lookup table
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x | (x >> 16);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t bit_ceil(std::uint32_t x) {
<<<<<<< HEAD
        #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
=======
        #if defined(AVEL_LZCNT)
        auto sh = (64 - _lzcnt_u64(std::uint64_t(x) - 1));
        auto result = std::uint64_t(1) << sh;
        return result;

        #elif defined(AVEL_X86)
>>>>>>> 1a886e614b5b765aae27285d2ed6ebd61ec1842c
        if (x == 0) {
            return 1;
        }

        auto tmp = 1 << _bit_scan_reverse(x);
        return tmp << (tmp != x);

        #elif defined(AVEL_MSVC)
        unsigned long bsr;
        if (_BitScanReverse(&bsr, x)) {
            auto tmp = 1 << bsr;
            return tmp << (tmp != x);
        } else {
            return 1;
        }

        #else
        //TODO: Consider using lookup table
        if (x == 0) {
            return 1;
        }

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        ++x;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::uint32_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t rotl(std::uint32_t x, long long s) {
        #if defined(AVEL_X86) && (defined(AVEL_GCC) || defined(AVEL_CLANG) || defined(AVEL_ICPX))
        return _rotl(x, s);

        #elif defined(AVEL_X86) && defined(AVEL_MSVC)
        return _rotl(x, s);

        #else
        s &= 0x1F;
        if (s == 0) {
            return x;
        }

        return (x << s) | (x >> (32 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t rotr(std::uint32_t x, long long s) {
        #if defined(AVEL_X86)
        return _rotr(x, s);

        #else
        s &= 0x1F;
        if (s == 0) {
            return x;
        }

        return (x >> s) | (x << (32 - s));
        #endif
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int32_t neg_abs(std::uint32_t x) {
        std::int32_t y = x;
        if (y < 0) {
            return y;
        } else {
            return -y;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t keep(bool m, std::uint32_t x) {
        if (m) {
            return x;
        } else {
            return 0;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t clear(bool m, std::uint32_t x) {
        if (m) {
            return 0;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t blend(bool m, std::uint32_t a, std::uint32_t b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t min(std::uint32_t a, std::uint32_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t max(std::uint32_t a, std::uint32_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::uint32_t, 2> minmax(std::uint32_t a, std::uint32_t b) {
        if (a < b) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t clamp(std::uint32_t x, std::uint32_t lo, std::uint32_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t average(std::uint32_t a, std::uint32_t b) {
        //TODO: AARCH32-suitable implementation
        return (std::uint64_t(a) + std::uint64_t(b)) >> 1;
    }

    [[nodiscard]]
    AVEL_FINL std::uint32_t midpoint(std::uint32_t a, std::uint32_t b) {
        //TODO: AARCH32-suitable implementation
        auto t0 = (b < a) & (a ^ b) & 0x1;
        return ((std::uint64_t(a) + std::uint64_t(b)) >> 1) + t0;
    }

}

#endif
