#ifndef AVEL_VEC128B_HPP
#define AVEL_VEC128B_HPP

namespace avel_impl {

    class alignas(16) Vector128b {
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

        //=================================================
        // -ctors
        //=================================================

        AVEL_FINL explicit Vector128b(primitive content):
            content(content) {}

        Vector128b() = default;
        Vector128b(const Vector128b&) = default;
        Vector128b(Vector128b&&) = default;
        ~Vector128b() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector128b& operator=(const Vector128b&) = default;
        Vector128b& operator=(Vector128b&&) = default;

        AVEL_FINL Vector128b& operator=(primitive p) {
            content = p;
            return *this;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector128b& operator&=(Vector128b rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vandq_u8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector128b& operator|=(Vector128b rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);
            #endif

            #if defined(AVEL_NEON)
            content = vorrq_u8(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector128b& operator^=(Vector128b rhs) {
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
        AVEL_FINL Vector128b operator~() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector128b{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector128b{_mm_andnot_si128(content, _mm_cmpeq_epi32(t, t))};

            #endif

            #if defined(AVEL_NEON)
            return Vector128b{vmvnq_u8(content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector128b operator&(Vector128b lhs, Vector128b rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector128b operator|(Vector128b lhs, Vector128b rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector128b operator^(Vector128b lhs, Vector128b rhs) {
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

    protected:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    };

}

#endif //AVEL_VEC128B_HPP
