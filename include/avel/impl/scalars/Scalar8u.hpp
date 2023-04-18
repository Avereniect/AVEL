#ifndef AVEL_SCALAR8U_HPP
#define AVEL_SCALAR8U_HPP

namespace avel {

    //=====================================================
    // Forward declarations
    //=====================================================

    std::uint8_t countl_one(std::uint8_t x);

    std::uint8_t min(std::uint8_t a, std::uint8_t b);

    //=====================================================
    // Bit manipulation
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::uint8_t set_bits<std::uint8_t>(bool x) {
        return -std::uint8_t(x);
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t popcount(std::uint8_t x) {
        #if defined(AVEL_POPCNT)
        return _popcnt32(std::uint32_t{x});
        #else
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & 0x55);
        x = (x & 0x33) + ((x >> 2) & 0x33);
        x = (x + (x >> 4)) & 0x0F;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t byteswap(std::uint8_t x) {
        return x;
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t countl_zero(std::uint8_t x) {
        #if defined(AVEL_LZCNT)
        return static_cast<uint8_t>(_lzcnt_u32(x) - 24);

        #elif defined(AVEL_X86)
        if (x) {
            return 7 - _bit_scan_reverse(x);
        } else {
            return 8;
        }

        #else
        alignas(16) static constexpr std::uint8_t table0[16] {
            8, 3, 2, 2,
            1, 1, 1, 1,
            0, 0, 0, 0,
            0, 0, 0, 0
        };

        alignas(16) static constexpr std::uint8_t table1[16] {
            8, 7, 6, 6,
            5, 5, 5, 5,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        auto lo_nibble = x & 0x0F;
        auto hi_nibble = x >> 4;

        auto lo = table0[hi_nibble];
        auto hi = table1[lo_nibble];

        return min(lo, hi);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t countl_one(std::uint8_t x) {
        return countl_zero(std::uint8_t(~x));
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t countr_zero(std::uint8_t x) {
        #if defined(AVEL_BMI)
        return __tzcnt_u32(x | 0xFFFFFF00);

        #elif defined(AVEL_X86)
        if (x) {
            return _bit_scan_forward(x);
        } else {
            return 8;
        }

        #else
        std::uint8_t ret = 0x00;

        if (x == 0x00) {
            return 8;
        }

        x &= -x;

        std::uint8_t b;
        b = bool(x & 0xAAu);
        ret |= (b << 0);
        b = bool(x & 0xCCu);
        ret |= (b << 1);
        b = bool(x & 0xF0u);
        ret |= (b << 2);

        return ret;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t countr_one(std::uint8_t x) {
        return countr_zero(std::uint8_t(~x));
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t bit_width(std::uint8_t x) {
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

        std::uint8_t ret = 0x00;

        bool b0 = x & 0xF0;
        ret += b0 * 4;
        x >>= (b0 * 4);

        bool b1 = x & 0xFC;
        ret += b1 * 2;
        x >>= (b1 * 2);

        bool b2 = x & 0xFE;
        ret += b2 * 1;

        return ret + 1;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t bit_floor(std::uint8_t x) {
        #if defined(AVEL_X86)
        if (x == 0) {
            return 0;
        }
        return 1 << _bit_scan_reverse(x);

        #else
        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        return x - (x >> 1);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t bit_ceil(std::uint8_t x) {
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
        ++x;
        return x;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool has_single_bit(std::uint8_t x) {
        #if defined(AVEL_POPCNT)
        return popcount(x) == 1;
        #else
        return x && !(x & (x - 1));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t rotl(std::uint8_t x, long long s) {
        s &= 0x7;
        if (s == 0) {
            return x;
        }

        return (x << s) | (x >> (8 - s));
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t rotr(std::uint8_t x, long long s) {
        s &= 0x7;
        if (s == 0) {
            return x;
        }

        return (x >> s) | (x << (8 - s));
    }

    //=====================================================
    // General operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::int8_t neg_abs(std::uint8_t x) {
        std::int8_t y = x;
        if (y < 0) {
            return y;
        } else {
            return -y;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t keep(bool m, std::uint8_t x) {
        if (m) {
            return x;
        } else {
            return 0;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t clear(bool m, std::uint8_t x) {
        if (m) {
            return 0;
        } else {
            return x;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t blend(bool m, std::uint8_t a, std::uint8_t b) {
        if (m) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t min(std::uint8_t a, std::uint8_t b) {
        if (a < b) {
            return a;
        } else {
            return b;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t max(std::uint8_t a, std::uint8_t b) {
        if (a < b) {
            return b;
        } else {
            return a;
        }
    }

    [[nodiscard]]
    AVEL_FINL std::array<std::uint8_t, 2> minmax(std::uint8_t a, std::uint8_t b) {
        if (a < b) {
            return {a, b};
        } else {
            return {b, a};
        }
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t clamp(std::uint8_t x, std::uint8_t lo, std::uint8_t hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t average(std::uint8_t a, std::uint8_t b) {
        return (std::uint32_t(a) + std::uint32_t(b)) >> 1;
    }

    [[nodiscard]]
    AVEL_FINL std::uint8_t midpoint(std::uint8_t a, std::uint8_t b) {
        std::int32_t average = (static_cast<std::int32_t>(a) + static_cast<std::int32_t>(b)) >> 1;
        std::int32_t bias = (b < a) & (a ^ b) & 0x1;
        return static_cast<std::uint8_t>(average + bias);
    }

}

#endif //AVEL_SCALAR8U_TESTS_HPP
