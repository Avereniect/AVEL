#ifndef AVEL_VECTOR_HPP
#define AVEL_VECTOR_HPP

#include <cstdint>
#include <array>

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

    //using vecNx32f = Vector<float, natural_width_32f>;
    //using vecNx64f = Vector<double, natural_width_64f>;



    using maskNx64u = Vector_mask<std::uint64_t, natural_width_64u>;
    using maskNx32u = Vector_mask<std::uint32_t, natural_width_32u>;
    using maskNx16u = Vector_mask<std::uint16_t, natural_width_16u>;
    using maskNx8u  = Vector_mask<std::uint8_t,  natural_width_8u>;

    using maskNx64i = Vector_mask<std::int64_t, natural_width_64i>;
    using maskNx32i = Vector_mask<std::int32_t, natural_width_32i>;
    using maskNx16i = Vector_mask<std::int16_t, natural_width_16i>;
    using maskNx8i  = Vector_mask<std::int8_t,  natural_width_8i>;

    //using maskNx32f = Vector_mask<float, natural_width_32f>;
    //using maskNx64f = Vector_mask<double, natural_width_64f>;



    using arrNx64u = std::array<std::uint64_t, natural_width_64u>;
    using arrNx32u = std::array<std::uint32_t, natural_width_32u>;
    using arrNx16u = std::array<std::uint16_t, natural_width_16u>;
    using arrNx8u  = std::array<std::uint8_t,  natural_width_8u>;

    using arrNx64i = std::array<std::int64_t, natural_width_64i>;
    using arrNx32i = std::array<std::int32_t, natural_width_32i>;
    using arrNx16i = std::array<std::int16_t, natural_width_16i>;
    using arrNx8i  = std::array<std::int8_t,  natural_width_8i>;

    //using arrNx32f = std::array<float, natural_width_32f>;
    //using arrNx64f = std::array<double, natural_width_64f>;

    //=====================================================
    // Aliases for maximum sized vectors
    //=====================================================

    using vecMx64u = Vector<std::uint64_t, max_width_64u>;
    using vecMx32u = Vector<std::uint32_t, max_width_32u>;
    using vecMx16u = Vector<std::uint16_t, max_width_16u>;
    using vecMx8u  = Vector<std::uint8_t,  max_width_8u>;

    using vecMx64i = Vector<std::int64_t, max_width_64i>;
    using vecMx32i = Vector<std::int32_t, max_width_32i>;
    using vecMx16i = Vector<std::int16_t, max_width_16i>;
    using vecMx8i  = Vector<std::int8_t,  max_width_8i>;

    //using vecMx32f = Vector<float, max_width_32f>;
    //using vecMx64f = Vector<double, max_width_64f>;



    using maskMx64u = Vector_mask<std::uint64_t, max_width_64u>;
    using maskMx32u = Vector_mask<std::uint32_t, max_width_32u>;
    using maskMx16u = Vector_mask<std::uint16_t, max_width_16u>;
    using maskMx8u  = Vector_mask<std::uint8_t,  max_width_8u>;

    using maskMx64i = Vector_mask<std::int64_t, max_width_64i>;
    using maskMx32i = Vector_mask<std::int32_t, max_width_32i>;
    using maskMx16i = Vector_mask<std::int16_t, max_width_16i>;
    using maskMx8i  = Vector_mask<std::int8_t,  max_width_8i>;

    //using maskMx32f = Vector_mask<float, max_width_32f>;
    //using maskMx64f = Vector_mask<double, max_width_64f>;



    using arrMx64u = std::array<std::uint64_t, max_width_64u>;
    using arrMx32u = std::array<std::uint32_t, max_width_32u>;
    using arrMx16u = std::array<std::uint16_t, max_width_16u>;
    using arrMx8u  = std::array<std::uint8_t,  max_width_8u>;

    using arrMx64i = std::array<std::int64_t, max_width_64i>;
    using arrMx32i = std::array<std::int32_t, max_width_32i>;
    using arrMx16i = std::array<std::int16_t, max_width_16i>;
    using arrMx8i  = std::array<std::int8_t,  max_width_8i>;

    //using arrMx32f = std::array<float, max_width_32f>;
    //using arrMx64f = std::array<double, max_width_64f>;

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
