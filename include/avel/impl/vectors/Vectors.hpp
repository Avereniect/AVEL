#ifndef AVEL_IMPL_VECTORS_HPP
#define AVEL_IMPL_VECTORS_HPP

#include <cmath>

#include "../vector_primitives/Vector_primitives.hpp"
#include "../Utils.hpp"
#include "../Constants.hpp"

namespace avel {

    template<class V>
    V load(const typename V::scalar_type*);

    template<class V>
    V aligned_load(const typename V::scalar_type*);

    template<class V>
    V stream_load(const typename V::scalar_type*);

    template<class V>
    V gather(const typename V::scalar_type*, Vector<std::int32_t, V::width>);

    template<class V>
    typename V::primitive to_primitive(V v) {
        return static_cast<typename V::primtive>(v);
    }

}

//Note: The order of inclusion of the following files in meaningful

//128-bit vectors

#if defined(AVEL_SSE2) | defined(AVEL_NEON)

#include "Vec4x32u.hpp"
#include "Vec4x32i.hpp"
#include "Vec4x32f.hpp"

#endif

//256-bit vectors

#ifdef AVEL_AVX

#include "Vec8x32u.hpp"
#include "Vec8x32i.hpp"
#include "Vec8x32f.hpp"

#endif

//512-bit vectors

#ifdef AVEL_AVX512F

#include "Vec16x32u.hpp"
#include "Vec16x32i.hpp"
#include "Vec16x32f.hpp"

#endif

/*
 * Should these specializations be made?
 * The vector is meant to largely replace a scalar in many contexts and this
 * specialization may be required by certain templates.
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
