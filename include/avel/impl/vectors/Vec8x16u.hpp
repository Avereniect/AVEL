#ifndef AVEL_VEC8X16U_HPP
#define AVEL_VEC8X16U_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec8x16u = Vector<std::uint16_t, 8>;

    using mask8x16u = Vector_mask<std::uint16_t, 8>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec8x16u> div(vec8x16u numerator, vec8x16u denominator);
    vec8x16u broadcast_mask(mask8x16u m);
    vec8x16u blend(vec8x16u a, vec8x16u b, mask8x16u m);
    vec8x16u countl_one(vec8x16u x);





    template<>
    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
    class Vector_mask<std::uint16_t, 8> : public avel_impl::Vector_mask8xT {
        using base = avel_impl::Vector_mask8xT;
    #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
    class Vector_mask<std::uint16_t, 8> : public avel_impl::Vector_mask128b {
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
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> x):
            base(convert<Vector_mask>(x)[0]) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 8>& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m128i array_data = _mm_loadu_si64(arr.data());
            content = __mmask8(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            primitive array_data = _mm_loadu_si64(arr.data());

            array_data = _mm_unpacklo_epi8(array_data, array_data);
            content = _mm_cmplt_epi16(_mm_setzero_si128(), array_data);

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
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask& operator&=(Vector_mask rhs) {
            base::operator&=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator|=(Vector_mask rhs) {
            base::operator|=(rhs);
            return *this;
        }

        AVEL_FINL Vector_mask& operator^=(Vector_mask rhs) {
            base::operator^=(rhs);
            return *this;
        }

        //=================================================
        // Bitwise operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask operator!() const {
            return Vector_mask{base::operator!()};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator&&(Vector_mask lhs, Vector_mask rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator|(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask operator||(Vector_mask lhs, Vector_mask rhs) {
            lhs |= rhs;
            return lhs;
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
    AVEL_FINL std::uint32_t count(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(_mm512_mask2int(decay(m)));

        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m))) / sizeof(std::uint16_t);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm512_mask2int(decay(m));

        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));

        #endif
    }


    [[nodiscard]]
    AVEL_FINL bool all(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return 0xFF == _mm512_mask2int(decay(m));

        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));

        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask8x16u m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask8x16u>(mask8x16u m) {
        return {m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 8> convert<mask1x8u, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x8i, 8> convert<mask1x8i, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x16u, 8> convert<mask1x16u, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x16i, 8> convert<mask1x16i, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x32u, 8> convert<mask1x32u, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x32i, 8> convert<mask1x32i, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x64u, 8> convert<mask1x64u, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x64i, 8> convert<mask1x64i, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x32f, 8> convert<mask1x32f, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask1x64f, 8> convert<mask1x64f, mask8x16u>(mask8x16u m) {
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
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask8x16u>(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return {mask16x8u{mask16x8u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return {mask16x8u{_mm_packus_epi16(decay(m), _mm_setzero_si128())}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask8x16u>(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_packus_epi16(decay(m), _mm_setzero_si128())}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x8i>(mask1x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x16u>(mask1x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x16i>(mask1x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x32u>(mask1x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x32i>(mask1x32i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x64u>(mask1x64u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x64i>(mask1x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x32f>(mask1x32f m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 1> convert<mask8x16u, mask1x64f>(mask1x64f m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 1>{mask8x16u{mask8x16u::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 1>{mask8x16u{_mm_set1_epi8(decay(m))}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 2> convert<mask8x16u, mask16x8u>(mask16x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 2>{
            mask8x16u{mask8x16u::primitive(_mm512_mask2int(decay(m)) & 0xFF)},
            mask8x16u{mask8x16u::primitive(_mm512_mask2int(decay(m)) >> 0x8)}
        };

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 2>{
            mask8x16u{_mm_unpacklo_epi8(decay(m), decay(m))},
            mask8x16u{_mm_unpackhi_epi8(decay(m), decay(m))}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask8x16u, 2> convert<mask8x16u, mask16x8i>(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask8x16u, 2>{
            mask8x16u{mask8x16u::primitive(_mm512_mask2int(decay(m)) & 0xFF)},
            mask8x16u{mask8x16u::primitive(_mm512_mask2int(decay(m)) >> 0x8)}
        };

        #elif defined(AVEL_SSE2)
        return std::array<mask8x16u, 2>{
            mask8x16u{_mm_unpacklo_epi8(decay(m), decay(m))},
            mask8x16u{_mm_unpackhi_epi8(decay(m), decay(m))}
        };

        #endif
    }





    template<>
    class Vector<std::uint16_t, 8> : public avel_impl::Vec8x16int {
        using base = avel_impl::Vec8x16int;
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 8;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::uint16_t;

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = uint16x8_t;
        #endif

        using mask = Vector_mask<scalar, width>;

        template<class U>
        using rebind_type = Vector<U, width>;

        template<int M>
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

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vector(primitive p):
            base(vreinterpretq_s16_u16(p)) {}
        #endif

        AVEL_FINL explicit Vector(mask m):
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            base(_mm_mask_blend_epi16(decay(m), _mm_setzero_si128(), _mm_set1_epi16(1))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_sub_epi16(_mm_setzero_si128(), static_cast<primitive>(m))) {}
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

        AVEL_FINL explicit Vector(const std::array<scalar, width>& array):
        #if defined(AVEL_SSE2)
            base(_mm_loadu_si128(reinterpret_cast<const __m128i*>(array.data()))) {}
        #endif
        #if defined(AVEL_NEON)
            base() {} //TODO: Implement
        #endif

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
        Vector& operator=(Vector&&) = default;

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
            return mask{_mm_cmpneq_epu16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !(lhs == rhs);
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epu16_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            primitive addition_mask = _mm_set1_epi16(0x8000);
            return mask{_mm_cmplt_epi16(_mm_xor_si128(lhs.content, addition_mask), _mm_xor_si128(rhs.content, addition_mask))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmple_epu16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{lhs > rhs};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpgt_epu16_mask(lhs.content, rhs.content)};

            #elif defined(AVEL_SSE2)
            primitive addition_mask = _mm_set1_epi16(0x8000);
            return mask{_mm_cmpgt_epi16(_mm_xor_si128(lhs.content, addition_mask), _mm_xor_si128(rhs.content, addition_mask))};

            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epu16_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{lhs < rhs};
            #endif
        }

        //=================================================
        // Unary arithmetic operators
        //=================================================

        AVEL_FINL Vector operator+() const {
            return *this;
        }

        //Definition of operator-() deferred until after definition of vec8x16i

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_add_epi16(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_sub_epi16(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector vec) {
            #if defined(AVEL_SSE2)
            content = _mm_mullo_epi16(content, vec.content);
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator/=(Vector vec) {
            auto results = div(*this, vec);
            *this = results.quot;
            return *this;
        }

        AVEL_FINL Vector& operator%=(const Vector vec) {
            auto results = div(*this, vec);
            *this = results.rem;
            return *this;
        }

        //=================================================
        // Arithmetic operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector operator+(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_add_epi16(content, vec.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator-(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_sub_epi16(content, vec.content)};
            #endif
        }

        [[nodiscard]]
       AVEL_FINL  Vector operator*(const Vector vec) const {
            #if defined(AVEL_SSE2)
            return Vector{_mm_mullo_epi16(content, vec.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL Vector operator/(const Vector vec) const {
            auto results = div(*this, vec);
            return results.quot;
        }

        [[nodiscard]]
        AVEL_FINL Vector operator%(const Vector vec) const {
            auto results = div(*this, vec);
            return results.rem;
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

        AVEL_FINL Vector& operator>>=(long long s) {
            #if defined(AVEL_SSE2)
            //TODO: Saturate shift value
            content = _mm_srl_epi16(content, _mm_cvtsi32_si128(static_cast<int>(s)));
            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            content = _mm_srlv_epi16(content, rhs.content);

            #elif defined(AVEL_AVX2)
            auto t0 = _mm256_cvtepu16_epi32(content);
            auto t1 = _mm256_cvtepu16_epi32(decay(rhs));

            auto t2 = _mm256_and_si256(_mm256_srlv_epi32(t0, t1), _mm256_set1_epi32(0x0000FFFF));

            auto lo = t2;
            auto hi = _mm256_permute2f128_si256(lo, lo, 0x01);
            content = _mm256_castsi256_si128(_mm256_packus_epi32(lo, hi));

            #elif defined(AVEL_SSE2)
            for (unsigned i = 0; i < 4; ++i) {
                auto t0 = _mm_and_si128(rhs.content, _mm_set1_epi16(1u << i));
                auto m = _mm_cmplt_epi16(primitive{}, t0);

                auto a = _mm_andnot_si128(m, content);
                auto b = _mm_and_si128(m, _mm_srl_epi16(content, _mm_cvtsi64_si128(1u << i)));
                content = _mm_or_si128(a, b);
            }

            content = _mm_and_si128(content, _mm_cmplt_epi16(rhs.content, _mm_set1_epi16(16)));

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
        AVEL_FINL friend Vector operator<<(Vector lhs, long long s) {
            lhs <<= s;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector operator>>(Vector lhs, long long s) {
            lhs >>= s;
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
        // Conversions
        //=================================================

        [[nodiscard]]
        AVEL_FINL explicit operator mask() const {
            return *this != Vector{};
        }

    };

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x16u broadcast_mask(mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec8x16u{_mm_movm_epi16(decay(m))};

        #elif defined(AVEL_SSE2)
        return vec8x16u{mask8x16u::primitive(m)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u blend(vec8x16u a, vec8x16u b, mask8x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return vec8x16u{_mm_mask_blend_epi16(decay(m), decay(a), decay(b))};

        #elif defined(AVEL_SSE41)
        return vec8x16u{_mm_blendv_epi8(decay(a), decay(b), decay(m))};

        #elif defined(AVEL_SSE2)
        auto x = _mm_andnot_si128(decay(m), decay(a));
        auto y = _mm_and_si128(decay(m), decay(b));
        return vec8x16u{_mm_or_si128(x, y)};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u byteswap(vec8x16u x) {
        #if defined(AVEL_SSE2)
        auto lo = _mm_srli_epi16(decay(x), 8);
        auto hi = _mm_slli_epi16(decay(x), 8);
        return vec8x16u{_mm_or_si128(lo, hi)};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u max(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE41)
        return vec8x16u{_mm_max_epu16(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        return blend(a, b, a < b);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u min(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE41)
        return vec8x16u{_mm_min_epu16(decay(a), decay(b))};
        #elif defined(AVEL_SSE2)
        return blend(a, b, b < a);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 2> minmax(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE41)
        return {
            vec8x16u{_mm_min_epu16(decay(a), decay(b))},
            vec8x16u{_mm_max_epu16(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto m = b < a;
        return std::array<vec8x16u, 2>{
            blend(a, b, m),
            blend(b, a, m)
        };

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u clamp(vec8x16u x, vec8x16u lo, vec8x16u hi) {
        return vec8x16u{min(max(x, lo), hi)};
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u midpoint(vec8x16u a, vec8x16u b) {
        vec8x16u t0 = a & b & vec8x16u{0x1};
        vec8x16u t1 = (a | b) & vec8x16u{0x1} & broadcast_mask(a > b);
        vec8x16u t2 = t0 | t1;
        return (a >> 1) + (b >> 1) + t2;
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u average(vec8x16u a, vec8x16u b) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_avg_epu16(decay(a), decay(b))};
        #endif

        #if defined(AVEL_NEON)
        return vec8x16u{vhaddq_u16(decay(a), decay(b))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u load<vec8x16u>(const std::uint16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec8x16u aligned_load<vec8x16u>(const std::uint16_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec8x16u{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    AVEL_FINL void store(std::uint16_t* ptr, vec8x16u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif
    }

    AVEL_FINL void aligned_store(std::uint16_t* ptr, vec8x16u v) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(v));
        #endif
    }

    //=====================================================
    // Integer arithmetic functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec8x16u> div(vec8x16u x, vec8x16u y) {
        vec8x16u quotient{};

        auto z = vec8x16u{};
        for (std::uint32_t i = 16; (i-- > 0) && any(x != z);) {
            mask8x16u b = ((x >> i) >= y);
            x -= (broadcast_mask(b) & (y << i));
            quotient |= (vec8x16u{b} << i);
        }

        return {quotient, x};
    }

    //=====================================================
    // Bit operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec8x16u popcount(vec8x16u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return vec8x16u{_mm_popcnt_epi16(decay(x))};

        #elif defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 1, 2,
            1, 2, 2, 3,
            1, 2, 2, 3,
            2, 3, 3, 4
        };

        auto table = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto nibble_mask = _mm_set1_epi8(0x0F);

        auto index0 = _mm_and_si128(decay(x), nibble_mask);
        auto index1 = _mm_and_si128(_mm_srli_epi16(decay(x), 0x4), nibble_mask);

        auto partial_sum0 = _mm_shuffle_epi8(table, index0);
        auto partial_sum1 = _mm_shuffle_epi8(table, index1);

        auto byte_sums = _mm_add_epi8(partial_sum0, partial_sum1);

        auto short_sums = _mm_add_epi16(byte_sums, _mm_slli_epi16(byte_sums, 8));
        auto ret = _mm_srli_epi16(short_sums, 8);

        return vec8x16u{ret};

        #elif defined(AVEL_SSE2)
        // https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
        x = x - ((x >> 1) & vec8x16u{0x5555});
        x = (x & vec8x16u{0x3333}) + ((x >> 2) & vec8x16u{0x3333});
        x = ((x + (x >> 4) & vec8x16u{0x0F0F}) * vec8x16u{0x0101}) >> 8;
        return x;

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u countl_zero(vec8x16u x) {
        return countl_one(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u countl_one(vec8x16u x) {
        vec8x16u sum{x == vec8x16u{0xFFFF}};

        vec8x16u m0{0xFF00u};
        mask8x16u b0 = (m0 & x) == m0;
        sum += broadcast_mask(b0) & vec8x16u{8};
        x <<= broadcast_mask(b0) & vec8x16u{8};

        vec8x16u m1{0xF000u};
        mask8x16u b1 = (m1 & x) == m1;
        sum += broadcast_mask(b1) & vec8x16u{4};
        x <<= broadcast_mask(b1) & vec8x16u{4};

        vec8x16u m2{0xC000u};
        mask8x16u b2 = (m2 & x) == m2;
        sum += broadcast_mask(b2) & vec8x16u{2};
        x <<= broadcast_mask(b2) & vec8x16u{2};

        vec8x16u m3{0x8000u};
        mask8x16u b3 = (m3 & x) == m3;
        sum += broadcast_mask(b3) & vec8x16u{1};

        return sum;
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u countr_zero(vec8x16u x) {
        vec8x16u ret{0x0000};

        mask8x16u zero_mask = (x == vec8x16u{0x0000});

        x &= vec8x16u{0x0000} - x;

        mask8x16u b;
        b = mask8x16u(x & vec8x16u{0xAAAAu});
        ret |= (vec8x16u{b} << 0);
        b = mask8x16u(x & vec8x16u{0xCCCCu});
        ret |= (vec8x16u{b} << 1);
        b = mask8x16u(x & vec8x16u{0xF0F0u});
        ret |= (vec8x16u{b} << 2);
        b = mask8x16u(x & vec8x16u{0xFF00u});
        ret |= (vec8x16u{b} << 3);

        ret = blend(ret, vec8x16u{16}, zero_mask);

        return ret;
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u countr_one(vec8x16u x) {
        return countr_zero(~x);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u bit_width(vec8x16u x) {
        #if defined(AVEL_SSSE3)
        alignas(16) static constexpr std::uint8_t table_data[16] {
            0, 1, 2, 2,
            3, 3, 3, 3,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        alignas(16) static constexpr std::uint8_t table_increments_data[16] {
            0, 4, 4, 4,
            4, 4, 4, 4,
            4, 4, 4, 4,
            4, 4, 4, 4
        };

        auto nibble_mask = _mm_set1_epi8(0x0F);

        auto nibble0 = _mm_and_si128(nibble_mask, decay(x));
        auto nibble1 = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x4));
        auto nibble2 = _mm_and_si128(nibble_mask, _mm_srli_epi16(decay(x), 0x8));
        auto nibble3 = _mm_srli_epi16(decay(x), 0xC);

        auto table_incrementers = _mm_load_si128(reinterpret_cast<const __m128i*>(table_increments_data));
        auto table0 = _mm_load_si128(reinterpret_cast<const __m128i*>(table_data));
        auto table1 = _mm_add_epi16(table0, table_incrementers);
        auto table2 = _mm_add_epi16(table1, table_incrementers);
        auto table3 = _mm_add_epi16(table2, table_incrementers);

        auto partial0 = _mm_shuffle_epi8(table0, nibble0);
        auto partial1 = _mm_shuffle_epi8(table1, nibble1);
        auto partial2 = _mm_shuffle_epi8(table2, nibble2);
        auto partial3 = _mm_shuffle_epi8(table3, nibble3);

        auto partial4 = _mm_max_epu8(partial0, partial1);
        auto partial5 = _mm_max_epu8(partial2, partial3);
        auto partial6 = _mm_max_epu8(partial4, partial5);

        auto ret = _mm_and_si128(partial6, _mm_set1_epi16(0x00FF));

        return vec8x16u{ret};

        #elif defined(AVEL_SSE2)
        mask8x16u zero_mask = (x == vec8x16u{0x0000});

        vec8x16u ret{0x0000};

        auto b0 = mask8x16u{x & vec8x16u{0xFF00}};
        auto t0 = broadcast_mask(b0) & vec8x16u{8};
        ret += t0;
        x >>= t0;

        auto b1 = mask8x16u{x & vec8x16u{0xFFF0}};
        auto t1 = broadcast_mask(b1) & vec8x16u{4};
        ret += t1;
        x >>= t1;

        auto b2 = mask8x16u{x & vec8x16u{0xFFFC}};
        auto t2 = broadcast_mask(b2) & vec8x16u{2};
        ret += t2;
        x >>= t2;

        auto b3 = mask8x16u{x & vec8x16u{0xFFFE}};
        auto t3 = broadcast_mask(b3) & vec8x16u{1};
        ret += t3;

        return blend(ret + vec8x16u{1}, vec8x16u{0}, zero_mask);
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u bit_floor(vec8x16u x) {
        auto zero_mask = (x == vec8x16u{0x0000});

        x = x | (x >> 1);
        x = x | (x >> 2);
        x = x | (x >> 4);
        x = x | (x >> 8);
        x = x - (x >> 1);
        return blend(x, vec8x16u{0x0}, zero_mask);
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u bit_ceil(vec8x16u x) {
        auto zero_mask = (x == vec8x16u{0x0000});

        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        ++x;
        return blend(x, vec8x16u{0x0001}, zero_mask);
    }

    [[nodiscard]]
    AVEL_FINL mask8x16u has_single_bit(vec8x16u x) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BITALG)
        return popcount(x) == vec8x16u{1};

        #else
        return mask8x16u{x} & !mask8x16u{x & (x - vec8x16u{1})};

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotl(vec8x16u v, std::uint16_t s) {
        return (v << s) | (v >> (16 - s));
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotl(vec8x16u v, vec8x16u s) {
        return (v << s) | (v >> (vec8x16u{16} - s));
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotr(vec8x16u v, std::uint16_t s) {
        return (v >> s) | (v << (16 - s));
    }

    [[nodiscard]]
    AVEL_FINL vec8x16u rotr(vec8x16u v, vec8x16u s) {
        return (v >> s) | (v << (vec8x16u{16} - s));
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::uint16_t, 8> to_array(vec8x16u x) {
        alignas(16) std::array<std::uint16_t, 8> ret;
        aligned_store(ret.data(), x);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec8x16u>(vec8x16u x) {
        return {x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 8> convert<vec1x8u, vec8x16u>(vec8x16u x) {
        alignas(8) std::array<vec1x8u, 8> ret;
        #if defined(AVEL_SSE2)
        auto t0 = _mm_and_si128(decay(x), _mm_set1_epi16(0x00FF));
        auto t1 = _mm_packus_epi16(t0, _mm_setzero_si128());
        _mm_storeu_si64(ret.data(), t1);
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 8> convert<vec1x8i, vec8x16u>(vec8x16u x) {
        alignas(8) std::array<vec1x8i, 8> ret;
        #if defined(AVEL_SSE2)
        auto t0 = _mm_and_si128(decay(x), _mm_set1_epi16(0x00FF));
        auto t1 = _mm_packs_epi16(t0, _mm_setzero_si128());
        _mm_storeu_si64(ret.data(), t1);
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 8> convert<vec1x16u, vec8x16u>(vec8x16u x) {
        alignas(16) std::array<vec1x16u, 8> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(x));
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 8> convert<vec1x16i, vec8x16u>(vec8x16u x) {
        alignas(16) std::array<vec1x16i, 8> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(x));
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 8> convert<vec1x32u, vec8x16u>(vec8x16u x) {
        alignas(32) std::array<vec1x32u, 8> ret;
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepu16_epi32(decay(x)));

        #elif defined(AVEL_SSE41)
        auto lo = decay(x);
        auto hi = _mm_unpackhi_epi64(lo, lo);

        auto half0 = _mm_cvtepi16_epi32(lo);
        auto half1 = _mm_cvtepi16_epi32(hi);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), half0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), half1);

        #elif defined(AVEL_SSE2)
        auto lo = _mm_unpacklo_epi16(decay(x), _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi16(decay(x), _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), lo);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), hi);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 8> convert<vec1x32i, vec8x16u>(vec8x16u x) {
        alignas(32) std::array<vec1x32i, 8> ret;
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepu16_epi32(decay(x)));

        #elif defined(AVEL_SSE41)
        auto lo = decay(x);
        auto hi = _mm_unpackhi_epi64(lo, lo);

        auto half0 = _mm_cvtepi16_epi32(lo);
        auto half1 = _mm_cvtepi16_epi32(hi);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), half0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), half1);

        #elif defined(AVEL_SSE2)
        auto lo = _mm_unpacklo_epi16(decay(x), _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi16(decay(x), _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), lo);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), hi);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 8> convert<vec1x64u, vec8x16u>(vec8x16u x) {
        alignas(64) std::array<vec1x64u, 8> ret;
        #if defined(AVEL_AVX512F)
        auto out0 = _mm512_cvtepi16_epi64(decay(x));
        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data()), out0);

        #elif defined(AVEL_AVX2)
        auto half0 = decay(x);
        auto half1 = _mm_unpacklo_epi64(half0, half0);

        auto out0 = _mm256_cvtepi16_epi64(half0);
        auto out1 = _mm256_cvtepi16_epi64(half1);

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), out0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x4), out1);

        #elif defined(AVEL_SSE41)
        auto half0 = decay(x);
        auto half1 = _mm_unpacklo_epi64(half0, half0);

        auto quarter0 = _mm_cvtepi16_epi64(half0);
        auto quarter1 = _mm_cvtepi16_epi64(_mm_srli_si128(half0, 0x4));
        auto quarter2 = _mm_cvtepi16_epi64(half1);
        auto quarter3 = _mm_cvtepi16_epi64(_mm_srli_si128(half1, 0x4));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quarter0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), quarter1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quarter2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), quarter3);

        #elif defined(AVEL_SSE2)
        auto lo = _mm_unpacklo_epi16(decay(x), _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi16(decay(x), _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi32(lo, _mm_setzero_si128());
        auto quarter1 = _mm_unpackhi_epi32(lo, _mm_setzero_si128());
        auto quarter2 = _mm_unpacklo_epi32(hi, _mm_setzero_si128());
        auto quarter3 = _mm_unpackhi_epi32(hi, _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quarter0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), quarter1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quarter2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), quarter3);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 8> convert<vec1x64i, vec8x16u>(vec8x16u x) {
        alignas(64) std::array<vec1x64i, 8> ret;
        #if defined(AVEL_SSE41)
        auto half0 = decay(x);
        auto half1 = _mm_unpacklo_epi64(decay(x), decay(x));

        auto quarter0 = _mm_cvtepi16_epi64(half0);
        auto quarter1 = _mm_cvtepi16_epi64(_mm_srli_si128(half0, 0x4));
        auto quarter2 = _mm_cvtepi16_epi64(half1);
        auto quarter3 = _mm_cvtepi16_epi64(_mm_srli_si128(half1, 0x4));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quarter0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), quarter1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quarter2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), quarter3);

        #elif defined(AVEL_SSE2)
        auto lo = _mm_unpacklo_epi16(decay(x), _mm_setzero_si128());
        auto hi = _mm_unpackhi_epi16(decay(x), _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi32(lo, _mm_setzero_si128());
        auto quarter1 = _mm_unpackhi_epi32(lo, _mm_setzero_si128());
        auto quarter2 = _mm_unpacklo_epi32(hi, _mm_setzero_si128());
        auto quarter3 = _mm_unpackhi_epi32(hi, _mm_setzero_si128());

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), quarter0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), quarter1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), quarter2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), quarter3);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 8> convert<vec1x32f, vec8x16u>(vec8x16u x) {
        alignas(32) std::array<vec1x32f, 8> ret;
        #if defined(AVEL_AVX2)
        auto t0 = _mm256_cvtepi32_ps(_mm256_cvtepu16_epi32(decay(x)));
        _mm256_store_ps(reinterpret_cast<float*>(ret.data()), t0);

        #elif defined(AVEL_AVX)
        auto half0 = _mm_cvtepu16_epi32(decay(x));
        auto half1 = _mm_cvtepu16_epi32(_mm_unpackhi_epi64(decay(x), decay(x)));
        _mm256_store_ps(reinterpret_cast<float*>(ret.data()), _mm256_cvtepi32_ps(_mm256_set_m128i(half0, half1)));

        #elif defined(AVEL_SSE41)
        auto half0 = _mm_cvtepi32_ps(_mm_cvtepu16_epi32(decay(x)));
        auto half1 = _mm_cvtepi32_ps(_mm_cvtepu16_epi32(_mm_unpackhi_epi64(decay(x), decay(x))));

        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), half0);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x4), half1);

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_cvtepi32_ps(_mm_unpacklo_epi16(decay(x), _mm_setzero_si128()));
        auto half1 = _mm_cvtepi32_ps(_mm_unpackhi_epi16(decay(x), _mm_setzero_si128()));

        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), half0);
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x4), half1);

        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64f, 8> convert<vec1x64f, vec8x16u>(vec8x16u x) {
        alignas(64) std::array<vec1x64f, 8> ret;
        #if defined(AVEL_AVX2)
        auto half0 = _mm_cvtepu16_epi32(decay(x));
        auto half1 = _mm_unpackhi_epi64(decay(x), decay(x));

        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), _mm256_cvtepi32_pd(half0));
        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), _mm256_cvtepi32_pd(half1));

        #elif defined(AVEL_SSE41)
        auto half0 = _mm_cvtepu16_epi32(decay(x));
        auto half1 = _mm_cvtepu16_epi32(_mm_unpackhi_epi64(decay(x), decay(x)));

        auto quarter0 = half0;
        auto quarter1 = _mm_srli_si128(half0, 4);
        auto quarter2 = half1;
        auto quarter3 = _mm_srli_si128(half1, 4);

        auto out0 = _mm_cvtepi32_pd(quarter0);
        auto out1 = _mm_cvtepi32_pd(quarter1);
        auto out2 = _mm_cvtepi32_pd(quarter2);
        auto out3 = _mm_cvtepi32_pd(quarter3);

        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), out0);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x2), out1);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), out2);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x6), out3);

        #elif defined(AVEL_SSE2)
        auto half0 = _mm_unpacklo_epi16(decay(x), _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi16(decay(x), _mm_setzero_si128());

        auto quarter0 = half0;
        auto quarter1 = _mm_srli_si128(half0, 4);
        auto quarter2 = half1;
        auto quarter3 = _mm_srli_si128(half1, 4);

        auto out0 = _mm_cvtepi32_pd(quarter0);
        auto out1 = _mm_cvtepi32_pd(quarter1);
        auto out2 = _mm_cvtepi32_pd(quarter2);
        auto out3 = _mm_cvtepi32_pd(quarter3);

        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), out0);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x2), out1);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x3), out2);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x6), out3);

        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec8x16u>(vec8x16u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{_mm_packus_epi16(_mm_and_si128(decay(x), _mm_set1_epi16(0xFF)), _mm_setzero_si128())}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec8x16u>(vec8x16u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_packus_epi16(_mm_and_si128(decay(x), _mm_set1_epi16(0xFF)), _mm_setzero_si128())}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x8u>(vec1x8u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x8i>(vec1x8i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x16u>(vec1x16u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x16i>(vec1x16i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)))}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x32u>(vec1x32u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)) & 0xFFFF)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x32i>(vec1x32i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)) & 0xFFFF)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x64u>(vec1x64u x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)) & 0xFFFF)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x64i>(vec1x64i x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)) & 0xFFFF)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x32f>(vec1x32f x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)) & 0xFFFF)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 1> convert<vec8x16u, vec1x64f>(vec1x64f x) {
        #if defined(AVEL_SSE2)
        return {vec8x16u{_mm_cvtsi32_si128(std::uint16_t(decay(x)) & 0xFFFF)}};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 2> convert<vec8x16u, vec16x8u>(vec16x8u x) {
        #if defined(AVEL_AVX2)
        auto t0 = _mm256_cvtepu8_epi16(decay(x));
        return {
            vec8x16u{_mm256_castsi256_si128(t0)},
            vec8x16u{_mm256_extracti128_si256(t0, 0x1)}
        };

        #elif defined(AVEL_SSE41)
        return {
            vec8x16u{_mm_cvtepu8_epi16(decay(x))},
            vec8x16u{_mm_cvtepu8_epi16(_mm_unpacklo_epi64(decay(x), decay(x)))}
        };

        #elif defined(AVEL_SSE2)
        return {
            vec8x16u{_mm_unpacklo_epi8(decay(x), _mm_setzero_si128())},
            vec8x16u{_mm_unpackhi_epi8(decay(x), _mm_setzero_si128())}
        };

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec8x16u, 2> convert<vec8x16u, vec16x8i>(vec16x8i x) {
        #if defined(AVEL_AVX2)
        auto t0 = _mm256_cvtepi8_epi16(decay(x));
        return {
            vec8x16u{_mm256_castsi256_si128(t0)},
            vec8x16u{_mm256_extracti128_si256(t0, 0x1)}
        };

        #elif defined(AVEL_SSE41)
        return {
            vec8x16u{_mm_cvtepi8_epi16(decay(x))},
            vec8x16u{_mm_cvtepi8_epi16(_mm_unpackhi_epi64(decay(x), decay(x)))}
        };

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        return {
            vec8x16u{_mm_unpacklo_epi8(decay(x), wholeb)},
            vec8x16u{_mm_unpackhi_epi8(decay(x), wholeb)}
        };

        #endif
    }

}

#endif //AVEL_VEC8X16U_HPP
