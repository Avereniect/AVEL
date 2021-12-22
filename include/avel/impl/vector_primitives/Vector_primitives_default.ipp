template<>
struct vector_primitive<std::uint64_t, 1> {
    using type = std::uint64_t;
};

template<>
struct vector_primitive<std::int64_t, 1> {
    using type = std::int64_t;;
};


template<>
struct vector_primitive<std::uint32_t, 1> {
    using type = std::uint32_t;
};

template<>
struct vector_primitive<std::int32_t, 1> {
    using type = std::int32_t;
};


template<>
struct vector_primitive<std::uint16_t, 1> {
    using type = std::uint16_t;
};

template<>
struct vector_primitive<std::int16_t, 1> {
    using type = std::int16_t;
};


template<>
struct vector_primitive<std::uint8_t, 1> {
    using type = std::uint8_t;
};

template<>
struct vector_primitive<std::int8_t, 1> {
    using type = std::int8_t;
};


template<>
struct vector_primitive<float, 1> {
    using type = float;
};

template<>
struct vector_primitive<double, 1> {
    using type = double;
};
