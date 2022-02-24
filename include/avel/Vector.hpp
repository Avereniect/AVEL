#ifndef AVEL_VECTOR_HPP
#define AVEL_VECTOR_HPP

#include <cstdint>

#include <avel/impl/Capabilities.hpp>
#include <avel/impl/sizes/Sizes.hpp>
#include <avel/impl/vector_primitives/Vector_primitives.hpp>

namespace avel {

    ///
    /// \tparam T Integral or floating-point type
    /// \tparam N Vector width
    template<class T, int N = avel::optimal_vector_width<T>::value>
    class alignas(alignof(typename avel::mask_primitive<T, N>::type)) Vector_mask{};

    ///
    /// A class representing a SIMD register.
    ///
    /// Since this class represents a register, variables of this type should
    /// not be used in large amounts, or for purposes besides .
    ///
    /// \tparam T A fundamental integral or floating-point type.
    template<class T, int N = avel::optimal_vector_width<T>::value>
    class alignas(N * sizeof(T)) Vector;

    //---------------------------------
    // Optimal Aliases
    //---------------------------------

    using vec64i = Vector<std::int64_t>;
    using vec32i = Vector<std::int32_t>;
    using vec16i = Vector<std::int16_t>;
    using vec8i  = Vector<std::int8_t>;

    using vec64u = Vector<std::uint64_t>;
    using vec32u = Vector<std::uint32_t>;
    using vec16u = Vector<std::uint16_t>;
    using vec8u  = Vector<std::uint8_t>;

    using vec32f = Vector<float>;
    using vec64f = Vector<double>;

}

#include "impl/vectors/Vectors.hpp"

#endif
