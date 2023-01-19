#ifndef AVEL_VECTOR_HPP
#define AVEL_VECTOR_HPP

#include <cstdint>

#include "./impl/Capabilities.hpp"
#include "./impl/Sizes.hpp"

#include "Scalar.hpp"

namespace avel {

    ///
    /// A class representing a mask corresponding to Vector<T, N>. Essentially a
    /// vector of booleans in certain operations on vectors of type Vector<T, N>
    ///
    /// \tparam T A fundamental integral or floating-point type.
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
    /// \tparam N Vector width
    template<class T, std::uint32_t N>
    class Vector;

    //=====================================================
    // Aliases for naturally sized vectors
    //=====================================================

    using vecNx64u = Vector<std::uint64_t, natural_width_64u>;
    using vecNx32u = Vector<std::uint32_t, natural_width_32u>;
    using vecNx16u = Vector<std::uint16_t, natural_width_16u>;
    using vecNx8u  = Vector<std::uint8_t,  natural_width_8u>;

    using vecNx64i = Vector<std::int64_t, natural_width_64i>;
    using vecNx32i = Vector<std::int32_t, natural_width_32i>;
    using vecNx16i = Vector<std::int16_t, natural_width_16i>;
    using vecNx8i  = Vector<std::int8_t,  natural_width_8i>;

    using vecNx32f = Vector<float, natural_width_32f>;
    using vecNx64f = Vector<double, natural_width_64f>;

    //=====================================================
    // Aliases for maximum sized vectors
    //=====================================================

    using vecMx64u = Vector<std::uint64_t, natural_width_64u>;
    using vecMx32u = Vector<std::uint32_t, natural_width_32u>;
    using vecMx16u = Vector<std::uint16_t, natural_width_16u>;
    using vecMx8u  = Vector<std::uint8_t,  natural_width_8u>;

    using vecMx64i = Vector<std::int64_t, natural_width_64i>;
    using vecMx32i = Vector<std::int32_t, natural_width_32i>;
    using vecMx16i = Vector<std::int16_t, natural_width_16i>;
    using vecMx8i  = Vector<std::int8_t,  natural_width_8i>;

    using vecMx32f = Vector<float, natural_width_32f>;
    using vecMx64f = Vector<double, natural_width_64f>;

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
