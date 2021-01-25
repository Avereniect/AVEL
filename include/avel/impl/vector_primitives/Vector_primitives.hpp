#ifndef AVEL_VECTOR_PRIMITIVES_HPP
#define AVEL_VECTOR_PRIMITIVES_HPP

#include "../Capabilities.hpp"

#include <type_traits>
#include <cstdint>

namespace avel {

    ///
    /// Vector primitive used internally by avel::Vector class and by
    /// implementation functions
    ///
    /// \tparam T Vector scalar type
    /// \tparam N Number of elements in vector
    template<class T, int N>
    struct vector_primitive;

    #ifdef AVELCPP14
    template<class T, int N>
    using vector_primitive_t = typename vector_primitive<T, N>::type;
    #endif

    template<class T, int N>
    struct mask_primitive;


    #if defined(AVEL_AVX512BW)
        #include "Vector_primitive_AVX512BW.hpp"
    #endif

    #if defined(AVEL_AVX512F)
        #include "Vector_primitives_AVX512F.hpp"
    #endif

    #if defined(AVEL_AVX)
        #include "Vector_primitives_AVX.hpp"
    #endif

    #if defined(AVEL_SSE2)
        #include "Vector_primitives_SSE2.hpp"
    #else
        #include "Vector_primitves_default.hpp"
    #endif

}

#endif //AVEL_VECTOR_PRIMITIVES_HPP
