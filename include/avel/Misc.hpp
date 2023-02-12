#ifndef AVEL_MISC_HPP
#define AVEL_MISC_HPP

#if __cplusplus >= 202002L
    #include <bit>
#endif

#include <type_traits>
#include <cstdint>
#include <cstring>

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
        static_assert(std::is_trivially_copyable<T>::value, "Target type must be trivially copyable");
        static_assert(std::is_trivially_copyable<U>::value, "Source type must be trivially copyable");

        #if __cplusplus >= 202002L
        return std::bit_cast<T>(u);

        #elif defined(AVEL_GCC)
        //GCC offers type punning via unions as implementation-defined behavior
        constexpr auto alignment = alignof(T) > alignof(U) ? alignof(T) : alignof(U);
        union Helper{
            alignas(alignment) T t;
            alignas(alignment) U u;
        };

        Helper helper;
        helper.u = u;
        return helper.t;

        #else
        T ret;
        std::memcpy(&ret, &u, sizeof(T));
        return ret;

        #endif
    }

}

#endif //AVEL_MISC_HPP
