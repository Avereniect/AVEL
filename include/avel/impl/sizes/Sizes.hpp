#ifndef AVEL_SIZES_HPP
#define AVEL_SIZES_HPP

#include <cstdint>
#include <functional>

namespace avel {

    ///
    /// A template used to determine the optimal size of vector is.
    ///
    /// Here optimal size means the largest size of a vector such that the
    /// current instruction set supports most operations on a SIMD register of
    /// the same size. Certain instruction sets allow registers to be used for
    /// types for which few SIMD instructions are supported and whose use
    /// would therefore be suboptimal/
    ///
    /// \tparam T An integral or floating-point type
    template<class T>
    struct optimal_vector_width : public std::integral_constant<int, 0> {};

    #ifdef AVEL_CPP14
    template<class T>
    int optimal_vector_width_v = optimal_vector_width<T>::value;
    #endif



    ///
    /// A template used to determine the largest vector size for the given type
    /// which is supported by the current instruction set.
    ///
    /// The largest vector size is not necessarily the most appropriate size to
    /// use however. avel::optimal_vector_width should be used to query the
    /// optimal vector size for performance considerations.
    ///
    /// \tparam T An integral or floating-point type
    template<class T>
    struct max_vector_width : public std::integral_constant<int, 0>{};

    #ifdef AVEL_CPP14
    template<class T>
    int max_vector_width_v = max_vector_width<T>::value;
    #endif

    ///
    /// \tparam T
    template<class T>
    struct optimal_saturated_vector_width : public std::integral_constant<int, 0>{};

    #ifdef AVEL_CPP14
    template<class T>
    int optimal_saturated_vector_width = max_vector_width<T>::value;
    #endif


    #if defined(AVEL_AVX512BW)
        #include "Sizes_AVX512BW.ipp"
    #elif defined(AVEL_AVX512F)
        #include "Sizes_AVX512F.ipp"
    #elif defined(AVEL_AVX2)
        #include "Sizes_AVX2.ipp"
    #elif defined(AVEL_AVX)
        #include "Sizes_AVX.ipp"
    #elif defined (AVEL_SSE2)
        #include "Sizes_SSE2.ipp"
    #elif defined(AVEL_NEON)
        #include "Sizes_NEON.ipp"
    #else
        #include "Sizes_default.ipp"
    #endif

}


#endif //AVEL_SIZES_HPP
