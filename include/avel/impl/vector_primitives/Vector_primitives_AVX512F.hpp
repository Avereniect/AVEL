#ifndef AVEL_VECTOR_PRIMITIVES_AVX512F_HPP
#define AVEL_VECTOR_PRIMITIVES_AVX512F_HPP

//=========================================================
// Vector primitives
//=========================================================

template<>
struct vector_primitive<std::uint64_t, 8> {
    using type = __m512i;
};

template<>
struct vector_primitive<std::int64_t, 8> {
    using type = __m512i;
};


template<>
struct vector_primitive<std::uint32_t, 16> {
    using type = __m512i;
};

template<>
struct vector_primitive<std::int32_t, 16> {
    using type = __m512i;
};


template<>
struct vector_primitive<std::uint16_t, 32> {
    using type = __m512i;
};

template<>
struct vector_primitive<std::int16_t, 32> {
    using type = __m512i;
};


template<>
struct vector_primitive<std::uint8_t, 64> {
    using type = __m512i;
};

template<>
struct vector_primitive<std::int8_t, 64> {
    using type = __m512i;
};


template<>
struct vector_primitive<float, 16> {
    using type = __m512;
};

template<>
struct vector_primitive<double, 8> {
    using type = __m512d;
};

//=========================================================
// Mask primitives
//=========================================================

#if !defined(AVEL_AVX512BW)

template<>
struct mask_primitive<std::uint8_t, 64> {
    using type = std::uint64_t;
};

template<>
struct mask_primitive<std::int8_t, 64> {
    using type = std::uint64_t;
};


template<>
struct mask_primitive<std::uint16_t, 32> {
    using type = std::uint32_t;
};

template<>
struct mask_primitive<std::int16_t, 32> {
    using type = std::uint32_t;
};

#endif


template<>
struct mask_primitive<std::uint32_t, 16> {
    using type = __mmask16;
};

template<>
struct mask_primitive<std::int32_t, 16> {
    using type = __mmask16;
};


template<>
struct mask_primitive<std::uint64_t, 8> {
    using type = __mmask8;
};

template<>
struct mask_primitive<std::int64_t, 8> {
    using type = __mmask8;
};


template<>
struct mask_primitive<float, 16> {
    using type = __mmask16;
};

template<>
struct mask_primitive<double, 32> {
    using type = __mmask32;
};

#endif //AVEL_VECTOR_PRIMITIVES_AVX512F_HPP
