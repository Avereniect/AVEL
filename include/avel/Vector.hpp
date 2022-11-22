#ifndef AVEL_VECTOR_HPP
#define AVEL_VECTOR_HPP

#include <cstdint>

#include "./impl/Capabilities.hpp"
#include "./impl/Sizes.hpp"

#include "Scalar.hpp"

namespace avel {

    ///
    /// A class representing a mask to be used
    ///
    /// \tparam T Integral or floating-point type
    /// \tparam N Vector width
    template<class T, std::uint32_t N>
    class Vector_mask;

    ///
    /// A class representing a SIMD register.
    ///
    /// Since this class represents a register, variables of this type should
    /// not be used in large amounts, or for purposes besides processing of
    /// data.
    ///
    /// Manipulations of
    ///
    /// \tparam T A fundamental integral or floating-point type.
    template<class T, std::uint32_t N>
    class Vector;

    //=====================================================
    // Aliases for naturally sized vectors
    //=====================================================

    using vecNx64u = Vector<std::uint64_t, AVEL_NATURAL_WIDTH_64U>;
    using vecNx32u = Vector<std::uint32_t, AVEL_NATURAL_WIDTH_32U>;
    using vecNx16u = Vector<std::uint16_t, AVEL_NATURAL_WIDTH_16U>;
    using vecNx8u  = Vector<std::uint8_t,  AVEL_NATURAL_WIDTH_8U>;

    using vecNx64i = Vector<std::int64_t, AVEL_NATURAL_WIDTH_64I>;
    using vecNx32i = Vector<std::int32_t, AVEL_NATURAL_WIDTH_32I>;
    using vecNx16i = Vector<std::int16_t, AVEL_NATURAL_WIDTH_16I>;
    using vecNx8i  = Vector<std::int8_t,  AVEL_NATURAL_WIDTH_8I>;

    using vecNx32f = Vector<float, AVEL_NATURAL_WIDTH_32F>;
    using vecNx64f = Vector<double, AVEL_NATURAL_WIDTH_64F>;

    //=====================================================
    // Aliases for maximum sized vectors
    //=====================================================

    using vecMx64u = Vector<std::uint64_t, AVEL_MAX_WIDTH_64U>;
    using vecMx32u = Vector<std::uint32_t, AVEL_MAX_WIDTH_32U>;
    using vecMx16u = Vector<std::uint16_t, AVEL_MAX_WIDTH_16U>;
    using vecMx8u  = Vector<std::uint8_t,  AVEL_MAX_WIDTH_8U>;

    using vecMx64i = Vector<std::int64_t, AVEL_MAX_WIDTH_64I>;
    using vecMx32i = Vector<std::int32_t, AVEL_MAX_WIDTH_32I>;
    using vecMx16i = Vector<std::int16_t, AVEL_MAX_WIDTH_16I>;
    using vecMx8i  = Vector<std::int8_t,  AVEL_MAX_WIDTH_8I>;

    using vecMx32f = Vector<float, AVEL_MAX_WIDTH_32F>;
    using vecMx64f = Vector<double, AVEL_MAX_WIDTH_64F>;

    //=====================================================
    // Type traits
    //=====================================================

    template<class T>
    struct is_vector : public std::integral_constant<bool, false> {};

    template<class T, std::uint32_t N>
    struct is_vector<Vector<T, N>> : public ::std::integral_constant<bool, true> {};

    #if 201402L <= __cplusplus

    template<class T>
    constexpr bool is_vector_v = is_vector<T>::value;

    #endif




    template<class T>
    struct is_vector_mask : public std::integral_constant<bool, false> {};

    template<class T, std::uint32_t N>
    struct is_vector_mask<Vector_mask<T, N>> : public ::std::integral_constant<bool, true> {};

    #if 201402L <= __cplusplus

    template<class T>
    constexpr bool is_vector_mask_v = is_vector_mask<T>::value;

    #endif

}

#include "impl/vectors/Vectors.hpp"

#if defined(AVEL_SCALAR_DENOMINATOR_HPP)
    #include "impl/vector_reciprocal/Vector_reciprocals.hpp"
#endif

#endif
