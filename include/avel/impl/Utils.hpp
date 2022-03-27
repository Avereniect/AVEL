//
// Created by avereniect on 1/3/22.
//

#ifndef AVEL_UTILS_HPP
#define AVEL_UTILS_HPP

#include <type_traits>
#include "Capabilities.hpp"

namespace avel {

    template<class T, class U>
    AVEL_FINL T bit_cast(const U& v) {
        static_assert(sizeof(T) == sizeof(U), "Size of target and source types must be equal");
        static_assert(std::is_trivial<T>::value, "Target type must be trivial");
        static_assert(std::is_trivial<U>::value, "Source type must be trivial");

        T ret;
        auto* src = reinterpret_cast<const unsigned char*>(&v);
        auto* dst = reinterpret_cast<unsigned char*>(&ret);
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            dst[i] = src[i];
        }

        return ret;
    }

    template<std::size_t Alignment, class T>
    AVEL_FINL const T* align_pointer(const T* p) {
        return align_pointer<Alignment>(reinterpret_cast<const void*>(p));
    }

    template<std::size_t Alignment>
    AVEL_FINL const void* align_pointer(const void* p) {
        constexpr bool is_power_of_two = (Alignment && !(Alignment & (Alignment - 1)));
        static_assert(is_power_of_two, "Alignment was not a power of 2");

        auto bits = reinterpret_cast<intptr_t>(p);
        const intptr_t mask = !(intptr_t(Alignment) - 1);
        auto ret = (mask & bits);
        return reinterpret_cast<const void*>(ret);
    }

    template<std::size_t Alignment, class T>
    AVEL_FINL T* align_pointer(T* p) {
        return align_pointer<Alignment>(reinterpret_cast<void*>(p));
    }

    template<std::size_t Alignment>
    AVEL_FINL void* align_pointer(void* p) {
        constexpr bool is_power_of_two = (Alignment && !(Alignment & (Alignment - 1)));
        static_assert(is_power_of_two, "Alignment was not a power of 2");

        auto bits = reinterpret_cast<std::intptr_t>(p);
        const std::intptr_t mask = !(std::intptr_t(Alignment) - 1);
        auto ret = (mask & bits);
        return reinterpret_cast<void*>(ret);
    }

}

#endif //AVEL_UTILS_HPP
