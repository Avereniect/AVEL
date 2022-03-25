//=====================================================
// Optimal vector widths
//=====================================================

template<>
struct optimal_vector_width<std::uint8_t> : public std::integral_constant<int, 1>{};

template<>
struct optimal_vector_width<std::int8_t> : public std::integral_constant<int, 1>{};


template<>
struct optimal_vector_width<std::uint16_t> : public std::integral_constant<int, 1>{};

template<>
struct optimal_vector_width<std::int16_t> : public std::integral_constant<int, 1>{};


template<>
struct optimal_vector_width<std::uint32_t> : public std::integral_constant<int, 1>{};

template<>
struct optimal_vector_width<std::int32_t> : public std::integral_constant<int, 1>{};


template<>
struct optimal_vector_width<std::uint64_t> : public std::integral_constant<int, 1>{};

template<>
struct optimal_vector_width<std::int64_t> : public std::integral_constant<int, 1>{};


template<>
struct optimal_vector_width<float> : public std::integral_constant<int, 1>{};

template<>
struct optimal_vector_width<double> : public std::integral_constant<int, 1>{};

//=====================================================
// Maximum vector widths
//=====================================================

template<>
struct max_vector_width<std::uint8_t> : public std::integral_constant<int, 1>{};

template<>
struct max_vector_width<std::int8_t> : public std::integral_constant<int, 1>{};


template<>
struct max_vector_width<std::uint16_t> : public std::integral_constant<int, 1>{};

template<>
struct max_vector_width<std::int16_t> : public std::integral_constant<int, 1>{};


template<>
struct max_vector_width<std::uint32_t> : public std::integral_constant<int, 1>{};

template<>
struct max_vector_width<std::int32_t> : public std::integral_constant<int, 1>{};


template<>
struct max_vector_width<std::uint64_t> : public std::integral_constant<int, 1>{};

template<>
struct max_vector_width<std::int64_t> : public std::integral_constant<int, 1>{};


template<>
struct max_vector_width<float> : public std::integral_constant<int, 1>{};

template<>
struct max_vector_width<double> : public std::integral_constant<int, 1>{};
