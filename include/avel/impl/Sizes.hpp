#ifndef AVEL_SIZES_HPP
#define AVEL_SIZES_HPP

//=========================================================
// Vectors sizes
//=========================================================

namespace avel {

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

    constexpr std::uint32_t max_width_32f = 4;
    constexpr std::uint32_t max_width_64f = 2;

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

}

#endif //AVEL_SIZES_HPP
