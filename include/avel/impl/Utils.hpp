//
// Created by avereniect on 1/3/22.
//

#ifndef AVEL_UTILS_HPP
#define AVEL_UTILS_HPP

#include <cstdint>
#include <type_traits>

namespace avel {

    template<class T, class U>
    AVEL_FINL T reinterpret_bits(const U v) {
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

}

#endif //AVEL_UTILS_HPP
