#ifndef AVEL_IMPL_TRAITS_HPP
#define AVEL_IMPL_TRAITS_HPP

#include <cstdint>
#include <type_traits>

namespace avel {

    //=====================================================
    // to_index_type trait
    //=====================================================

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

    //=====================================================
    // Queries
    //=====================================================

    template<class V>
    struct is_unsigned_vector : public std::integral_constant<bool, avel::is_vector<V>::value && std::is_unsigned<typename V::scalar>::value> {};

    #if 201402L <= __cplusplus

    template<class V>
    constexpr bool is_unsigned_vector_v = is_unsigned_vector<V>::value;

    #endif



    template<class V>
    struct is_signed_vector : public std::integral_constant<bool, avel::is_vector<V>::value && std::is_signed<typename V::scalar>::value> {};

    #if 201402L <= __cplusplus

    template<class V>
    constexpr bool is_signed_vector_v = is_signed_vector<V>::value;

    #endif



    template<class V>
    struct is_integer_vector : public std::integral_constant<bool, avel::is_vector<V>::value && std::is_integral<typename V::scalar>::value> {};

    #if 201402L <= __cplusplus

    template<class V>
    constexpr bool is_integer_vector_v = is_integer_vector<V>::value;

    #endif



    template<class V>
    struct is_float_vector : public std::integral_constant<bool, avel::is_vector<V>::value && std::is_floating_point<typename V::scalar>::value> {};

    #if 201402L <= __cplusplus

    template<class V>
    constexpr bool is_float_vector_V = is_float_vector<V>::value;

    #endif

    //=====================================================
    // SFINAE helpers
    //=====================================================

    template<class T>
    struct enable_if_scalar {
        using type = typename std::enable_if<avel::is_vector<T>::value>::type;
    };

    template<class T>
    using enable_if_scalar_t = typename std::enable_if<avel::is_vector<T>::value>::type;



    template<class V>
    struct enable_if_vector {
        using type = typename std::enable_if<avel::is_vector<V>::value>::type;
    };

    template<class V>
    using enable_if_vector_t = typename std::enable_if<avel::is_vector<V>::value>::type;



    template<class V>
    struct enable_if_vector_mask {
        using type = typename std::enable_if<avel::is_vector<V>::value>::type;
    };

    template<class V>
    using enable_if_vector_mask_t = typename std::enable_if<avel::is_vector<V>::value>::type;



    template<class V>
    struct enable_if_vector_or_vector_mask {
        using type = typename std::enable_if<avel::is_vector<V>::value || avel::is_vector_mask<V>::value>::type;
    };

    template<class V>
    using enable_if_vector_or_vector_mask_t = typename std::enable_if<avel::is_vector<V>::value || avel::is_vector_mask<V>::value>::type;



    template<class V>
    struct enable_if_unsigned_vector {
        using type = typename std::enable_if<avel::is_unsigned_vector<V>::value>::type;
    };

    template<class V>
    using enable_if_unsigned_vector_t = typename std::enable_if<avel::is_unsigned_vector<V>::value>::type;



    template<class V>
    struct enable_if_signed_vector {
        using type = typename std::enable_if<avel::is_signed_vector<V>::value>::type;
    };

    template<class V>
    using enable_if_signed_vector_t = typename std::enable_if<avel::is_signed_vector<V>::value>::type;



    template<class V>
    struct enable_if_integer_vector {
        using type = typename std::enable_if<avel::is_integer_vector<V>::value>::type;
    };

    template<class V>
    using enable_if_integer_vector_t = typename std::enable_if<avel::is_integer_vector<V>::value>::type;

}

#endif //AVEL_IMPL_TRAITS_HPP
