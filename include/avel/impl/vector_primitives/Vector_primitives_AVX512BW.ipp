template<>
struct mask_primitive<std::uint8_t, 64> {
    using type = __mmask64;
};

template<>
struct mask_primitive<std::int8_t, 64> {
    using type = __mmask64;
};


template<>
struct mask_primitive<std::uint16_t, 32> {
    using type = __mmask32;
};

template<>
struct mask_primitive<std::int16_t, 32> {
    using type = __mmask32;
};

