#ifndef AVEL_IMPL_VECTORS_HPP
#define AVEL_IMPL_VECTORS_HPP

#include <cmath>
#include <cfloat>
#include <array>

#if defined(AVEL_NEON) && (__cplusplus >= 202002)
#include <memory> //Necessary to use std::assume_aligned
#endif

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



    // Array aliases

    using arr1x8u = std::array<std::uint8_t, 1>;
    using arr1x8i = std::array<std::int8_t,  1>;

    using arr1x16u = std::array<std::uint16_t, 1>;
    using arr1x16i = std::array<std::int16_t,  1>;

    using arr1x32u = std::array<std::uint32_t, 1>;
    using arr1x32i = std::array<std::int32_t,  1>;

    using arr1x64u = std::array<std::uint64_t, 1>;
    using arr1x64i = std::array<std::int64_t,  1>;

    //=====================================================
    // Load/Store Functions
    //=====================================================

    template<class V>
    V load(const typename V::scalar* p, std::uint32_t n);

    template<class V, std::uint32_t N = V::width>
    AVEL_FINL V load(const typename V::scalar* ptr) {
        static_assert(N <= V::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= V::width, int>::type dummy_variable = 0;

        //Relying on const folding to optimize this implementation
        return load<V>(ptr, N);
    }



    template<class V>
    V aligned_load(const typename V::scalar* p, std::uint32_t n);

    template<class V, std::uint32_t N = V::width>
    AVEL_FINL V aligned_load(const typename V::scalar* ptr) {
        static_assert(N <= V::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= V::width, int>::type dummy_variable = 0;

        //Relying on const folding to optimize this implementation
        return aligned_load<V>(ptr, N);
    }



    template<class V>
    V gather(
        const typename V::scalar* ptr,
        Vector<typename to_index_type<typename V::scalar>::type, V::width> indices,
        std::uint32_t n
    );

    template<class V, std::uint32_t N = V::width>
    AVEL_FINL V gather(
        const typename V::scalar* ptr,
        Vector<typename to_index_type<typename V::scalar>::type, V::width> indices
    ) {
        static_assert(N <= V::width, "Cannot load more elements than width of vector");
        typename std::enable_if<N <= V::width, int>::type dummy_variable = 0;

        //Relying on const folding, branch elimination to optimize this implementation
        return gather<V>(ptr, indices, N);
    }

    //=====================================================
    // Misc. Functions
    //=====================================================

    template<class V>
    AVEL_FINL typename V::primitive decay(V v) {
        return static_cast<typename V::primitive>(v);
    }

    //=====================================================
    // Conversion Functions
    //=====================================================

    template<class V0, class V1 = V0, class = typename std::enable_if<std::is_same<V0, V1>::value>::type>
    [[nodiscard]]
    AVEL_FINL std::array<V0, 1> convert(V1 v) {
        return {v};
    }

    template<class V0, class V1 = V0, class = typename std::enable_if<!std::is_same<V0, V1>::value>::type>
    [[nodiscard]]
    AVEL_FINL std::array<V0, V1::width / V0::width + bool(V1::width % V0::width)> convert(V1 v);

    /*
    template<
        class V,
        class = typename avel::enable_if_vector_or_vector_mask_t<V>
    >
    [[nodiscard]]
    AVEL_FINL std::array<V, 1> convert(V v) {
        return v;
    }

    template<
        class V0,
        class V1 = V0,
        class = typename avel::enable_if_vector_or_vector_mask_t<V0>,
        class = typename avel::enable_if_vector_or_vector_mask_t<V1>
    >
    [[nodiscard]]
    AVEL_FINL std::array<V0, V1::width / V0::width + bool(V1::width % V0::width)> convert(V1 v);
    */


    /*
    template<
        class V,
        class = typename avel::enable_if_vector_or_vector_mask_t<V>
    >
    [[nodiscard]]
    AVEL_FINL V widen(V v) {
        return v;
    }

    template<
        class V0,
        class V1 = V0,
        class = typename avel::enable_if_vector_or_vector_mask_t<V0>,
        class = typename avel::enable_if_vector_or_vector_mask_t<V1>,
        class = typename std::enable_if<std::is_same<typename V0::scalar, typename V1::scalar>::value>::type
    >
    [[nodiscard]]
    AVEL_FINL V0 widen(V1 v);
    */

/*
#include "Widen.hpp"

#include "Zero_extend.hpp"

    template<
        class V0,
        class V1,
        class = typename avel::enable_if_integer_vector_t<V0>,
        class = typename avel::enable_if_integer_vector_t<V1>,
        class = typename std::enable_if<sizeof(V0::scalar) >= sizeof(V1::scalar)>::type
    >
    [[nodiscard]]
    AVEL_FINL std::array<V0, V1::width / V0::width + bool(V1::width % V0::width)> zero_extend(V1 v) {
        return avel_impl::zero_extend<V0, V1>(v, V0{});
    }
*/

    //=====================================================
    // Integer comparison functions
    //=====================================================

    template<std::size_t N, class Uint, class = typename std::enable_if<std::is_unsigned<Uint>::value>>
    AVEL_FINL avel::Vector_mask<Uint, N> operator==(
        avel::Vector<Uint, N> lhs,
        avel::Vector<typename std::make_signed<Uint>::type, N> rhs
    ) {
        auto t0 = (lhs == avel::Vector<Uint, N>{rhs});
        auto t1 = (rhs < avel::Vector<typename std::make_signed<Uint>::type, N>{0});
        return t0 && !t1;
    }

    template<std::size_t N, class Int, class = typename std::enable_if<std::is_signed<Int>::value>>
    AVEL_FINL avel::Vector_mask<Int, N> operator==(
        avel::Vector<Int, N> lhs,
        avel::Vector<typename std::make_unsigned<Int>::type, N> rhs
    ) {
        auto t0 = (lhs == avel::Vector<Int, N>{rhs});
        auto t1 = (lhs < avel::Vector<Int, N>{0});
        return t0 && !t1;
    }



    template<std::size_t N, class Uint, class = typename std::enable_if<std::is_unsigned<Uint>::value>>
    AVEL_FINL avel::Vector_mask<Uint, N> operator!=(
        avel::Vector<Uint, N> lhs,
        avel::Vector<typename std::make_signed<Uint>::type, N> rhs
    ) {
        auto t0 = (lhs != avel::Vector<Uint, N>{rhs});
        auto t1 = (rhs < avel::Vector<typename std::make_signed<Uint>::type, N>{0});
        return t0 || t1;
    }

    template<std::size_t N, class Int, class = typename std::enable_if<std::is_signed<Int>::value>>
    AVEL_FINL avel::Vector_mask<Int, N> operator!=(
        avel::Vector<Int, N> lhs,
        avel::Vector<typename std::make_unsigned<Int>::type, N> rhs
    ) {
        auto t0 = (lhs != avel::Vector<Int, N>{rhs});
        auto t1 = (lhs < avel::Vector<typename std::make_signed<Int>::type, N>{0});
        return t0 || t1;
    }



    template<std::size_t N, class Uint, class = typename std::enable_if<std::is_unsigned<Uint>::value>>
    AVEL_FINL avel::Vector_mask<Uint, N> operator<(
        avel::Vector<Uint, N> lhs,
        avel::Vector<typename std::make_signed<Uint>::type, N> rhs
    ) {
        auto t0 = avel::Vector<typename std::make_signed<Uint>::type, N>{lhs} < rhs;
        auto t1 = (avel::Vector<typename std::make_signed<Uint>::type, N>{lhs} | rhs) < avel::Vector<Uint, N>{0};
        return !t0 && t1;
    }

    template<std::size_t N, class Int, class = typename std::enable_if<std::is_signed<Int>::value>>
    AVEL_FINL avel::Vector_mask<Int, N> operator<(
        avel::Vector<Int, N> lhs,
        avel::Vector<typename std::make_unsigned<Int>::type, N> rhs
    ) {
        auto t0 = lhs < avel::Vector<Int, N>{rhs};
        auto t1 = (lhs | avel::Vector<Int, N>{rhs}) < avel::Vector<Int, N>{0};
        return t0 || t1;
    }



    template<std::size_t N, class Uint, class = typename std::enable_if<std::is_unsigned<Uint>::value>>
    AVEL_FINL avel::Vector_mask<Uint, N> operator<=(
        avel::Vector<Uint, N> lhs,
        avel::Vector<typename std::make_signed<Uint>::type, N> rhs
    ) {
        using Sint = typename std::make_signed<Uint>::type;

        auto t0 = avel::Vector<Sint, N>{lhs} < rhs;
        auto t1 = avel::Vector<Sint, N>{lhs} | rhs < avel::Vector<Sint, N>{0};
        return avel::Vector_mask<Uint, N>{!t0 && t1};
    }

    template<std::size_t N, class Int, class = typename std::enable_if<std::is_signed<Int>::value>>
    AVEL_FINL avel::Vector_mask<Int, N> operator<=(
        avel::Vector<Int, N> lhs,
        avel::Vector<typename std::make_unsigned<Int>::type, N> rhs
    ) {
        auto t0 = lhs <= avel::Vector<Int, N>{rhs};
        auto t1 = lhs | avel::Vector<Int, N>{rhs} < avel::Vector<Int, N>{0};
        return t0 || t1;
    }



    template<std::size_t N, class Uint, class = typename std::enable_if<std::is_unsigned<Uint>::value>>
    AVEL_FINL avel::Vector_mask<Uint, N> operator>(
        avel::Vector<Uint, N> lhs,
        avel::Vector<typename std::make_signed<Uint>::type, N> rhs
    ) {
        return avel::Vector_mask<Uint, N>{rhs < lhs};
    }

    template<std::size_t N, class Int, class = typename std::enable_if<std::is_signed<Int>::value>>
    AVEL_FINL avel::Vector_mask<Int, N> operator>(
        avel::Vector<Int, N> lhs,
        avel::Vector<typename std::make_unsigned<Int>::type, N> rhs
    ) {
        return avel::Vector_mask<Int, N>{rhs < lhs};
    }



    template<std::size_t N, class Uint, class = typename std::enable_if<std::is_unsigned<Uint>::value>>
    AVEL_FINL avel::Vector_mask<Uint, N> operator>=(
        avel::Vector<Uint, N> lhs,
        avel::Vector<typename std::make_signed<Uint>::type, N> rhs
    ) {
        return avel::Vector_mask<Uint, N>{rhs <= lhs};
    }

    template<std::size_t N, class Int, class = typename std::enable_if<std::is_signed<Int>::value>>
    AVEL_FINL avel::Vector_mask<Int, N> operator>=(
        avel::Vector<Int, N> lhs,
        avel::Vector<typename std::make_unsigned<Int>::type, N> rhs
    ) {
        return avel::Vector_mask<Int, N>{rhs <= lhs};
    }

    //=====================================================
    // Specializations
    //=====================================================

    ///
    /// Version of bit_cast which is specific to vectors,
    /// simplifying the implementation under certain circumstances.
    ///
    /// \tparam V0 AVEL vector or vector mask type
    /// \tparam V1 AVEL vector or vector mask type
    /// \param v1 Object to cast
    /// \return Object with equivalent bit-wise representation
    template<class V0, class V1, typename = typename std::is_same<typename V0::primitive, typename V1::primitive>::value>
    AVEL_FINL V0 bit_cast(V1 v1) {
        return V0{decay(v1)};
    }

}

// Note: The order of inclusion of the following files is meaningful as later
// files use declarations/definitions contained within earlier files

#include "Vectors_common.hpp"

//Native vectors

#include "Vec1x8u.hpp"
#include "Vec1x8i.hpp"

#include "Vec1x16u.hpp"
#include "Vec1x16i.hpp"

#include "Vec1x32u.hpp"
#include "Vec1x32i.hpp"

#include "Vec1x64u.hpp"
#include "Vec1x64i.hpp"

#include "Vec1x32f.hpp"
#include "Vec1x64f.hpp"



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

    #include "Vec4x32f.hpp"
    #include "Vec2x64f.hpp"
#endif



//256-bit vectors

#if defined(AVEL_AVX2)
    #include "Vec32x8u.hpp"
    #include "Vec32x8i.hpp"

    #include "Vec16x16u.hpp"
    #include "Vec16x16i.hpp"

    #include "Vec8x32u.hpp"
    #include "Vec8x32i.hpp"

    #include "Vec4x64u.hpp"
    #include "Vec4x64i.hpp"

    #include "Vec8x32f.hpp"
    #include "Vec4x64f.hpp"
#endif

//512-bit vectors

#if defined(AVEL_AVX512F)
    #include "Vec16x32u.hpp"
    #include "Vec16x32i.hpp"

    #include "Vec8x64u.hpp"
    #include "Vec8x64i.hpp"

    #include "Vec16x32f.hpp"
    #include "Vec8x64f.hpp"
#endif

#if defined(AVEL_AVX512BW)
    #include "Vec64x8u.hpp"
    #include "Vec64x8i.hpp"

    #include "Vec32x16u.hpp"
    #include "Vec32x16i.hpp"
#endif



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
