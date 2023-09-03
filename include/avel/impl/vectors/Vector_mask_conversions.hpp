#ifndef AVEL_VECTOR_MASK_CONVERSIONS_HPP
#define AVEL_VECTOR_MASK_CONVERSIONS_HPP

#define AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(FROM, TO)     \
    template<>                                                    \
    [[nodiscard]]                                                 \
    AVEL_FINL std::array<TO, 1> convert<TO, FROM, void>(FROM v) { \
        return {TO(decay(v))};                                    \
    }

#define AVEL_SINGE_WIDTH_MASK_TO_M128I_MASK(FROM, TO)                                   \
    template<>                                                                          \
    [[nodiscard]]                                                                       \
    AVEL_FINL std::array<TO, 1> convert<TO, FROM, void>(FROM v) {                       \
        using T = typename std::to_unsigned<avel::to_vector<TO>::type::scalar>type;     \
        std::uint64_t mask_value = avel::set_bits<T>(decay(v));                         \
        return {TO(_mm_set1_epi64x(mask_value))};                                       \
    }

#define AVEL_M128I_MASK_TO_SINGE_WIDTH_MASK(FROM, TO)
    template<>
    [[nodiscard]]
    AVEL_FINL std::array<TO, TO::WIDTH> convert<TO, FROM, void>(FROM v) {
        static_assert(
            sizeof(bool) == 1,
            "Implementation assumes bools occupy a single byte"
        );

        std::array<T0, TO::WIDTH> ret;

        auto mask = _mm_loadu_si128(reinterpret_cast<__m128*>(masks128_table + 16 - FROM::width));

        _mm_maskmoveu_si128(data, mask, reinterpret_cast<char*>(ret.data()));

        return ret;
    }


namespace avel {

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8u, mask1x8i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8i, mask1x8u);



    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8u, mask1x16u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16u, mask1x8u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8i, mask1x16u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16u, mask1x8i);



    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8u, mask1x16i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16i, mask1x8u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8i, mask1x16i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16i, mask1x8i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16u, mask1x16i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16i, mask1x16u);



    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8u, mask1x32u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32u, mask1x8u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8i, mask1x32u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32u, mask1x8i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16u, mask1x32u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32u, mask1x16u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16i, mask1x32u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32u, mask1x16i);



    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8u, mask1x32i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32i, mask1x8u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8i, mask1x32i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32i, mask1x8i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16u, mask1x32i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32i, mask1x16u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16i, mask1x32i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32i, mask1x16i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32u, mask1x32i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32i, mask1x32u);



    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8u, mask1x64u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64u, mask1x8u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8i, mask1x64u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64u, mask1x8i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16u, mask1x64u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64u, mask1x16u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16i, mask1x64u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64u, mask1x16i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32u, mask1x64u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64u, mask1x32u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32i, mask1x64u);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64u, mask1x32i);



    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8u, mask1x64i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64i, mask1x8u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x8i, mask1x64i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64i, mask1x8i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16u, mask1x64i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64i, mask1x16u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x16i, mask1x64i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64i, mask1x16i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32u, mask1x64i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64i, mask1x32u);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x32i, mask1x64i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64i, mask1x32i);

    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64u, mask1x64i);
    AVEL_SINGLE_WIDTH_MASK_TO_SINGLE_WIDTH_MASK(mask1x64i, mask1x64u);





    #if defined(AVEL_SSE2)

    AVEL_SINGE_WIDTH_MASK_TO_M128I_MASK(mask1x8u, mask16x8u);

    #endif




}

#endif //AVEL_VECTOR_MASK_CONVERSIONS_HPP
