#ifndef AVEL_SIZES_HPP
#define AVEL_SIZES_HPP

//=========================================================
// Vectors sizes
//=========================================================

namespace avel {

/* //TODO: Uncomment as wider vectors are implemented
#if defined(AVEL_AVX512BW)
    constexpr std::uint32_t natural_width_8u  = 64;
    constexpr std::uint32_t natural_width_16u = 32;
    constexpr std::uint32_t natural_width_32u = 16;
    constexpr std::uint32_t natural_width_64u = 8;

    constexpr std::uint32_t natural_width_8i  = 64;
    constexpr std::uint32_t natural_width_16i = 32;
    constexpr std::uint32_t natural_width_32i = 16;
    constexpr std::uint32_t natural_width_64i = 8;

    constexpr std::uint32_t natural_width_32f = 16;
    constexpr std::uint32_t natural_width_64f = 8;



    constexpr std::uint32_t max_width_8u  = 64;
    constexpr std::uint32_t max_width_16u = 32;
    constexpr std::uint32_t max_width_32u = 16;
    constexpr std::uint32_t max_width_64u = 8;

    constexpr std::uint32_t max_width_8i  = 64;
    constexpr std::uint32_t max_width_16i = 32;
    constexpr std::uint32_t max_width_32i = 16;
    constexpr std::uint32_t max_width_64i = 8;

    constexpr std::uint32_t max_width_32f = 16;
    constexpr std::uint32_t max_width_64f = 8;

#elif defined(AVEL_AVX512F)
    constexpr std::uint32_t natural_width_8u  = 32;
    constexpr std::uint32_t natural_width_16u = 16;
    constexpr std::uint32_t natural_width_32u = 16;
    constexpr std::uint32_t natural_width_64u = 8;

    constexpr std::uint32_t natural_width_8i  = 32;
    constexpr std::uint32_t natural_width_16i = 16;
    constexpr std::uint32_t natural_width_32i = 16;
    constexpr std::uint32_t natural_width_64i = 8;

    constexpr std::uint32_t natural_width_32f = 16;
    constexpr std::uint32_t natural_width_64f = 8;



    constexpr std::uint32_t max_width_8u  = 64;
    constexpr std::uint32_t max_width_16u = 32;
    constexpr std::uint32_t max_width_32u = 16;
    constexpr std::uint32_t max_width_64u = 8;

    constexpr std::uint32_t max_width_8i  = 64;
    constexpr std::uint32_t max_width_16i = 32;
    constexpr std::uint32_t max_width_32i = 16;
    constexpr std::uint32_t max_width_64i = 8;

    constexpr std::uint32_t max_width_32f = 16;
    constexpr std::uint32_t max_width_64f = 8;

#elif defined(AVEL_AVX2)
    constexpr std::uint32_t natural_width_8u  = 32;
    constexpr std::uint32_t natural_width_16u = 16;
    constexpr std::uint32_t natural_width_32u = 8;
    constexpr std::uint32_t natural_width_64u = 4;

    constexpr std::uint32_t natural_width_8i  = 32;
    constexpr std::uint32_t natural_width_16i = 16;
    constexpr std::uint32_t natural_width_32i = 8;
    constexpr std::uint32_t natural_width_64i = 4;

    constexpr std::uint32_t natural_width_32f = 8;
    constexpr std::uint32_t natural_width_64f = 4;



    constexpr std::uint32_t max_width_8u  = 32;
    constexpr std::uint32_t max_width_16u = 16;
    constexpr std::uint32_t max_width_32u = 8;
    constexpr std::uint32_t max_width_64u = 4;

    constexpr std::uint32_t max_width_8i  = 32;
    constexpr std::uint32_t max_width_16i = 16;
    constexpr std::uint32_t max_width_32i = 8;
    constexpr std::uint32_t max_width_64i = 4;

    constexpr std::uint32_t max_width_32f = 8;
    constexpr std::uint32_t max_width_64f = 4;

#elif defined(AVEL_AVX)
    constexpr std::uint32_t natural_width_8u = 16;
    constexpr std::uint32_t natural_width_16u = 8;
    constexpr std::uint32_t natural_width_32u = 4;
    constexpr std::uint32_t natural_width_64u = 2;

    constexpr std::uint32_t natural_width_8i = 16;
    constexpr std::uint32_t natural_width_16i = 8;
    constexpr std::uint32_t natural_width_32i = 4;
    constexpr std::uint32_t natural_width_64i = 2;

    constexpr std::uint32_t natural_width_32f = 8;
    constexpr std::uint32_t natural_width_64f = 4;



    constexpr std::uint32_t max_width_8u  = 32;
    constexpr std::uint32_t max_width_16u = 16;
    constexpr std::uint32_t max_width_32u = 8;
    constexpr std::uint32_t max_width_64u = 4;

    constexpr std::uint32_t max_width_8i  = 32;
    constexpr std::uint32_t max_width_16i = 16;
    constexpr std::uint32_t max_width_32i = 8;
    constexpr std::uint32_t max_width_64i = 4;

    constexpr std::uint32_t max_width_32f = 8;
    constexpr std::uint32_t max_width_64f = 4;

#elif defined(AVEL_SSE2) || defined(AVEL_NEON)
 */

#if defined(AVEL_SSE2) || defined(AVEL_NEON)
    constexpr std::uint32_t natural_width_8u = 16;
    constexpr std::uint32_t natural_width_16u = 8;
    constexpr std::uint32_t natural_width_32u = 4;
    constexpr std::uint32_t natural_width_64u = 2;

    constexpr std::uint32_t natural_width_8i = 16;
    constexpr std::uint32_t natural_width_16i = 8;
    constexpr std::uint32_t natural_width_32i = 4;
    constexpr std::uint32_t natural_width_64i = 2;

    constexpr std::uint32_t natural_width_32f = 4;
    constexpr std::uint32_t natural_width_64f = 2;



    constexpr std::uint32_t max_width_8u = 16;
    constexpr std::uint32_t max_width_16u = 8;
    constexpr std::uint32_t max_width_32u = 4;
    constexpr std::uint32_t max_width_64u = 2;

    constexpr std::uint32_t max_width_8i = 16;
    constexpr std::uint32_t max_width_16i = 8;
    constexpr std::uint32_t max_width_32i = 4;
    constexpr std::uint32_t max_width_64i = 2;

    //constexpr std::uint32_t max_width_32f = 4;
    //constexpr std::uint32_t max_width_64f = 2;

#endif

#if !defined(AVEL_SSE2) && !defined(AVEL_NEON)
    constexpr std::uint32_t natural_width_8u = 1;
    constexpr std::uint32_t natural_width_16u = 1;
    constexpr std::uint32_t natural_width_32u = 1;
    constexpr std::uint32_t natural_width_64u = 1;

    constexpr std::uint32_t natural_width_8i = 1;
    constexpr std::uint32_t natural_width_16i = 1;
    constexpr std::uint32_t natural_width_32i = 1;
    constexpr std::uint32_t natural_width_64i = 1;

    constexpr std::uint32_t natural_width_32f = 1;
    constexpr std::uint32_t natural_width_64f = 1;



    constexpr std::uint32_t max_width_8u = 1;
    constexpr std::uint32_t max_width_16u = 1;
    constexpr std::uint32_t max_width_32u = 1;
    constexpr std::uint32_t max_width_64u = 1;

    constexpr std::uint32_t max_width_8i = 1;
    constexpr std::uint32_t max_width_16i = 1;
    constexpr std::uint32_t max_width_32i = 1;
    constexpr std::uint32_t max_width_64i = 1;

    constexpr std::uint32_t max_width_32f = 1;
    constexpr std::uint32_t max_width_64f = 1;
#endif

    //=====================================================
    // Template interface
    //=====================================================

    //=================================
    // Natural widths
    //=================================

    template<class T>
    struct natural_vector_width {
        static constexpr std::uint32_t value = 0;
    };



    template<>
    struct natural_vector_width<std::uint8_t> {
        static constexpr std::uint32_t value = natural_width_8u;
    };

    template<>
    struct natural_vector_width<std::uint16_t> {
        static constexpr std::uint32_t value = natural_width_16u;
    };

    template<>
    struct natural_vector_width<std::uint32_t> {
        static constexpr std::uint32_t value = natural_width_32u;
    };

    template<>
    struct natural_vector_width<std::uint64_t> {
        static constexpr std::uint32_t value = natural_width_64u;
    };



    template<>
    struct natural_vector_width<std::int8_t> {
        static constexpr std::uint32_t value = natural_width_8u;
    };

    template<>
    struct natural_vector_width<std::int16_t> {
        static constexpr std::uint32_t value = natural_width_16u;
    };

    template<>
    struct natural_vector_width<std::int32_t> {
        static constexpr std::uint32_t value = natural_width_32u;
    };

    template<>
    struct natural_vector_width<std::int64_t> {
        static constexpr std::uint32_t value = natural_width_64u;
    };



    #if 201402L <= __cplusplus

    template<class T>
    constexpr bool natural_vector_width_v = natural_vector_width<T>::value;

    #endif

    //=================================
    // Maximum widths
    //=================================


    template<class T>
    struct maximum_vector_width {
        static constexpr std::uint32_t value = 0;
    };



    template<>
    struct maximum_vector_width<std::uint8_t> {
        static constexpr std::uint32_t value = max_width_8u;
    };

    template<>
    struct maximum_vector_width<std::uint16_t> {
        static constexpr std::uint32_t value = max_width_16u;
    };

    template<>
    struct maximum_vector_width<std::uint32_t> {
        static constexpr std::uint32_t value = max_width_32u;
    };

    template<>
    struct maximum_vector_width<std::uint64_t> {
        static constexpr std::uint32_t value = max_width_64u;
    };



    template<>
    struct maximum_vector_width<std::int8_t> {
        static constexpr std::uint32_t value = max_width_8u;
    };

    template<>
    struct maximum_vector_width<std::int16_t> {
        static constexpr std::uint32_t value = max_width_16u;
    };

    template<>
    struct maximum_vector_width<std::int32_t> {
        static constexpr std::uint32_t value = max_width_32u;
    };

    template<>
    struct maximum_vector_width<std::int64_t> {
        static constexpr std::uint32_t value = max_width_64u;
    };



    #if 201402L <= __cplusplus

    template<class T>
    constexpr bool maximum_vector_width_v = maximum_vector_width<T>::value;

    #endif


}

#endif //AVEL_SIZES_HPP
