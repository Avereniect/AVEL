#ifndef AVEL_VECTOR_HPP
#define AVEL_VECTOR_HPP

#if !defined(AVEL_X86) & !defined(AVEL_ARM)
    static_assert(false, "Use of vector classes required an ISA to be specified");
#endif

#include <cstdint>

#include <avel/impl/Capabilities.hpp>
#include "avel/impl/Sizes.hpp"

#include "Scalar.hpp"

namespace avel {

    ///
    /// \tparam T Integral or floating-point type
    /// \tparam N Vector width
    template<class T, int N>
    class Vector_mask;

    ///
    /// A class representing a SIMD register.
    ///
    /// Since this class represents a register, variables of this type should
    /// not be used in large amounts, or for purposes besides .
    ///
    /// \tparam T A fundamental integral or floating-point type.
    template<class T, int N>
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

}

#include "impl/vectors/Vectors.hpp"

#if defined(AVEL_SCALAR_RECIPROCAL_HPP)
    #include "impl/vector_reciprocal/Vector_reciprocals.hpp"
#endif

#endif
