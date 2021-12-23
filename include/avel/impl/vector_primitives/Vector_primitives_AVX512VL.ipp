//=========================================================
// Mask primitives
//=========================================================

template<>
struct mask_primitive<std::uint8_t, 16> {
    using type = __mmask16;
};

template<>
struct mask_primitive<std::int8_t, 16> {
    using type = __mmask16;
};


template<>
struct mask_primitive<std::uint16_t, 8> {
    using type = __mmask8;
};

template<>
struct mask_primitive<std::int16_t, 8> {
    using type = __mmask8;
};


template<>
struct mask_primitive<std::uint32_t, 4> {
    using type = __mmask8;
};

template<>
struct mask_primitive<std::int32_t, 4> {
    using type = __mmask8;
};


template<>
struct mask_primitive<std::uint64_t, 2> {
    using type = __mmask8;
};

template<>
struct mask_primitive<std::int64_t, 2> {
    using type = __mmask8;
};


template<>
struct mask_primitive<float, 4> {
    using type = __mmask8;
};

template<>
struct mask_primitive<double, 2> {
    using type = __mmask8;
};





template<>
struct mask_primitive<std::uint8_t, 32> {
    using type = __mmask32;
};

template<>
struct mask_primitive<std::int8_t, 32> {
    using type = __mmask32;
};


template<>
struct mask_primitive<std::uint16_t, 16> {
    using type = __mmask16;
};

template<>
struct mask_primitive<std::int16_t, 16> {
    using type = __mmask16;
};


template<>
struct mask_primitive<std::uint32_t, 8> {
    using type = __mmask8;
};

template<>
struct mask_primitive<std::int32_t, 8> {
    using type = __mmask8;
};


template<>
struct mask_primitive<std::uint64_t, 4> {
    using type = __mmask8;
};

template<>
struct mask_primitive<std::int64_t, 4> {
    using type = __mmask8;
};


template<>
struct mask_primitive<float, 8> {
    using type = __mmask8;
};

template<>
struct mask_primitive<double, 4> {
    using type = __mmask8;
};

