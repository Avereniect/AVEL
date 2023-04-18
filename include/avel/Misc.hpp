#ifndef AVEL_MISC_HPP
#define AVEL_MISC_HPP

#if __cplusplus >= 202002
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

        #if __cplusplus >= 202002
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

    //=====================================================
    // Integer widths
    //=====================================================

    template<class T>
    class type_width : public std::integral_constant<std::size_t, CHAR_BIT * sizeof(T)> {};

    #if 201402L <= __cplusplus

    template<class T>
    constexpr bool type_width_v = type_width<T>::value;

    #endif

}

#endif //AVEL_MISC_HPP
