#ifndef AVEL_IMPL_VECTORS_HPP
#define AVEL_IMPL_VECTORS_HPP

#include <cmath>
#include <array>

#include "avel/Misc.hpp"

#include "../Traits.hpp"
#include "../Constants.hpp"

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    // Boolean Array aliases

    using arr1xb = std::array<bool, 1>;
    using arr2xb = std::array<bool, 2>;
    using arr4xb = std::array<bool, 4>;
    using arr8xb = std::array<bool, 8>;
    using arr16xb = std::array<bool, 16>;
    using arr32xb = std::array<bool, 32>;
    using arr64xb = std::array<bool, 64>;

    // Vector, Vector mask, array aliases

    using vec1x8u = Vector<std::uint8_t, 1>;
    using vec1x8i = Vector<std::int8_t,  1>;

    using vec1x16u = Vector<std::uint16_t, 1>;
    using vec1x16i = Vector<std::int16_t,  1>;

    using vec1x32u = Vector<std::uint32_t, 1>;
    using vec1x32i = Vector<std::int32_t,  1>;

    using vec1x64u = Vector<std::uint64_t, 1>;
    using vec1x64i = Vector<std::int64_t,  1>;



    using mask1x8u = Vector_mask<std::uint8_t, 1>;
    using mask1x8i = Vector_mask<std::int8_t,  1>;

    using mask1x16u = Vector_mask<std::uint16_t, 1>;
    using mask1x16i = Vector_mask<std::int16_t,  1>;

    using mask1x32u = Vector_mask<std::uint32_t, 1>;
    using mask1x32i = Vector_mask<std::int32_t,  1>;

    using mask1x64u = Vector_mask<std::uint64_t, 1>;
    using mask1x64i = Vector_mask<std::int64_t,  1>;



    using arr1x8u = std::array<std::uint8_t, 1>;
    using arr1x8i = std::array<std::int8_t,  1>;

    using arr1x16u = std::array<std::uint16_t, 1>;
    using arr1x16i = std::array<std::int16_t,  1>;

    using arr1x32u = std::array<std::uint32_t, 1>;
    using arr1x32i = std::array<std::int32_t,  1>;

    using arr1x64u = std::array<std::uint64_t, 1>;
    using arr1x64i = std::array<std::int64_t,  1>;





    #if defined(AVEL_SSE2) || defined(AVEL_NEON)

    using vec16x8u = Vector<std::uint8_t, 16>;
    using vec16x8i = Vector<std::int8_t,  16>;

    using vec8x16u = Vector<std::uint16_t, 8>;
    using vec8x16i = Vector<std::int16_t,  8>;

    using vec4x32u = Vector<std::uint32_t, 4>;
    using vec4x32i = Vector<std::int32_t,  4>;

    using vec2x64u = Vector<std::uint64_t, 2>;
    using vec2x64i = Vector<std::int64_t,  2>;



    using mask16x8u = Vector_mask<std::uint8_t, 16>;
    using mask16x8i = Vector_mask<std::int8_t,  16>;

    using mask8x16u = Vector_mask<std::uint16_t, 8>;
    using mask8x16i = Vector_mask<std::int16_t,  8>;

    using mask4x32u = Vector_mask<std::uint32_t, 4>;
    using mask4x32i = Vector_mask<std::int32_t,  4>;

    using mask2x64u = Vector_mask<std::uint64_t, 2>;
    using mask2x64i = Vector_mask<std::int64_t,  2>;



    using arr16x8u = std::array<std::uint8_t, 16>;
    using arr16x8i = std::array<std::int8_t,  16>;

    using arr8x16u = std::array<std::uint16_t, 8>;
    using arr8x16i = std::array<std::int16_t,  8>;

    using arr4x32u = std::array<std::uint32_t, 4>;
    using arr4x32i = std::array<std::int32_t,  4>;

    using arr2x64u = std::array<std::uint64_t, 2>;
    using arr2x64i = std::array<std::int64_t,  2>;

    #endif





    #if defined(AVEL_AVX)

    using vec32x8u = Vector<std::uint8_t, 32>;
    using vec32x8i = Vector<std::int8_t,  32>;

    using vec16x16u = Vector<std::uint16_t, 16>;
    using vec16x16i = Vector<std::int16_t,  16>;

    using vec8x32u = Vector<std::uint32_t, 8>;
    using vec8x32i = Vector<std::int32_t,  8>;

    using vec4x64u = Vector<std::uint64_t, 4>;
    using vec4x64i = Vector<std::int64_t,  4>;



    using mask32x8u = Vector_mask<std::uint8_t, 32>;
    using mask32x8i = Vector_mask<std::int8_t,  32>;

    using mask16x16u = Vector_mask<std::uint16_t, 16>;
    using mask16x16i = Vector_mask<std::int16_t,  16>;

    using mask8x32u = Vector_mask<std::uint32_t, 8>;
    using mask8x32i = Vector_mask<std::int32_t,  8>;

    using mask4x64u = Vector_mask<std::uint64_t, 4>;
    using mask4x64i = Vector_mask<std::int64_t,  4>;



    using arr32x8u = std::array<std::uint8_t, 32>;
    using arr32x8i = std::array<std::int8_t,  32>;

    using arr16x16u = std::array<std::uint16_t, 16>;
    using arr16x16i = std::array<std::int16_t,  16>;

    using arr8x32u = std::array<std::uint32_t, 8>;
    using arr8x32i = std::array<std::int32_t,  8>;

    using arr4x64u = std::array<std::uint64_t, 4>;
    using arr4x64i = std::array<std::int64_t,  4>;

    #endif

    #if defined(AVEL_AVX512F)

    using vec64x8u = Vector<std::uint8_t, 64>;
    using vec64x8i = Vector<std::int8_t,  64>;

    using vec32x16u = Vector<std::uint16_t, 32>;
    using vec32x16i = Vector<std::int16_t,  32>;

    using vec16x32u = Vector<std::uint32_t, 16>;
    using vec16x32i = Vector<std::int32_t,  16>;

    using vec8x64u = Vector<std::uint64_t, 8>;
    using vec8x64i = Vector<std::int64_t,  8>;



    using mask64x8u = Vector_mask<std::uint8_t, 64>;
    using mask64x8i = Vector_mask<std::int8_t,  64>;

    using mask32x16u = Vector_mask<std::uint16_t, 32>;
    using mask32x16i = Vector_mask<std::int16_t,  32>;

    using mask16x32u = Vector_mask<std::uint32_t, 16>;
    using mask16x32i = Vector_mask<std::int32_t,  16>;

    using mask8x64u = Vector_mask<std::uint64_t, 8>;
    using mask8x64i = Vector_mask<std::int64_t,  8>;



    using arr64x8u = std::array<std::uint8_t, 64>;
    using arr64x8i = std::array<std::int8_t,  64>;

    using arr32x16u = std::array<std::uint16_t, 32>;
    using arr32x16i = std::array<std::int16_t,  32>;

    using arr16x32u = std::array<std::uint32_t, 16>;
    using arr16x32i = std::array<std::int32_t,  16>;

    using arr8x64u = std::array<std::uint64_t, 8>;
    using arr8x64i = std::array<std::int64_t,  8>;

    #endif

    //=====================================================
    // Load/Store Functions
    //=====================================================

    template<class V>
    V load(const typename V::scalar* p, std::uint32_t n);

    template<class V, std::uint32_t N = V::width>
    V load(const typename V::scalar* ptr) {
        static_assert(N <= V::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= V::width, int>::type dummy_variable = 0;

        //Relying on const folding to optimize this implementation
        return load<V>(ptr, N);
    }



    template<class V>
    V aligned_load(const typename V::scalar* p, std::uint32_t n);

    template<class V, std::uint32_t N = V::width>
    V aligned_load(const typename V::scalar* ptr) {
        static_assert(N <= V::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= V::width, int>::type dummy_variable = 0;

        //Relying on const folding to optimize this implementation
        return aligned_load<V>(ptr, N);
    }



    template<class V>
    V gather(const typename V::scalar* ptr, Vector<typename to_index_type<typename V::scalar>::type, V::width> indices, std::uint32_t n);

    template<class V, std::uint32_t N = V::width>
    V gather(const typename V::scalar* ptr, Vector<typename to_index_type<typename V::scalar>::type, V::width> indices) {
        static_assert(N <= V::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= V::width, int>::type dummy_variable = 0;

        //Relying on const folding, branch elimination to optimize this implementation
        return gather<V>(ptr, indices, N);
    }

    //=====================================================
    // Misc. Functions
    //=====================================================

    template<class V>
    typename V::primitive decay(V v) {
        return static_cast<typename V::primitive>(v);
    }

    //=====================================================
    // Conversion Functions
    //=====================================================

    template<class V0, class V1 = V0, typename std::enable_if<std::is_same<V0, V1>::value, bool>::type = true>
    std::array<V0, 1> convert(V1 v) {
        return {v};
    }

    template<class V0, class V1 = V0, typename std::enable_if<!std::is_same<V0, V1>::value, bool>::type = true>
    std::array<V0, V1::width / V0::width + bool(V1::width % V0::width)> convert(V1 v);

}

// Note: The order of inclusion of the following files is meaningful as later
// files use declarations/definitions contained within earlier files

//Native vectors

#include "Vec1x8u.hpp"
#include "Vec1x8i.hpp"

#include "Vec1x16u.hpp"
#include "Vec1x16i.hpp"

#include "Vec1x32u.hpp"
#include "Vec1x32i.hpp"

#include "Vec1x64u.hpp"
#include "Vec1x64i.hpp"

//#include "Vec1x32f.hpp"
//#include "Vec1x64f.hpp"

//128-bit vectors

#if defined(AVEL_SSE2) || defined(AVEL_NEON)
    #include "Vec16x8u.hpp"
    #include "Vec16x8i.hpp"

    #include "Vec8x16u.hpp"
    #include "Vec8x16i.hpp"

    #include "Vec4x32u.hpp"
    #include "Vec4x32i.hpp"

    #include "Vec2x64u.hpp"
    #include "Vec2x64i.hpp"

    //#include "Vec4x32f.hpp"
    //#include "Vec2x64f.hpp"

#endif

/*
//256-bit vectors

#if defined(AVEL_AVX)
    #include "Vec8x32u.hpp"
    #include "Vec8x32i.hpp"

    #include "Vec4x64u.hpp"
    #include "Vec4x64i.hpp"

    #include "Vec8x32f.hpp"
    #include "Vec4x64f.hpp"

#endif

#if defined(AVEL_AVX2)
    #include "Vec32x8u.hpp"
    #include "Vec32x8i.hpp"

    #include "Vec16x16u.hpp"
    #include "Vec16x16i.hpp"

#endif

//512-bit vectors

#if defined(AVEL_AVX512F)
    #include "Vec64x8u.hpp"
    #include "Vec64x8i.hpp"

    #include "Vec32x16u.hpp"
    #include "Vec32x16i.hpp"

    #include "Vec16x32u.hpp"
    #include "Vec16x32i.hpp"

    #include "Vec8x64u.hpp"
    #include "Vec8x64i.hpp"

    #include "Vec16x32f.hpp"
    #include "Vec8x64f.hpp"

#endif
*/

/*
 * Should these specializations be made?
 * The vector is meant to largely replace a scalar in many contexts and this
 * specialization may be useful to code built on top of this library
 *
namespace std {

    ///
    /// \tparam T AVEL vector type
    /// \tparam N AVEL vector width
    template<class T, std::size_t N>
    class numeric_limits<avel::Vector<T, N>> {
    public:

        static constexpr bool is_specialized = true;
        static constexpr bool is_signed = numeric_limits<T>::is_signed;
        static constexpr bool is_integer = numeric_limits<T>::is_integer;
        static constexpr bool is_exact = numeric_limits<T>::is_exact;
        static constexpr bool has_infinity = numeric_limits<T>::has_infinity;
        static constexpr bool has_quiet_NaN = numeric_limits<T>::has_quiet_NaN;
        static constexpr bool has_signaling_NaN = numeric_limits<T>::has_signaling_NaN;
        static constexpr bool has_denorm = numeric_limits<T>::has_denorm;
        static constexpr bool has_denorm_loss = numeric_limits<T>::has_denorm_loss;
        static constexpr std::float_round_style round_style = numeric_limits<T>::round_style;
        static constexpr bool is_iec559 = numeric_limits<T>::is_iec559;
        static constexpr bool is_bounded = numeric_limits<T>::is_bounded;
        static constexpr bool is_modulo = numeric_limits<T>::is_modulo;
        static constexpr int digits = numeric_limits<T>::digits;
        static constexpr int digits10 = numeric_limits<T>::digits10;
        static constexpr int max_digits10 = numeric_limits<T>::max_digits10;
        static constexpr int radix = numeric_limits<T>::radix;
        static constexpr int min_exponent =

    };

}
*/

#endif //AVEL_IMPL_VECTORS_HPP
