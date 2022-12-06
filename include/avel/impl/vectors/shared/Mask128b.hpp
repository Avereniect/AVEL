#ifndef AVEL_MASK128B_HPP
#define AVEL_MASK128B_HPP

namespace avel_impl {

    class alignas(16) Vector_mask128b {
    public:

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_SSE2)
        using primitive = __m128i;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint8x16_t;
        #endif

    protected:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector_mask128b(bool b):
        #if defined(AVEL_SSE2)
            content(b ? _mm_set1_epi32(-1) : _mm_setzero_si128()) {}
        #endif
        #if defined(AVEL_NEON)
            content(vmovq_n_u8(b ? -1 : 0)) {}
        #endif

        AVEL_FINL explicit Vector_mask128b(primitive content):
            content(content) {}

        Vector_mask128b() = default;
        Vector_mask128b(const Vector_mask128b&) = default;
        Vector_mask128b(Vector_mask128b&&) = default;
        ~Vector_mask128b() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask128b& operator=(bool b) {
            #if defined(AVEL_SSE2)
            content = b ?  _mm_set1_epi32(-1) : _mm_setzero_si128();
            #endif

            #if defined(AVEL_NEON)
            content = vmovq_n_u8(b ? -1 : 0);
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask128b& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask128b& operator=(const Vector_mask128b&) = default;
        Vector_mask128b& operator=(Vector_mask128b&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        AVEL_FINL friend bool operator==(Vector_mask128b lhs, Vector_mask128b rhs) {
            #if defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u8(vceqq_s8(decay(lhs), decay(rhs)));
            return min == 0xFF;

            #elif defined(AVEL_NEON)
            auto x_as_ints = vreinterpretq_s32_u8(decay(lhs));
            auto y_as_ints = vreinterpretq_s32_u8(decay(rhs));

            auto abcd = vceqq_s32(x_as_ints, y_as_ints);
            auto addition0 = vpadd_u32(vget_high_u32(abcd), vget_high_u32(abcd));
            auto addition1 = vpadd_u32(addition0, addition0);

            return vget_lane_u32(addition1, 0x00) == -4;

            #endif
        }

        AVEL_FINL friend bool operator!=(Vector_mask128b lhs, Vector_mask128b rhs) {
            #if defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));
            #endif

            #if defined(AVEL_AARCH64)
            auto min = vminvq_u8(vceqq_s8(decay(lhs), decay(rhs)));
            return min == 0x00;

            #elif defined(AVEL_NEON)
            auto x_as_ints = vreinterpretq_s32_u8(decay(lhs));
            auto y_as_ints = vreinterpretq_s32_u8(decay(rhs));

            auto abcd = vceqq_s32(x_as_ints, y_as_ints);
            auto addition0 = vpadd_u32(vget_high_u32(abcd), vget_high_u32(abcd));
            auto addition1 = vpadd_u32(addition0, addition0);

            return vget_lane_u32(addition1, 0x00) != -4;

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask128b& operator&=(Vector_mask128b rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u8(content, rhs.content);
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask128b& operator|=(Vector_mask128b rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u8(content, rhs.content);
            #endif

            return *this;
        }

        AVEL_FINL Vector_mask128b& operator^=(Vector_mask128b rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = veorq_u8(content, rhs.content);
            #endif

            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask128b operator!() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector_mask128b{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector_mask128b{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};

            #endif

            #if defined(AVEL_NEON)
            return Vector_mask128b{vmvnq_u8(content)};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask128b operator&(Vector_mask128b lhs, Vector_mask128b rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask128b operator&&(Vector_mask128b lhs, Vector_mask128b rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask128b operator|(Vector_mask128b lhs, Vector_mask128b rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask128b operator||(Vector_mask128b lhs, Vector_mask128b rhs) {
            lhs |= rhs;
            return lhs;
        }


        [[nodiscard]]
        AVEL_FINL friend Vector_mask128b operator^(Vector_mask128b lhs, Vector_mask128b rhs) {
            lhs ^= rhs;
            return lhs;
        }

        //=================================================
        // Conversion operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

}

#endif //AVEL_MASK128B_HPP
