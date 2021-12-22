//=========================================================
// Vector primitives
//=========================================================

template<>
struct vector_primitive<std::uint64_t, 4> {
    using type = __m256i;
};

template<>
struct vector_primitive<std::int64_t, 4> {
    using type = __m256i;
};


template<>
struct vector_primitive<std::uint32_t, 8> {
    using type = __m256i;
};

template<>
struct vector_primitive<std::int32_t, 8> {
    using type = __m256i;
};


template<>
struct vector_primitive<std::uint16_t, 16> {
    using type = __m256i;
};

template<>
struct vector_primitive<std::int16_t, 16> {
    using type = __m256i;
};


template<>
struct vector_primitive<std::uint8_t, 32> {
    using type = __m256i;
};

template<>
struct vector_primitive<std::int8_t, 32> {
    using type = __m256i;
};


template<>
struct vector_primitive<float, 8> {
    using type = __m256;
};

template<>
struct vector_primitive<double, 4> {
    using type = __m256d;
};

//=========================================================
// Mask primitives
//=========================================================

template<>
struct mask_primitive<std::uint8_t, 32> {
    using type = __m256i;
};

template<>
struct mask_primitive<std::int8_t, 32> {
    using type = __m256i;
};


template<>
struct mask_primitive<std::uint16_t, 16> {
    using type = __m256i;
};

template<>
struct mask_primitive<std::int16_t, 16> {
    using type = __m256i;
};


template<>
struct mask_primitive<std::uint32_t, 8> {
    using type = __m256i;
};

template<>
struct mask_primitive<std::int32_t, 8> {
    using type = __m256i;
};


template<>
struct mask_primitive<std::uint64_t, 4> {
    using type = __m256i;
};

template<>
struct mask_primitive<std::int64_t, 4> {
    using type = __m256i;
};


template<>
struct mask_primitive<float, 8> {
    using type = __m256;
};

template<>
struct mask_primitive<double, 16> {
    using type = __m256d;
};
