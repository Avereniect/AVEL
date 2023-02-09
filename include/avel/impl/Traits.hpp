#ifndef AVEL_IMPL_TRAITS_HPP
#define AVEL_IMPL_TRAITS_HPP

#include <cstdint>

namespace avel {

    template<class P>
    struct to_index_type {
        using type = void;
    };

    template<>
    struct to_index_type<std::uint32_t> {
        using type = std::int32_t;
    };

    template<>
    struct to_index_type<std::int32_t> {
        using type = std::int32_t;
    };

    template<>
    struct to_index_type<float> {
        using type = std::int32_t;
    };



    template<>
    struct to_index_type<std::uint64_t> {
        using type = std::int64_t;
    };

    template<>
    struct to_index_type<std::int64_t> {
        using type = std::int64_t;
    };

    template<>
    struct to_index_type<double> {
        using type = std::int64_t;
    };

}

#endif //AVEL_IMPL_TRAITS_HPP
