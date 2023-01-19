#ifndef AVEL_SIZES_HPP
#define AVEL_SIZES_HPP

//=========================================================
// Vectors sizes
//=========================================================

#if defined(AVEL_AVX512BW)
    #define AVEL_NATURAL_WIDTH_8U 64
    #define AVEL_NATURAL_WIDTH_16U 32
    #define AVEL_NATURAL_WIDTH_32U 16
    #define AVEL_NATURAL_WIDTH_64U 8

    #define AVEL_NATURAL_WIDTH_8I 32
    #define AVEL_NATURAL_WIDTH_16I 16
    #define AVEL_NATURAL_WIDTH_32I 16
    #define AVEL_NATURAL_WIDTH_64I 8

    #define AVEL_NATURAL_WIDTH_32F 16
    #define AVEL_NATURAL_WIDTH_64F 8



    #define AVEL_MAX_WIDTH_8U 64
    #define AVEL_MAX_WIDTH_16U 32
    #define AVEL_MAX_WIDTH_32U 16
    #define AVEL_MAX_WIDTH_64U 8

    #define AVEL_MAX_WIDTH_8I 64
    #define AVEL_MAX_WIDTH_16I 32
    #define AVEL_MAX_WIDTH_32I 16
    #define AVEL_MAX_WIDTH_64I 8

    #define AVEL_MAX_WIDTH_32F 16
    #define AVEL_MAX_WIDTH_64F 8

#elif defined(AVEL_AVX512F)
    #define AVEL_NATURAL_WIDTH_8U 32
    #define AVEL_NATURAL_WIDTH_16U 16
    #define AVEL_NATURAL_WIDTH_32U 16
    #define AVEL_NATURAL_WIDTH_64U 8

    #define AVEL_NATURAL_WIDTH_8I 32
    #define AVEL_NATURAL_WIDTH_16I 16
    #define AVEL_NATURAL_WIDTH_32I 16
    #define AVEL_NATURAL_WIDTH_64I 8

    #define AVEL_NATURAL_WIDTH_32F 16
    #define AVEL_NATURAL_WIDTH_64F 8



    #define AVEL_MAX_WIDTH_8U 64
    #define AVEL_MAX_WIDTH_16U 32
    #define AVEL_MAX_WIDTH_32U 16
    #define AVEL_MAX_WIDTH_64U 8

    #define AVEL_MAX_WIDTH_8I 64
    #define AVEL_MAX_WIDTH_16I 32
    #define AVEL_MAX_WIDTH_32I 16
    #define AVEL_MAX_WIDTH_64I 8

    #define AVEL_MAX_WIDTH_32F 16
    #define AVEL_MAX_WIDTH_64F 8

#elif defined(AVEL_AVX2)
    #define AVEL_NATURAL_WIDTH_8U 32
    #define AVEL_NATURAL_WIDTH_16U 16
    #define AVEL_NATURAL_WIDTH_32U 8
    #define AVEL_NATURAL_WIDTH_64U 4

    #define AVEL_NATURAL_WIDTH_8I 32
    #define AVEL_NATURAL_WIDTH_16I 16
    #define AVEL_NATURAL_WIDTH_32I 8
    #define AVEL_NATURAL_WIDTH_64I 4

    #define AVEL_NATURAL_WIDTH_32F 8
    #define AVEL_NATURAL_WIDTH_64F 4



    #define AVEL_MAX_WIDTH_8U 32
    #define AVEL_MAX_WIDTH_16U 16
    #define AVEL_MAX_WIDTH_32U 8
    #define AVEL_MAX_WIDTH_64U 4

    #define AVEL_MAX_WIDTH_8I 32
    #define AVEL_MAX_WIDTH_16I 16
    #define AVEL_MAX_WIDTH_32I 8
    #define AVEL_MAX_WIDTH_64I 4

    #define AVEL_MAX_WIDTH_32F 8
    #define AVEL_MAX_WIDTH_64F 4

#elif defined(AVEL_AVX)
    #define AVEL_NATURAL_WIDTH_8U 16
    #define AVEL_NATURAL_WIDTH_16U 8
    #define AVEL_NATURAL_WIDTH_32U 4
    #define AVEL_NATURAL_WIDTH_64U 2

    #define AVEL_NATURAL_WIDTH_8I 16
    #define AVEL_NATURAL_WIDTH_16I 8
    #define AVEL_NATURAL_WIDTH_32I 4
    #define AVEL_NATURAL_WIDTH_64I 2

    #define AVEL_NATURAL_WIDTH_32F 4
    #define AVEL_NATURAL_WIDTH_64F 2



    #define AVEL_MAX_WIDTH_8U 32
    #define AVEL_MAX_WIDTH_16U 16
    #define AVEL_MAX_WIDTH_32U 8
    #define AVEL_MAX_WIDTH_64U 4

    #define AVEL_MAX_WIDTH_8I 32
    #define AVEL_MAX_WIDTH_16I 16
    #define AVEL_MAX_WIDTH_32I 8
    #define AVEL_MAX_WIDTH_64I 4

    #define AVEL_MAX_WIDTH_32F 8
    #define AVEL_MAX_WIDTH_64F 4

#elif defined(AVEL_SSE2) || defined(AVEL_NEON)
    #define AVEL_NATURAL_WIDTH_8U 16
    #define AVEL_NATURAL_WIDTH_16U 8
    #define AVEL_NATURAL_WIDTH_32U 4
    #define AVEL_NATURAL_WIDTH_64U 2

    #define AVEL_NATURAL_WIDTH_8I 16
    #define AVEL_NATURAL_WIDTH_16I 8
    #define AVEL_NATURAL_WIDTH_32I 4
    #define AVEL_NATURAL_WIDTH_64I 2

    #define AVEL_NATURAL_WIDTH_32F 4
    #define AVEL_NATURAL_WIDTH_64F 2



    #define AVEL_MAX_WIDTH_8U 16
    #define AVEL_MAX_WIDTH_16U 8
    #define AVEL_MAX_WIDTH_32U 4
    #define AVEL_MAX_WIDTH_64U 2

    #define AVEL_MAX_WIDTH_8I 16
    #define AVEL_MAX_WIDTH_16I 8
    #define AVEL_MAX_WIDTH_32I 4
    #define AVEL_MAX_WIDTH_64I 2

    #define AVEL_MAX_WIDTH_32F 4
    #define AVEL_MAX_WIDTH_64F 2

#endif

#if !defined(AVEL_SSE2) && !defined(AVEL_NEON)
    #define AVEL_NATURAL_WIDTH_8U 1
    #define AVEL_NATURAL_WIDTH_16U 1
    #define AVEL_NATURAL_WIDTH_32U 1
    #define AVEL_NATURAL_WIDTH_64U 1

    #define AVEL_NATURAL_WIDTH_8I 1
    #define AVEL_NATURAL_WIDTH_16I 1
    #define AVEL_NATURAL_WIDTH_32I 1
    #define AVEL_NATURAL_WIDTH_64I 1

    #define AVEL_NATURAL_WIDTH_32F 1
    #define AVEL_NATURAL_WIDTH_64F 1



    #define AVEL_MAX_WIDTH_8U 1
    #define AVEL_MAX_WIDTH_16U 1
    #define AVEL_MAX_WIDTH_32U 1
    #define AVEL_MAX_WIDTH_64U 1

    #define AVEL_MAX_WIDTH_8I 1
    #define AVEL_MAX_WIDTH_16I 1
    #define AVEL_MAX_WIDTH_32I 1
    #define AVEL_MAX_WIDTH_64I 1

    #define AVEL_MAX_WIDTH_32F 1
    #define AVEL_MAX_WIDTH_64F 1
#endif

#endif //AVEL_SIZES_HPP
