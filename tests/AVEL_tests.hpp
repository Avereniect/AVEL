#ifndef AVEL_AVEL_TESTS_HPP
#define AVEL_AVEL_TESTS_HPP

#include <random>
#include <algorithm>

#include <gtest/gtest.h>

//#define AVEL_FORCE_INLINE
//#define AVEL_AUTO_DETECT
//#define AVEL_GFNI
//#define AVEL_AVX512VBMI2
//#define AVEL_AVX512VBMI
//#define AVEL_AVX512VPOPCNTDQ
//#define AVEL_AVX512BITALG
//#define AVEL_AVX512BW
//#define AVEL_AVX512VL
//#define AVEL_AVX512DQ
//#define AVEL_AVX512CD
//#define AVEL_AVX512F
//#define AVEL_FMA
//#define AVEL_AVX2
//#define AVEL_AVX
//#define AVEL_SSE42
//#define AVEL_SSE41
//#define AVEL_SSSE3
//#define AVEL_SSE3
//#define AVEL_SSE2
//#define AVEL_BMI2
//#define AVEL_BMI
//#define AVEL_LZCNT
//#define AVEL_POPCNT
//#define AVEL_X86

//#define AVEL_AARCH64
//#define AVEL_NEON

#include <avel/Avel.hpp>
#include <avel/Aligned_allocator.hpp>
#include <avel/Cache.hpp>
#include <avel/Scalar.hpp>
#include <avel/Vector.hpp>
#include <avel/Scalar_denominator.hpp>
#include <avel/Vector_denominator.hpp>

#include "Test_utils.hpp"

namespace avel_tests {

    using namespace avel;

    static constexpr std::size_t iterations = 16;

    static auto random8u  = [] () -> std::uint8_t {
        static auto tmp = std::mt19937{0xDEADBEEF};
        auto ret = tmp();
        return static_cast<std::uint8_t>(ret);
    };

    static auto random16u  = [] () -> std::uint16_t {
        static auto tmp = std::mt19937{0xDEADBEEF};
        auto ret = tmp();
        return static_cast<std::uint16_t>(ret);
    };

    static auto random32u  = [] () -> std::uint32_t {
        static auto tmp = std::mt19937{0xDEADBEEF};
        auto ret = tmp();
        return static_cast<std::uint32_t>(ret);
    };

    static auto random64u  = [] () -> std::uint64_t {
        static auto tmp = std::mt19937_64{0xDEADBEEF};
        auto ret = tmp();
        return static_cast<std::uint64_t>(ret);
    };

    static auto random32f = [] () -> float {
        static auto rng = std::mt19937{0xDEADBEEF};
        static std::uniform_int_distribution<std::uint32_t> distribution{
            0x0,
            static_cast<std::uint32_t>(2 * 0x7f7fffffu)
        };

        std::int32_t bits = distribution(rng);

        if (bits > 0x7f7fffff) {
            bits -= 0x7f7fffff;
            bits |= 0x80000000;
        }

        return avel::bit_cast<float>(bits);
    };

    static auto random64f = [] () -> double {
        static auto rng = std::mt19937_64{0xDEADBEEF};
        static std::uniform_int_distribution<std::uint64_t> distribution{
            0x0ull,
            static_cast<std::uint64_t>(2 * 0x7fefffffffffffffull)
        };

        std::int64_t bits = distribution(rng);

        if (bits > 0x7fefffffffffffffull) {
            bits -= 0x7fefffffffffffffull;
            bits |= 0x8000000000000000ull;
        }

        return avel::bit_cast<double>(bits);
    };

    //=====================================================
    // Random integer generation
    //=====================================================

    template<class I>
    inline I random_val();

    template<>
    inline std::uint8_t random_val<std::uint8_t>() {
        return random8u();
    }

    template<>
    inline std::int8_t random_val<std::int8_t>() {
        return random8u();
    }

    template<>
    inline std::uint16_t random_val<std::uint16_t>() {
        return random16u();
    }

    template<>
    inline std::int16_t random_val<std::int16_t>() {
        return random16u();
    }

    template<>
    inline std::uint32_t random_val<std::uint32_t>() {
        return random32u();
    }

    template<>
    inline std::int32_t random_val<std::int32_t>() {
        return random32u();
    }

    template<>
    inline std::uint64_t random_val<std::uint64_t>() {
        return random64u();
    }

    template<>
    inline std::int64_t random_val<std::int64_t>() {
        return random64u();
    }

    template<>
    inline float random_val<float>() {
        return random32f();
    }

    template<>
    inline double random_val<double>() {
        return random64f();
    }

    template<>
    inline bool random_val<bool>() {
        return random8u() & 0x1;
    }




    template<class F>
    std::int32_t random_exponent();

    template<>
    inline std::int32_t random_exponent<std::int32_t>() {
        return (random32u() % 279) - 150;
    }

    template<>
    inline std::int32_t random_exponent<std::int64_t>() {
        return (random32u() % 2100) - 1075;
    }



    template<class I>
    inline I random_shift() {
        constexpr auto bound = sizeof(I) * CHAR_BIT + 1;
        return random_val<I>() % bound;
    }



    template<class I>
    inline I random_denominator() {
        auto tmp = random_val<I>();
        if (tmp == 0x0) {
            return 1;
        } else {
            return tmp;
        }
    }

    //=====================================================
    // Random array generation
    //=====================================================

    template<class A>
    inline A random_array() {
        A ret{};

        for (int i = 0; i < ret.size(); ++i) {
            ret[i] = random_val<typename A::value_type>();
        }

        return ret;
    }

    template<class A>
    inline A random_exponent_array() {
        A ret{};

        for (int i = 0; i < ret.size(); ++i) {
            ret[i] = random_exponent<typename A::value_type>();
        }

        return ret;
    }

    template<class A>
    inline A random_denominator_array() {
        A ret{};

        for (int i = 0; i < ret.size(); ++i) {
            ret[i] = random_val<typename A::value_type>();
            if (ret[i] == 0x0) {
                ret[i] = 0x1;
            }
        }

        return ret;
    }

    template<class A>
    inline A random_shift_array() {
        A ret{};

        for (int i = 0; i < ret.size(); ++i) {
            ret[i] = random_shift<typename A::value_type>();
        }

        return ret;
    }

    //=====================================================
    // Random vector generation
    //=====================================================

    template<class V>
    inline V random_vec() {
        using scalar = typename V::scalar;
        std::array<scalar, V::width> arr{};

        for (int i = 0; i < V::width; ++i) {
            arr[i] = random_val<scalar>();
        }

        return V{arr};
    }

    template<class V>
    inline V random_denominator_vec() {
        using scalar = typename V::scalar;
        std::array<scalar, V::width> arr;

        for (int i = 0; i < V::width; ++i) {
            arr[i] = avel::max(random_val<scalar>(), scalar(1));
        }

        return V{arr};
    }

    //=====================================================
    // Comparison Functions
    //=====================================================

    template<class V>
    [[nodiscard]]
    bool compare_stored_data(
        const typename V::scalar* p,
        V v,
        avel::Vector<typename avel::to_index_type<typename V::scalar>::type, V::width> indices,
        int j
    ) {
        auto arr = to_array(v);
        auto ind = to_array(indices);

        for (std::uint32_t i = 0; i < V::width; ++i) {
            if (i < j) {
                if (p[ind[i]] != arr[i]) {
                    return false;
                }
            }
        }

        return true;
    }

    template<class V>
    [[nodiscard]]
    bool compare_stored_data(const typename V::scalar* p, V v, int j) {
        auto arr = to_array(v);
        for (std::uint32_t i = 0; i < V::width; ++i) {
            if (i < j) {
                if (p[i] != arr[i]) {
                    return false;
                }
            } else {
                if (p[i] != 0x00) {
                    return false;
                }
            }
        }

        return true;
    }

    template<class T, class U>
    [[nodiscard]]
    T compare_common_bytes(const T& t, const U& u) {
        static_assert(std::is_trivial<T>::value, "Target type must be trivial");
        static_assert(std::is_trivial<U>::value, "Source type must be trivial");

        auto* p0 = reinterpret_cast<const unsigned char*>(&t);
        auto* p1 = reinterpret_cast<const unsigned char*>(&u);

        bool are_equal = true;
        for (std::size_t i = 0; i < std::min(sizeof(T), sizeof(U)); ++i) {
            are_equal &= (p0[i] == p1[i]);
        }

        return are_equal;
    }

}

#endif //AVEL_AVEL_TESTS_HPP
