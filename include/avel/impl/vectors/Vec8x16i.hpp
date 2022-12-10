#ifndef AVEL_VEC8X16I_HPP
#define AVEL_VEC8X16I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x16i = Vector<std::int16_t, 8>;

    using mask8x16i = Vector_mask<std::int16_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x16i> div(vec8x16i numerator, vec8x16i denominator);
    vec8x16i broadcast_mask(mask8x16i m);
    vec8x16i blend(vec8x16i a, vec8x16i b, mask8x16i m);





    template<>
    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
    class Vector_mask<std::int16_t, 8> : public avel_impl::Vector_mask8xT {
        using base = avel_impl::Vector_mask8xT;
    #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
    class Vector_mask<std::int16_t, 8> : public avel_impl::Vector_mask128b {
        using base = avel_impl::Vector_mask128b;
    #endif
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using primitive = base::primitive;

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        AVEL_FINL explicit Vector_mask(base b):
            base(b) {}

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> m):
            base(convert<Vector_mask>(m)[0]) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 8>& arr) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m128i array_data = _mm_loadu_si64(arr.data());
            content = _mm_cmplt_epu8_mask(_mm_setzero_si128(), array_data);

            #elif defined(AVEL_SSE2)
            __m128i array_data = _mm_loadu_si64(arr.data());
            content = _mm_sub_epi16(_mm_setzero_si128(), _mm_unpacklo_epi8(array_data, _mm_setzero_si128()));

            #endif
        }

        Vector_mask() = default;
        Vector_mask(const Vector_mask&) = default;
        Vector_mask(Vector_mask&&) = default;
        ~Vector_mask() = default;

        //=================================================
        // Assignment operators
        //=================================================

        using base::operator=;

        Vector_mask& operator=(const Vector_mask&) = default;
        Vector_mask& operator=(Vector_mask&&) = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return _mm512_mask2int(lhs.content) == _mm512_mask2int(rhs.content);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) == _mm_movemask_epi8(decay(rhs));

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask lhs, Vector_mask rhs) noexcept {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return _mm512_mask2int(lhs.content) != _mm512_mask2int(rhs.content);

            #elif defined(AVEL_SSE2)
            return _mm_movemask_epi8(decay(lhs)) != _mm_movemask_epi8(decay(rhs));

            #endif
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _kand_mask16(content, rhs.content);

            #elif defined(AVEL_SSE2)
            content = _mm_and_si128(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _kor_mask16(content, rhs.content);

            #elif defined(AVEL_SSE2)
            content = _mm_or_si128(content, rhs.content);

            #endif
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _kand_mask16(_kxor_mask16(content, rhs.content), _cvtu32_mask16(0xFF));

            #elif defined(AVEL_SSE2)
            content = _mm_xor_si128(content, rhs.content);

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return Vector_mask{primitive(_knot_mask16(content))};

            #elif defined(AVEL_AVX512VL)
            return Vector_mask{_mm_ternarylogic_epi32(content, content, content, 0x01)};

            #elif defined(AVEL_SSE2)
            primitive t = _mm_undefined_si128();
            return Vector_mask{_mm_andnot_si128(content, _mm_cmpeq_epi16(t, t))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            return lhs | rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator^(Vector_mask lhs, Vector_mask rhs) {
            lhs ^= rhs;
            return lhs;
        }

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(decay(m));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / 2;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return decay(m);
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm512_mask2int(decay(m)) == 0x00FF;
        #elif defined(AVEL_SSE2)
        auto tmp = _mm_movemask_epi8(decay(m));
        return 0xFFFF == tmp;
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x16i m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask8x16i>(mask8x16i m) {
        return std::array<mask8x16i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 8> convert<mask1x8u, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x8u, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 8> convert<mask1x8i, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x8i, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 8> convert<mask1x16u, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x16u, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 8> convert<mask1x16i, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x16i, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 8> convert<mask1x32u, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x32u, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 8> convert<mask1x32i, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x32i, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64u, 8> convert<mask1x64u, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x64u, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 8> convert<mask1x64i, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x64i, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 8> convert<mask1x32f, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x32f, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64f, 8> convert<mask1x64f, mask8x16i>(mask8x16i m) {
        alignas(8) std::array<mask1x64f, 8> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        _mm_store_epi64(ret.data(), _mm_movm_epi8(decay(m)));

        #elif defined(AVEL_SSE2)
        _mm_store_epi64(ret.data(), _mm_packus_epi16(decay(m), _mm_setzero_si128()));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask8x16i>(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_packus_epi16(decay(m), _mm_setzero_si128())}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask8x16i>(mask8x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8i{_mm_packus_epi16(decay(m), _mm_setzero_si128())}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask8x16i>(mask8x16i m) {
        return {mask8x16u{mask8x16u::primitive(decay(m))}};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x8u>(mask1x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x8i>(mask1x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x16u>(mask1x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x16i>(mask1x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x32u>(mask1x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x32i>(mask1x32i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x64u>(mask1x64u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x64i>(mask1x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x32f>(mask1x32f m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask1x64f>(mask1x64f m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask8x16i{mask8x16i::primitive(-decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask8x16i{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 2> convert<mask8x16i, mask16x8u>(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16i, 2>{
            mask8x16i{mask8x16i::primitive(_mm512_mask2int(decay(m)) & 0xFF)},
            mask8x16i{mask8x16i::primitive(_mm512_mask2int(decay(m)) >> 0x8)}
        };

        #elif defined(AVEL_SSE2)
        return {
            mask8x16i{_mm_unpacklo_epi8(decay(m), decay(m))},
            mask8x16i{_mm_unpackhi_epi8(decay(m), decay(m))}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 2> convert<mask8x16i, mask16x8i>(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {
            mask8x16i{mask8x16i::primitive(_mm512_mask2int(decay(m)) & 0xFF)},
            mask8x16i{mask8x16i::primitive(_mm512_mask2int(decay(m)) >> 0x8)}
        };

        #elif defined(AVEL_SSE2)
        return {
            mask8x16i{_mm_unpacklo_epi8(decay(m), decay(m))},
            mask8x16i{_mm_unpackhi_epi8(decay(m), decay(m))}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16i, 1> convert<mask8x16i, mask8x16u>(mask8x16u m) {
        return {mask8x16i{mask8x16i::primitive(decay(m))}};
    }






    template<>
    class alignas(16) Vector<std::int16_t, 8> : public avel_impl::Vec8x16int {
        using base = avel_impl::Vec8x16int;
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int16_t;

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int16x8_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<std::uint32_t M>
        using rebind_width = Vector<scalar, M>;

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        AVEL_FINL explicit Vector(base b):
            base(b) {}

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> x):
            base(convert<Vector>(x)[0]) {}

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            base(_mm_sub_epi16(_mm_setzero_si128(), _mm_movm_epi16(decay(m)))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_sub_epi16(_mm_setzero_si128(), decay(m))) {}
        #endif
        #if defined(AVEL_NEON)
            base() {} //TODO: Implement
        #endif

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_SSE2)
            base(_mm_set1_epi16(x)) {}
        #endif
        #if defined(AVEL_NEON)
            base() {} //TODO: Implement
        #endif

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array) {
            #if defined(AVEL_SSE2)
            content = _mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()));
            #endif

            #if defined(AVEL_NEON)
            //TODO: Implement
            #endif
        }

        Vector() = default;
        Vector(const Vector&) = default;
        Vector(Vector&&) = default;
        ~Vector() = default;

        //=================================================
        // Assignment operators
        //=================================================

        using base::operator=;

        AVEL_FINL Vector& operator=(scalar x) {
            #if defined(AVEL_SSE2)
            content = _mm_set1_epi16(x);
            #endif
            return *this;
        }

        Vector& operator=(const Vector&) = default;
        Vector& operator=(Vector&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend mask operator==(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpeq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return !mask{_mm_cmpeq_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpeq_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{mask::primitive(_mm_cmple_epi16_mask(lhs.content, rhs.content))};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpgt_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpgt_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi16(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epi16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmplt_epi16(lhs.content, rhs.content)};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+() const {
            return *this;
        }

        [[nodiscard]]
        AVEL_FINL Vector operator-() const {
            return Vector{} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_mullo_epi16(content, rhs.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.quot.content;
            return *this;
        }

        AVEL_FINL Vector& operator%=(Vector rhs) {
            auto results = div(*this, rhs);
            content = results.rem.content;
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend Vector operator+(Vector lhs, Vector rhs) {
            lhs += rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator-(Vector lhs, Vector rhs) {
            lhs -= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator*(Vector lhs, Vector rhs) {
            lhs *= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator/(Vector lhs, Vector rhs) {
            lhs /= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator%(Vector lhs, Vector rhs) {
            lhs %= rhs;
            return lhs;
        }

        //=================================================
        // Increment/Decrement operators
        //=================================================

        AVEL_FINL Vector& operator++() {
            *this += Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator++(int) {
            auto temp = *this;
            *this += Vector{1};
            return temp;
        }

        AVEL_FINL Vector& operator--() {
            *this -= Vector{1};
            return *this;
        }

        AVEL_FINL Vector operator--(int) {
            auto temp = *this;
            *this -= Vector{1};
            return temp;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector& operator>>=(long long rhs) {
            #if defined(AVEL_SSE2)
            content = _mm_sra_epi16(content, _mm_cvtsi64_si128(rhs));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_srav_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepi16_epi32(content);
            auto t1 = _mm256_cvtepi16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_srav_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));

            #elif defined(AVEL_SSE2)
            auto threshold = _mm_set1_epi16(8);

            for (unsigned i = 4; i-- > 0; ) {
                auto m = _mm_cmplt_epi16(decay(rhs), threshold);

                auto a = _mm_sra_epi16(content, _mm_cvtsi64_si128(1u << i));
                auto b = content;

                auto t0 = _mm_andnot_si128(m, a);
                auto t1 = _mm_and_si128(m, b);
                content = _mm_or_si128(t0, t1);

                rhs = _mm_sub_epi16(decay(rhs), _mm_andnot_si128(m, threshold));
                threshold = _mm_srai_epi16(threshold, 0x1);
            }

            #endif
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator~() const {
            return Vector{base::operator~()};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator&(Vector lhs, Vector rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator|(Vector lhs, Vector rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator^(Vector lhs, Vector rhs) {
            lhs ^= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, long long rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, long long rhs) {
            lhs >>= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator<<(Vector lhs, Vector rhs) {
            lhs <<= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, Vector rhs) {
            lhs >>= rhs;
            return lhs;
        }

        //=================================================
        // Conversion operators
        //=================================================

        #if defined(AVEL_NEON)
        [[nodiscard]]
        AVEL_FINL explicit operator primitive() const {
            return vreinterpretq_s16_u8(content);
        }
        #endif

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return Vector{} != *this;
        }

    };

    //=====================================================
    // Delayed definitions
    //=====================================================

    AVEL_FINL vec8x16i operator-(vec8x16u x) {
        return vec8x16i{} - vec8x16i{x};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x16i broadcast_mask(mask8x16i m) {
        return vec8x16i{broadcast_mask(mask8x16u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i blend(vec8x16i a, vec8x16i b, mask8x16i m) {
        return vec8x16i{blend(vec8x16u{a}, vec8x16u{b}, mask8x16u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i byteswap(vec8x16i x) {
        return vec8x16i{byteswap(vec8x16u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i max(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return vec8x16i{_mm_max_epi16(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i min(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return vec8x16i{_mm_min_epi16(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 2> minmax(vec8x16i a, vec8x16i b) {
        #if defined(AVEL_SSE41)
        return std::array<vec8x16i, 2>{
            vec8x16i{_mm_min_epi16(decay(a), decay(b))},
            vec8x16i{_mm_max_epi16(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto m = b < a;
        return std::array<vec8x16i, 2>{
            blend(a, b, m),
            blend(b, a, m)
        };

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i clamp(vec8x16i x, vec8x16i lo, vec8x16i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i average(vec8x16i x, vec8x16i y) {
        #if defined(AVEL_AVX512VL)
        auto z = (x >> 1) + (y >> 1);
        auto c = _mm_cmplt_epi16(decay(z), _mm_setzero_si128());
        auto d = _mm_ternarylogic_epi32(decay(x), decay(y), c, 0xE8);

        return z + (vec8x16i{d} & vec8x16i{0x1});

        #elif defined(AVEL_SSE2)
        auto z = (x >> 1) + (y >> 1);
        auto c = z < vec8x16i{0};
        auto d = (x & y) | (broadcast_mask(c) & (x ^ y));

        return z + (d & vec8x16i{0x1});

        #endif

        #if defined(AVEL_NEON)
        return vec8x16i{vhaddq_s16(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i midpoint(vec8x16i a, vec8x16i b) {
        vec8x16i addition_mask{static_cast<std::int16_t>(0x8000)};
        a ^= addition_mask;
        b ^= addition_mask;
        return vec8x16i{midpoint(vec8x16u{a}, vec8x16u{b})} ^ addition_mask;
    };

    [[nodiscard]]
    AVEL_FINL vec8x16i abs(vec8x16i x) {
        #if defined(AVEL_SSSE3)
        return vec8x16i{_mm_abs_epi16(decay(x))};

        #elif defined(AVEL_SSE2)
        auto y = x >> 15;
        return (x ^ y) - y;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i neg_abs(vec8x16i x) {
        #if defined(AVEL_SSSE3)
        return -vec8x16i{_mm_abs_epi16(decay(x))};

        #elif defined(AVEL_SSE2)
        auto y = ~(x >> 15);
        return (x ^ y) - y;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i neg_abs(vec8x16u x) {
        #if defined(AVEL_SSSE3)
        return -vec8x16i{_mm_abs_epi16(decay(x))};

        #elif defined(AVEL_SSE2)
        auto z = vec8x16i(x);
        auto y = ~z >> 15;
        return (z ^ y) - y;

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16i load<vec8x16i>(const std::int16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16i aligned_load<vec8x16i>(const std::int16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    AVEL_FINL void store(std::int16_t* ptr, vec8x16i x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::int16_t* ptr, vec8x16i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x16i> div(vec8x16i x, vec8x16i y) {
        vec8x16i quotient{};

        mask8x16i sign_mask0 = (x < vec8x16i{});
        mask8x16i sign_mask1 = (y < vec8x16i{});

        mask8x16i sign_mask2 = sign_mask0 ^ sign_mask1;

        vec8x16u numerator{abs(x)};
        vec8x16u denominator{abs(y)};

        //TODO: Compute i more appropriately?
        //TODO: Otherwise optimize
        std::int32_t i = 16;

        for (; (i-- > 0) && any(mask8x16u(numerator));) {
            mask8x16u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_mask(b) & (denominator << i));
            quotient |= (vec8x16u{b} << i);
        }

        //Adjust quotient's sign. Should be xor of operands' signs
        quotient = blend(quotient, -quotient, sign_mask2);

        //Adjust numerator's sign. Should be same sign as it was originally
        x = blend(vec8x16i{numerator}, -vec8x16i{numerator}, sign_mask0);

        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i popcount(vec8x16i x) {
        return vec8x16i{popcount(vec8x16u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i countl_zero(vec8x16i x) {
        return vec8x16i{countl_zero(vec8x16u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i countl_one(vec8x16i x) {
        return vec8x16i{countl_one(vec8x16u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i countr_zero(vec8x16i x) {
        return vec8x16i{countr_zero(vec8x16u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i countr_one(vec8x16i x) {
        return vec8x16i{countr_one(vec8x16u{x})};
    }

    [[nodiscard]]
    AVEL_FINL mask8x16i has_single_bit(vec8x16i x) {
        return mask8x16i{has_single_bit(vec8x16u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotl(vec8x16i x, long long s) {
        return vec8x16i{rotl(vec8x16u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotl(vec8x16i x, vec8x16i s) {
        return vec8x16i{rotl(vec8x16u{x}, vec8x16u{s})};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotr(vec8x16i x, long long s) {
        return vec8x16i{rotr(vec8x16u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16i rotr(vec8x16i x, vec8x16i s) {
        return vec8x16i{rotr(vec8x16u{x}, vec8x16u{s})};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::int16_t, 8> to_array(vec8x16i x) {
        alignas(16) std::array<std::int16_t, 8> ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec8x16i>(vec8x16i x) {
        return std::array<vec8x16i, 1>{x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 8> convert<vec1x8u, vec8x16i>(vec8x16i x) {
        alignas(8) std::array<vec1x8u, 8> ret;

        #if defined(AVEL_SSE2)
        auto t0 = _mm_packus_epi16(_mm_set1_epi16(0xFF), _mm_setzero_si128());
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), t0);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 8> convert<vec1x8i, vec8x16i>(vec8x16i x) {
        alignas(8) std::array<vec1x8i, 8> ret;

        #if defined(AVEL_SSE2)
        auto t0 = _mm_packs_epi16(_mm_set1_epi16(0xFF), _mm_setzero_si128());
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), t0);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 8> convert<vec1x16u, vec8x16i>(vec8x16i x) {
        alignas(16) std::array<vec1x16u, 8> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(x));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 8> convert<vec1x16i, vec8x16i>(vec8x16i x) {
        alignas(16) std::array<vec1x16i, 8> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(x));

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 8> convert<vec1x32u, vec8x16i>(vec8x16i x) {
        alignas(32) std::array<vec1x32u, 8> ret;
        #if defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi16(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi16(whole, wholeb);
        auto half1 = _mm_unpackhi_epi16(whole, wholeb);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), half0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), half1);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 8> convert<vec1x32i, vec8x16i>(vec8x16i x) {
        alignas(32) std::array<vec1x32i, 8> ret;
        #if defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi16(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi16(whole, wholeb);
        auto half1 = _mm_unpackhi_epi16(whole, wholeb);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), half0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), half1);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 8> convert<vec1x64u, vec8x16i>(vec8x16i x) {
        alignas(64) std::array<vec1x64u, 8> ret;

        #if defined(AVEL_AVX512F)
        auto t0 = _mm512_cvtepi16_epi64(decay(x));
        _mm512_store_si512(ret.data(), t0);

        #elif defined(AVEL_AVX2)
        auto half0 = _mm256_cvtepi16_epi64(decay(x));
        auto half1 = _mm256_cvtepi16_epi64(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), half0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x4), half1);

        #elif defined(AVEL_SSE41)
        auto quarter0 = _mm_cvtepi16_epi64(decay(x));
        auto quarter1 = _mm_cvtepi16_epi64(_mm_srli_si128(decay(x), 0x4));
        auto quarter2 = _mm_cvtepi16_epi64(_mm_unpackhi_epi64(decay(x), decay(x)));
        auto quarter3 = _mm_cvtepi16_epi64(_mm_srli_si128(decay(x), 0xC));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quarter0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), quarter1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quarter2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), quarter3);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi16(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi16(whole, wholeb);
        auto half0b = _mm_cmplt_epi16(half0, _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi16(whole, wholeb);
        auto half1b = _mm_cmplt_epi16(half1, _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi32(half0, half0b);
        auto quarter1 = _mm_unpackhi_epi32(half0, half0b);
        auto quarter2 = _mm_unpacklo_epi32(half1, half1b);
        auto quarter3 = _mm_unpackhi_epi32(half1, half1b);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quarter0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), quarter1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quarter2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), quarter3);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 8> convert<vec1x64i, vec8x16i>(vec8x16i x) {
        alignas(64) std::array<vec1x64i, 8> ret;

        #if defined(AVEL_AVX512F)
        auto t0 = _mm512_cvtepi16_epi64(decay(x));
        _mm512_store_si512(ret.data(), t0);

        #elif defined(AVEL_AVX2)
        auto half0 = _mm256_cvtepi16_epi64(decay(x));
        auto half1 = _mm256_cvtepi16_epi64(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), half0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x4), half1);

        #elif defined(AVEL_SSE41)
        auto quarter0 = _mm_cvtepi16_epi64(decay(x));
        auto quarter1 = _mm_cvtepi16_epi64(_mm_srli_si128(decay(x), 0x4));
        auto quarter2 = _mm_cvtepi16_epi64(_mm_unpackhi_epi64(decay(x), decay(x)));
        auto quarter3 = _mm_cvtepi16_epi64(_mm_srli_si128(decay(x), 0xC));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quarter0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), quarter1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quarter2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), quarter3);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi16(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi16(whole, wholeb);
        auto half0b = _mm_cmplt_epi16(half0, _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi16(whole, wholeb);
        auto half1b = _mm_cmplt_epi16(half1, _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi32(half0, half0b);
        auto quarter1 = _mm_unpackhi_epi32(half0, half0b);
        auto quarter2 = _mm_unpacklo_epi32(half1, half1b);
        auto quarter3 = _mm_unpackhi_epi32(half1, half1b);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quarter0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), quarter1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quarter2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), quarter3);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 8> convert<vec1x32f, vec8x16i>(vec8x16i x) {
        alignas(32) std::array<vec1x32f, 8> ret;

        #if defined(AVEL_AVX2)
        auto whole = _mm256_cvtepi32_ps(_mm256_cvtepi16_epi32(decay(x)));
        _mm256_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), whole);

        #elif defined(AVEL_AVX)
        auto whole = decay(x);

        auto half0 = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(whole));
        auto half1 = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(_mm_unpackhi_epi64(whole, whole)));

        _mm256_store_ps(reinterpret_cast<float*>(ret.data()), _mm256_set_m128(half0, half1));

        #elif defined(AVEL_SSE41)
        auto whole = decay(x);

        auto half0 = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(whole));
        auto half1 = _mm_cvtepi32_ps(_mm_cvtepi16_epi32(_mm_unpackhi_epi64(whole, whole)));

        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), half0);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x4), half1);

        #elif defined(AVEL_SSE2)
        auto zeros = _mm_setzero_si128();

        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi16(whole, zeros);

        auto half0 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(whole, wholeb));
        auto half1 = _mm_cvtepi32_ps(_mm_unpackhi_epi16(whole, wholeb));

        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), half0);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x4), half1);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64f, 8> convert<vec1x64f, vec8x16i>(vec8x16i x) {
        alignas(64) std::array<vec1x64f, 8> ret;

        #if defined(AVEL_AVX512F)
        auto whole = _mm512_cvtepi32_pd(_mm256_cvtepi16_epi32(decay(x)));
        _mm512_store_pd(ret.data(), whole);

        #elif defined(AVEL_AVX2)
        auto whole = _mm256_cvtepi16_epi32(decay(x));

        auto half0 = _mm256_cvtepi32_pd(_mm256_castsi256_si128(whole));
        auto half1 = _mm256_cvtepi32_pd(_mm256_extractf128_si256(whole, 0x1));

        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), half0);
        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), half1);

        #elif defined(AVEL_AVX)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi16(whole, _mm_setzero_si128());

        auto half0 = _mm256_cvtepi32_pd(_mm_unpacklo_epi16(whole, wholeb));
        auto half1 = _mm256_cvtepi32_pd(_mm_unpackhi_epi16(whole, wholeb));

        _mm256_store_pd(reinterpret_cast<double*>(ret.data()), half0);
        _mm256_store_pd(reinterpret_cast<double*>(ret.data()), half1);

        #elif defined(AVEL_SSE41)
        auto half0 = _mm_cvtepi16_epi32(decay(x));
        auto half1 = _mm_cvtepi16_epi32(_mm_unpackhi_epi64(decay(x), decay(x)));

        auto quarter0 = _mm_cvtepi32_pd(half0);
        auto quarter1 = _mm_cvtepi32_pd(_mm_srli_si128(half0, 0x8));
        auto quarter2 = _mm_cvtepi32_pd(half1);
        auto quarter3 = _mm_cvtepi32_pd(_mm_srli_si128(half1, 0x8));

        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), quarter0);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x2), quarter1);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), quarter2);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x6), quarter3);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi16(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi16(whole, wholeb);
        auto half1 = _mm_unpackhi_epi16(whole, wholeb);

        auto quarter0 = _mm_cvtepi32_pd(half0);
        auto quarter1 = _mm_cvtepi32_pd(_mm_srli_si128(half0, 0x8));
        auto quarter2 = _mm_cvtepi32_pd(half1);
        auto quarter3 = _mm_cvtepi32_pd(_mm_srli_si128(half1, 0x8));

        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), quarter0);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x2), quarter1);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), quarter2);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x6), quarter3);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec8x16i>(vec8x16i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_packus_epi16(decay(x), _mm_setzero_si128())}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec8x16i>(vec8x16i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_packs_epi16(decay(x), _mm_setzero_si128())}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec8x16i>(vec8x16i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{decay(x)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x8u>(vec1x8u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x8i>(vec1x8i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x16u>(vec1x16u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x16i>(vec1x16i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x32u>(vec1x32u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x) & 0xFFFF))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x32i>(vec1x32i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x) & 0xFFFF))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x64u>(vec1x64u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x) & 0xFFFF))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x64i>(vec1x64i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x) & 0xFFFF))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x32f>(vec1x32f x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec1x64f>(vec1x64f x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{_mm_cvtsi32_si128(vec8x16i::scalar(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 2> convert<vec8x16i, vec16x8u>(vec16x8u x) {
        #if defined(AVEL_SSE2)
        return {
            vec8x16i{_mm_unpacklo_epi8(decay(x), _mm_setzero_si128())},
            vec8x16i{_mm_unpackhi_epi8(decay(x), _mm_setzero_si128())}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 2> convert<vec8x16i, vec16x8i>(vec16x8i x) {
        #if defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        return {
            vec8x16i{_mm_unpacklo_epi8(whole, wholeb)},
            vec8x16i{_mm_unpackhi_epi8(whole, wholeb)}
        };
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16i, 1> convert<vec8x16i, vec8x16u>(vec8x16u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16i{vec8x16i::primitive(decay(x))}};
        #endif

        #if defined(AVEL_NEON)
        return {vec8x16i{vreinterpretq_s16_u16(decay(x))}};
        #endif
    }

}

#endif //AVEL_VEC8x16I_HPP
