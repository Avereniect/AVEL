#ifndef AVEL_SCALAR16U_HPP
#define AVEL_SCALAR16U_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    std::uint16_t countl_one(std::uint16_t x);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::uint16_t set_bits<std::uint16_t>(bool x) {
        return -std::uint16_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t popcount(std::uint16_t x) {
        #if defined(AVEL_POPCNT)
        return _popcnt32(x);

        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & 0x5555);
        x = (x & 0x3333) + ((x >> 2) & 0x3333);
        x = (x + (x >> 4) & 0x0F0F);
        x = (x + (x >> 8) & 0x00FF);

        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t byteswap(std::uint16_t x) {
        #if defined(AVEL_GCC) || defined(AVEL_CLANG)
        return __builtin_bswap16(x);

        #else
        std::uint16_t ret = 0x00;

        ret |= (x << 8);
        ret |= (x >> 8);

        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t countl_zero(std::uint16_t x) {
        #if defined(AVEL_LZCNT)
        return static_cast<std::uint16_t>(_lzcnt_u32(x) - 16);

        #elif defined(AVEL_X86)
        if (x) {
            return 15 - _bit_scan_reverse(x);
        } else {
            return 16;
        }

        #else
        return countl_one(std::uint16_t(~x));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t countl_one(std::uint16_t x) {
        #if defined(AVEL_X86)
        return countl_zero(std::uint16_t(~x));
        #else
        std::uint32_t sum = (x == 0xFFFF);

        bool b0 = (0xFF00u & x) == 0xFF00u;
        sum += (b0 * 8);
        x <<= (b0 * 8);

        bool b1 = (0xF000u & x) == 0xF000u;
        sum += (b1 * 4);
        x <<= (b1 * 4);

        bool b2 = (0xC000u & x) == 0xC000u;
        sum += (b2 * 2);
        x <<= (b2 * 2);

        bool b3 = (0x8000u & x) == 0x8000u;
        sum += (b3 * 1);

        return sum;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t countr_zero(std::uint16_t x) {
        #if defined(AVEL_BMI)
        return __tzcnt_u32(x | 0xFFFF0000);
        if (x) {
            return _bit_scan_forward(x | 0xFFFF0000);
        } else {
            return 16;
        }
        #else
        std::uint16_t ret = 0x00;

        if (x == 0x00) {
            return 16;
        }

        x &= -x;

        std::uint16_t b = 0x00;
        b = bool(x & 0xAAAAu);
        ret |= b;
        b = bool(x & 0xCCCCu);
        ret |= (b << 1);
        b = bool(x & 0xF0F0u);
        ret |= (b << 2);
        b = bool(x & 0xFF00u);
        ret |= (b << 3);

        return ret;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t countr_one(std::uint16_t x) {
        return countr_zero(std::uint16_t(~x));
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t bit_width(std::uint16_t x) {
        #if defined(AVEL_X86)
        if (x == 0) {
            return 0;
        } else {
            return _bit_scan_reverse(x) + 1;
        }
        #else
        if (x == 0) {
            return 0;
        }

        std::uint16_t ret = 0x00;

        bool b0 = x & 0xFF00;
        ret += b0 * 8;
        x >>= (b0 * 8);

        bool b1 = x & 0xFFF0;
        ret += b1 * 4;
        x >>= (b1 * 4);

        bool b2 = x & 0xFFFC;
        ret += b2 * 2;
        x >>= (b2 * 2);

        bool b3 = x & 0xFFFE;
        ret += b3 * 1;

        return ret + 1;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t bit_floor(std::uint16_t x) {
        #if defined(AVEL_X86)
        if (x == 0) {
            return 0;
        }
        return 1 << _bit_scan_reverse(x);
        #else
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t bit_ceil(std::uint16_t x) {
        #if defined(AVEL_LZCNT)
        auto sh = (32 - _lzcnt_u32(x - 1));
        auto result = 1 << sh;
        return result;

        #elif defined(AVEL_X86)
        if (x == 0) {
            return 1;
        }

        auto width = _bit_scan_reverse(x);
        auto tmp = 1 << width;
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
        ++x;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::uint16_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t rotl(std::uint16_t x, long long s) {
        #if defined(AVEL_X86)
        return _rotwl(x, s);
        #else
        s &= 0x0F;
        if (s == 0) {
            return x;
        }

        return (x << s) | (x >> (16 - s));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t rotr(std::uint16_t x, long long s) {
        #if defined(AVEL_X86)
        return _rotwr(x, s);
        #else
        s &= 0x0F;
        if (s == 0) {
            return x;
        }

        return (x >> s) | (x << (16 - s));
        #endif
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int16_t neg_abs(std::uint16_t x) {
        std::int16_t y = x;
        if (y < 0) {
            return y;
        } else {
            return -y;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t keep(bool m, std::uint16_t x) {
        if (m) {
            return x;
        } else {
            return 0;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t clear(bool m, std::uint16_t x) {
        if (m) {
            return 0;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t blend(bool m, std::uint16_t a, std::uint16_t b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t min(std::uint16_t a, std::uint16_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t max(std::uint16_t a, std::uint16_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::uint16_t, 2> minmax(std::uint16_t a, std::uint16_t b) {
        if (a < b) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t clamp(std::uint16_t x, std::uint16_t lo, std::uint16_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t average(std::uint16_t a, std::uint16_t b) {
        return (std::uint32_t(a) + std::uint32_t(b)) >> 1;
    }

    [[nodiscard]]
    AVEL_FINL std::uint16_t midpoint(std::uint16_t a, std::uint16_t b) {
        auto t0 = (b < a) & (a ^ b) & 0x1;
        return ((std::uint32_t(a) + std::uint32_t(b)) >> 1) + t0;
    }

}

#endif //AVEL_SCALAR16U_HPP
