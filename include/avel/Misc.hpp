//
// Created by avereniect on 1/3/22.
//

#ifndef AVEL_MISC_HPP
#define AVEL_MISC_HPP

#if __cplusplus >= 202002L
    #include <bit>
#endif

#include <type_traits>
#include <cstdint>

#include "avel/impl/Capabilities.hpp"

namespace avel {

    //=====================================================
    // Utility classes
    //=====================================================

    ///
    /// class that is returned by avel::div to return quotient and remainder
    /// values
    ///
    /// \tparam T An integral type
    template<class T>
    struct alignas(2 * alignof(T)) div_type {
        T quot;
        T rem;
    };

    //=====================================================
    // Bit-wise operations
    //=====================================================

    template<class T, class U>
    AVEL_FINL T bit_cast(const U& u) {
        static_assert(sizeof(T) == sizeof(U), "Size of target and source types must be equal");
        static_assert(std::is_trivially_copyable<T>::value, "Target type must be trivial");
        static_assert(std::is_trivially_copyable<U>::value, "Source type must be trivial");

        #if __cplusplus >= 202002L
        return std::bit_cast<T>(u);

        #else

        T ret;

        auto* src = reinterpret_cast<const unsigned char*>(&u);
        auto* dst = reinterpret_cast<unsigned char*>(&ret);

        for (std::size_t i = 0; i < sizeof(T); ++i) {
            dst[i] = src[i];
        }

        return ret;
        #endif
    }

    //=====================================================
    // Synchronization
    //=====================================================

    void load_fence() {
        #if defined(AVEL_SSE2)
        _mm_lfence();
        #endif
    }

    void store_fence() {
        #if defined(AVEL_SSE2)
        _mm_sfence();
        #endif
    }

    void load_store_fence() {
        #if defined(AVEL_SSE2)
        _mm_mfence();
        #endif
    }

}

#endif //AVEL_MISC_HPP
