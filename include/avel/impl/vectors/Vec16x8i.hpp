#ifndef AVEL_VEC16X8I_HPP
#define AVEL_VEC16X8I_HPP

namespace avel {

    //=====================================================
    // Type aliases
    //=====================================================

    using vec16x8i = Vector<std::int8_t, 16>;

    using mask16x8i = Vector_mask<std::int8_t, 16>;

    //=====================================================
    // Forward declarations
    //=====================================================

    div_type<vec16x8i> div(vec16x8i numerator, vec16x8i denominator);
    vec16x8i broadcast_mask(mask16x8i m);
    vec16x8i blend(vec16x8i a, vec16x8i b, mask16x8i m);
    vec16x8i countl_one(vec16x8i x);





    template<>
    #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
    class Vector_mask<std::int8_t, 16> : public avel_impl::Vector_mask16xT {
        using base = avel_impl::Vector_mask16xT;
    #elif defined(AVEL_SSE2) || defined(AVEL_NEON)
    class Vector_mask<std::int8_t, 16> : public avel_impl::Vector_mask128b {
        using base = avel_impl::Vector_mask128b;
    #endif
    public:

        //=================================================
        // Static constants
        //=================================================

        static constexpr std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int8x16_t;
        #endif

        //=================================================
        // -ctors
        //=================================================

        using base::base;

        AVEL_FINL explicit Vector_mask(base b):
            base(b) {}

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vector_mask(primitive p):
            base(vreinterpretq_u8_s8(p)) {}
        #endif

        template<class U>
        AVEL_FINL explicit Vector_mask(Vector_mask<U, width> v):
            base(convert<Vector_mask>(v)[0]) {}

        AVEL_FINL explicit Vector_mask(const std::array<bool, 16>& arr) {
            static_assert(
                sizeof(bool) == 1,
                "Implementation assumes bool occupy a single byte"
            );

            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m128i array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = primitive(_mm_cmplt_epi8_mask(_mm_setzero_si128(), array_data));

            #elif defined(AVEL_SSE2)
            primitive array_data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(arr.data()));
            content = _mm_cmplt_epi8(_mm_setzero_si128(), array_data);

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
        // Bitwise operations
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

        //=================================================
        // Conversion operators
        //=================================================

        #if defined(AVEL_NEON)
        [[nodiscard]]
        AVEL_FINL explicit operator int8x16_t() const {
            return vreinterpretq_s8_u8(content);
        }
        #endif

    };

    //=====================================================
    // Mask functions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::uint32_t count(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return popcount(_mm512_mask2int(decay(m)));
        #elif defined(AVEL_SSE2)
        return popcount(_mm_movemask_epi8(decay(m)));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool any(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return _mm512_mask2int(decay(m));
        #elif defined(AVEL_SSE2)
        return _mm_movemask_epi8(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool all(mask16x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        auto t0 = _mm512_mask2int(decay(m));
        return 0xFFFF == t0;
        #elif defined(AVEL_SSE41)
        return _mm_test_all_ones(decay(m));
        #elif defined(AVEL_SSE2)
        return 0xFFFF == _mm_movemask_epi8(decay(m));
        #endif
    }

    [[nodiscard]]
    AVEL_FINL bool none(mask16x8i m) {
        return !any(m);
    }

    //=====================================================
    // Mask conversions
    //=====================================================

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask16x8i>(mask16x8i m) {
        return std::array<mask16x8i, 1>{m};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8u, 16> convert<mask1x8u, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x8u, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x8i, 16> convert<mask1x8i, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x8i, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16u, 16> convert<mask1x16u, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x16u, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x16i, 16> convert<mask1x16i, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x16i, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32u, 16> convert<mask1x32u, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x32u, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32i, 16> convert<mask1x32i, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x32i, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64u, 16> convert<mask1x64u, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x64u, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64i, 16> convert<mask1x64i, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x64i, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x32f, 16> convert<mask1x32f, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x32f, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask1x64f, 16> convert<mask1x64f, mask16x8i>(mask16x8i m) {
        alignas(16) std::array<mask1x64f, 16> ret;
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), _mm_movm_epi8(decay(m)));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);

        #elif defined(AVEL_SSE2)
        __m128i reg = _mm_sub_epi8(_mm_setzero_si128(), decay(m));
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), reg);
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8u, 1> convert<mask16x8u, mask16x8i>(mask16x8i m) {
        return std::array<mask16x8u, 1>{mask16x8u(mask16x8u::primitive(decay(m)))};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x8u>(mask1x8u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x8i>(mask1x8i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x16u>(mask1x16u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x16i>(mask1x16i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x32u>(mask1x32u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x32i>(mask1x32i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x64u>(mask1x64u m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x64i>(mask1x64i m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x32f>(mask1x32f m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask1x64f>(mask1x64f m) {
        #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
        return std::array<mask16x8i, 1>{mask16x8i{mask16x8i::primitive(decay(m))}};

        #elif defined(AVEL_SSE2)
        return std::array<mask16x8i, 1>{mask16x8i{_mm_cvtsi32_si128(-decay(m) & 0xFF)}};

        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<mask16x8i, 1> convert<mask16x8i, mask16x8u>(mask16x8u m) {
        return std::array<mask16x8i, 1>{mask16x8i(decay(m))};
    }






    template<>
    class alignas(16) Vector<std::int8_t, 16> : public avel_impl::Vec16x8int {
        using base = avel_impl::Vec16x8int;
    public:

        //=================================================
        // Static constants
        //=================================================

        constexpr static std::uint32_t width = 16;

        //=================================================
        // Type aliases
        //=================================================

        using scalar = std::int8_t;

        #if defined(AVEL_SSE2)
        using primitive = base::primitive;
        #endif

        #if defined(AVEL_NEON)
        using primitive = int8x16_t;
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

        template<class U>
        AVEL_FINL explicit Vector(Vector<U, width> v):
            base(convert<Vector>(v)[0]) {}

        #if defined(AVEL_NEON)
        AVEL_FINL explicit Vector(primitive p):
            base(p) {}
        #endif

        AVEL_FINL explicit Vector(scalar x):
        #if defined(AVEL_AVX2)
            base(_mm_broadcastb_epi8(_mm_cvtsi32_si128(x))) {}
        #elif defined(AVEL_SSE2)
            base(_mm_set1_epi8(x)) {}
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
            #if defined(AVEL_AVX2)
            content = _mm_broadcastb_epi8(_mm_cvtsi32_si128(x));
            #elif defined(AVEL_SSE2)
            content = _mm_set1_epi8(x);
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
            return mask{base(lhs) == base(rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator!=(Vector lhs, Vector rhs) {
            return mask{base(lhs) != base(rhs)};
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmplt_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmplt_epi8(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator<=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmple_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmpgt_epi8(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpgt_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return mask{_mm_cmpgt_epi8(lhs.content, rhs.content)};
            #endif
        }

        [[nodiscard]]
        AVEL_FINL friend mask operator>=(Vector lhs, Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            return mask{_mm_cmpge_epi8_mask(lhs.content, rhs.content)};
            #elif defined(AVEL_SSE2)
            return !mask{_mm_cmplt_epi8(lhs.content, rhs.content)};
            #endif

            #if defined(AVEL_NEON)
            //TODO: Implement
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
            return Vector{0x00} - *this;
        }

        //=================================================
        // Arithmetic assignment operators
        //=================================================

        AVEL_FINL Vector& operator+=(Vector rhs) {
            base::operator+=(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator-=(Vector rhs) {
            base::operator-=(rhs);
            return *this;
        }

        AVEL_FINL Vector& operator*=(Vector rhs) {
            base::operator*=(rhs);
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
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            __m256i tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sra_epi16(tmp, _mm_cvtsi32_si128(rhs));
            content = _mm256_cvtepi16_epi8(tmp);

            #elif defined(AVEL_AVX2)
            __m256i tmp = _mm256_cvtepi8_epi16(content);
            tmp = _mm256_sra_epi16(tmp, _mm_cvtsi32_si128(rhs));

            __m128i lo = _mm256_castsi256_si128(tmp);
            __m128i hi = _mm256_extractf128_si256(tmp, 0x1);

            content = _mm_packs_epi16(lo, hi);

            #elif defined(AVEL_SSE2)
            primitive sign_bits = _mm_cmplt_epi8(content, _mm_setzero_si128());
            primitive lo = _mm_unpacklo_epi8(content, sign_bits);
            primitive hi = _mm_unpackhi_epi8(content, sign_bits);

            primitive s = _mm_cvtsi32_si128(rhs);
            primitive lo_shifted = _mm_sra_epi16(lo, s);
            primitive hi_shifted = _mm_sra_epi16(hi, s);

            primitive result = _mm_packs_epi16(lo_shifted, hi_shifted);

            content = result;

            #endif
            return *this;
        }

        AVEL_FINL Vector& operator>>=(Vector rhs) {
            #if defined(AVEL_AVX512VL) && defined(AVEL_AVX512BW)
            auto whole = _mm256_cvtepi8_epi16(content);
            auto shifts = _mm256_cvtepi8_epi16(rhs.content);
            auto shifted = _mm256_srav_epi16(whole, shifts);
            content = _mm256_cvtepi16_epi8(shifted);

            #elif defined(AVEL_AVX2)
            auto lhs_whole = _mm256_cvtepi8_epi16(content);
            auto rhs_whole = _mm256_cvtepi8_epi16(rhs.content);

            auto shifted = _mm256_srav_epi16(lhs_whole, rhs_whole);
            auto packed = _mm256_packs_epi16(shifted, _mm256_zextsi128_si256(_mm256_extractf128_si256(shifted, 0x1)));

            content = _mm256_castsi256_si128(packed);

            #elif defined(AVEL_SSE2)
            //TODO: Offer alternative approach
            alignas(16) std::int8_t data[16];
            alignas(16) std::int8_t shifts[16];

            _mm_store_si128(reinterpret_cast<__m128i*>(data), content);
            _mm_store_si128(reinterpret_cast<__m128i*>(shifts), rhs.content);

            data[0x0] >>= shifts[0x0];
            data[0x1] >>= shifts[0x1];
            data[0x2] >>= shifts[0x2];
            data[0x3] >>= shifts[0x3];
            data[0x4] >>= shifts[0x4];
            data[0x5] >>= shifts[0x5];
            data[0x6] >>= shifts[0x6];
            data[0x7] >>= shifts[0x7];
            data[0x8] >>= shifts[0x8];
            data[0x9] >>= shifts[0x9];
            data[0xa] >>= shifts[0xa];
            data[0xb] >>= shifts[0xb];
            data[0xc] >>= shifts[0xc];
            data[0xd] >>= shifts[0xd];
            data[0xe] >>= shifts[0xe];
            data[0xf] >>= shifts[0xf];

            content = _mm_load_si128(reinterpret_cast<const __m128i*>(data));

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
            return vreinterpretq_s8_u8(content);
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

    AVEL_FINL vec16x8i operator-(vec16x8u v) {
        return vec16x8i{} - vec16x8i{v};
    }

    //=====================================================
    // General vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL vec16x8i broadcast_mask(mask16x8i m) {
        return vec16x8i{broadcast_mask(mask16x8u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i blend(vec16x8i a, vec16x8i b, mask16x8i m) {
        return vec16x8i{blend(vec16x8u{a}, vec16x8u{b}, mask16x8u{m})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i max(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE41)
        return vec16x8i{_mm_max_epi8(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        auto mask = a < b;
        return blend(a, b, mask);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i min(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE41)
        return vec16x8i{_mm_min_epi8(decay(a), decay(b))};

        #elif defined(AVEL_SSE2)
        auto mask = a < b;
        return blend(b, a, mask);

        #endif
    }

    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 2> minmax(vec16x8i a, vec16x8i b) {
        #if defined(AVEL_SSE41)
        return {
            vec16x8i{_mm_min_epi8(decay(a), decay(b))},
            vec16x8i{_mm_max_epi8(decay(a), decay(b))}
        };

        #elif defined(AVEL_SSE2)
        auto mask = a < b;

        return {
            blend(b, a, mask),
            blend(a, b, mask)
        };

        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i clamp(vec16x8i x, vec16x8i lo, vec16x8i hi) {
        return min(max(x, lo), hi);
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i average(vec16x8i x, vec16x8i y) {
        #if defined(AVEL_AVX512VL)
        auto z = (x >> 1) + (y >> 1);
        auto c = _mm_cmplt_epi8(decay(z), _mm_setzero_si128());
        auto d = _mm_ternarylogic_epi32(decay(x), decay(y), c, 0xE8);

        return z + (vec16x8i{d} & vec16x8i{0x1});

        #elif defined(AVEL_SSE2)
        auto z = (x >> 1) + (y >> 1);
        auto c = z < vec16x8i{0};
        auto d = (x & y) | (broadcast_mask(c) & (x ^ y));

        return z + (d & vec16x8i{0x1});

        #endif

        #if defined(AVEL_NEON)
        return vec16x8i{vhaddq_s8(decay(a), decay(b))};
        #endif
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i midpoint(vec16x8i a, vec16x8i b) {
        vec16x8i addition_mask{std::int8_t(0x80)};
        a ^= addition_mask;
        b ^= addition_mask;

        return vec16x8i{midpoint(vec16x8u{a}, vec16x8u{b})} ^ addition_mask;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i abs(vec16x8i x) {
        auto y = x >> 7;
        return (x ^ y) - y;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i neg_abs(vec16x8i x) {
        auto y = ~x >> 7;
        return (x ^ y) - y;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i neg_abs(vec16x8u x) {
        return neg_abs(vec16x8i{x});
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i load<vec16x8i>(const std::int8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8i{_mm_loadu_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL vec16x8i aligned_load<vec16x8i>(const std::int8_t* ptr) {
        #if defined(AVEL_SSE2)
        return vec16x8i{_mm_load_si128(reinterpret_cast<const __m128i*>(ptr))};
        #endif
    }

    AVEL_FINL void store(std::int8_t* ptr, vec16x8i x) {
        #if defined(AVEL_SSE2)
        _mm_storeu_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif
    }

    AVEL_FINL void aligned_store(std::int8_t* ptr, vec16x8i x) {
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ptr), decay(x));
        #endif
    }

    //=====================================================
    // Integer vector operations
    //=====================================================

    [[nodiscard]]
    AVEL_FINL div_type<vec16x8i> div(vec16x8i x, vec16x8i y) {
        vec16x8i quotient{0x00};

        mask16x8i sign_mask0 = (x < vec16x8i{});
        mask16x8i sign_mask1 = (y < vec16x8i{});

        mask16x8i sign_mask2 = sign_mask0 ^ sign_mask1;

        vec16x8u numerator{abs(x)};
        vec16x8u denominator{abs(y)};

        //TODO: Compute i more appropriately?
        std::int32_t i = 8;

        //TODO: Consider performing division with abs_neg values
        for (; (i-- > 0) && any(mask16x8u(numerator));) {
            mask16x8u b = ((numerator >> i) >= denominator);
            numerator -= (broadcast_mask(b) & (denominator << i));
            quotient |= (vec16x8u{b} << i);
        }

        //Adjust quotient's sign. Should be xor of operands' signs
        quotient = blend(quotient, -quotient, sign_mask2);

        //Adjust numerator's sign. Should be same sign as it was originally
        x = blend(vec16x8i{numerator}, -vec16x8i{numerator}, sign_mask0);

        return {quotient, x};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i popcount(vec16x8i x) {
        return vec16x8i{popcount(vec16x8u(x))};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i byteswap(vec16x8i x) {
        return x;
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i countl_zero(vec16x8i x) {
        return vec16x8i{countl_zero(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i countl_one(vec16x8i x) {
        return vec16x8i{countl_one(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i countr_zero(vec16x8i x) {
        return vec16x8i{countr_zero(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i countr_one(vec16x8i x) {
        return vec16x8i{countr_one(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL mask16x8i has_single_bit(vec16x8i x) {
        return mask16x8i{has_single_bit(vec16x8u{x})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotl(vec16x8i x, long long s) {
        return vec16x8i{rotl(vec16x8u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotl(vec16x8i x, vec16x8i s) {
        return vec16x8i{rotl(vec16x8u{x}, vec16x8u{s})};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotr(vec16x8i x, long long s) {
        return vec16x8i{rotr(vec16x8u{x}, s)};
    }

    [[nodiscard]]
    AVEL_FINL vec16x8i rotr(vec16x8i x, vec16x8i s) {
        return vec16x8i{rotr(vec16x8u{x}, vec16x8u{s})};
    }

    //=====================================================
    // Vector conversions
    //=====================================================

    [[nodiscard]]
    AVEL_FINL std::array<std::int8_t, 16> to_array(vec16x8i v) {
        alignas(16) std::array<std::int8_t, 16> ret;
        aligned_store(ret.data(), v);
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec16x8i>(vec16x8i x) {
        return std::array<vec16x8i, 1>{x};
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8u, 16> convert<vec1x8u, vec16x8i>(vec16x8i x) {
        alignas(16) std::array<vec1x8u, 16> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(x));
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x8i, 16> convert<vec1x8i, vec16x8i>(vec16x8i x) {
        alignas(16) std::array<vec1x8i, 16> ret;
        #if defined(AVEL_SSE2)
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data()), decay(x));
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16u, 16> convert<vec1x16u, vec16x8i>(vec16x8i x) {
        alignas(32) std::array<vec1x16u, 16> ret;
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepi8_epi16(decay(x)));

        #elif defined(AVEL_SSE41)
        auto half0 = decay(x);
        auto half1 = _mm_unpacklo_epi64(half0, half0);

        auto out0 = _mm_cvtepi8_epi16(half0);
        auto out1 = _mm_cvtepi8_epi16(half1);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), out0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out1);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        auto out0 = _mm_unpacklo_epi8(whole, wholeb);
        auto out1 = _mm_unpackhi_epi8(whole, wholeb);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), out0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out1);
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x16i, 16> convert<vec1x16i, vec16x8i>(vec16x8i x) {
        alignas(32) std::array<vec1x16i, 16> ret;
        #if defined(AVEL_AVX2)
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data()), _mm256_cvtepi8_epi16(decay(x)));

        #elif defined(AVEL_SSE41)
        auto half0 = decay(x);
        auto half1 = _mm_unpacklo_epi64(half0, half0);

        auto out0 = _mm_cvtepi8_epi16(half0);
        auto out1 = _mm_cvtepi8_epi16(half1);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), out0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out1);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        auto out0 = _mm_unpacklo_epi8(whole, wholeb);
        auto out1 = _mm_unpackhi_epi8(whole, wholeb);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), out0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out1);
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32u, 16> convert<vec1x32u, vec16x8i>(vec16x8i x) {
        alignas(64) std::array<vec1x32u, 16> ret;
        #if defined(AVEL_AVX512F)
        _mm512_store_si512(ret.data(), _mm512_cvtepi8_epi32(decay(x)));

        #elif defined(AVEL_AVX2)
        auto half0 = decay(x);
        auto half1 = _mm_unpackhi_epi64(half0, half0);

        auto out0 = _mm256_cvtepi8_epi32(half0);
        auto out1 = _mm256_cvtepi8_epi32(half1);

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), out0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x8), out1);

        #elif defined(AVEL_SSE41)
        auto half0 = decay(x);
        auto half1 = _mm_unpackhi_epi64(half0, half0);

        auto quarter0 = half0;
        auto quarter1 = _mm_srli_si128(half0, 4);
        auto quarter2 = half1;
        auto quarter3 = _mm_srli_si128(half1, 4);

        auto out0 = _mm_cvtepi8_epi32(quarter0);
        auto out1 = _mm_cvtepi8_epi32(quarter1);
        auto out2 = _mm_cvtepi8_epi32(quarter2);
        auto out3 = _mm_cvtepi8_epi32(quarter3);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out3);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi8(whole, wholeb);
        auto half0b = _mm_cmplt_epi16(half0, _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(whole, wholeb);
        auto half1b = _mm_cmplt_epi16(half1, _mm_setzero_si128());

        auto out0 = _mm_unpacklo_epi16(half0, half0b);
        auto out1 = _mm_unpackhi_epi16(half0, half0b);
        auto out2 = _mm_unpacklo_epi16(half1, half1b);
        auto out3 = _mm_unpackhi_epi16(half1, half1b);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out3);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32i, 16> convert<vec1x32i, vec16x8i>(vec16x8i x) {
        alignas(64) std::array<vec1x32i, 16> ret;
        #if defined(AVEL_AVX512F)
        _mm512_store_si512(ret.data(), _mm512_cvtepi8_epi32(decay(x)));

        #elif defined(AVEL_AVX2)
        auto half0 = decay(x);
        auto half1 = _mm_unpackhi_epi64(half0, half0);

        auto out0 = _mm256_cvtepi8_epi32(half0);
        auto out1 = _mm256_cvtepi8_epi32(half1);

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), out0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x8), out1);

        #elif defined(AVEL_SSE41)
        auto half0 = decay(x);
        auto half1 = _mm_unpackhi_epi64(half0, half0);

        auto quarter0 = half0;
        auto quarter1 = _mm_srli_si128(half0, 4);
        auto quarter2 = half1;
        auto quarter3 = _mm_srli_si128(half1, 4);

        auto out0 = _mm_cvtepi8_epi32(quarter0);
        auto out1 = _mm_cvtepi8_epi32(quarter1);
        auto out2 = _mm_cvtepi8_epi32(quarter2);
        auto out3 = _mm_cvtepi8_epi32(quarter3);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out3);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi8(whole, wholeb);
        auto half0b = _mm_cmplt_epi16(half0, _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(whole, wholeb);
        auto half1b = _mm_cmplt_epi16(half1, _mm_setzero_si128());

        auto out0 = _mm_unpacklo_epi16(half0, half0b);
        auto out1 = _mm_unpackhi_epi16(half0, half0b);
        auto out2 = _mm_unpacklo_epi16(half1, half1b);
        auto out3 = _mm_unpackhi_epi16(half1, half1b);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), out3);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64u, 16> convert<vec1x64u, vec16x8i>(vec16x8i x) {
        alignas(128) std::array<vec1x64u, 16> ret;
        #if defined(AVEL_AVX512F)
        auto half0 = _mm512_cvtepi8_epi64(decay(x));
        auto half1 = _mm512_cvtepi8_epi64(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm512_store_si512(ret.data() + 0x0, half0);
        _mm512_store_si512(ret.data() + 0x8, half1);

        #elif defined(AVEL_AVX2)
        __m128i whole = decay(x);

        __m256i quarter0 = _mm256_cvtepi8_epi64(whole);
        __m256i quarter1 = _mm256_cvtepi8_epi64(_mm_srli_si128(whole, 0x4));
        __m256i quarter2 = _mm256_cvtepi8_epi64(_mm_unpackhi_epi64(whole, whole));
        __m256i quarter3 = _mm256_cvtepi8_epi64(_mm_srli_si128(whole, 0xC));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), quarter0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x4), quarter1);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x8), quarter2);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0xC), quarter3);

        #elif defined(AVEL_SSE41)
        auto half0 = decay(x);
        auto half1 = _mm_unpackhi_epi64(half0, half0);

        auto eighth0 = _mm_cvtepi8_epi64(half0);
        auto eighth1 = _mm_cvtepi8_epi64(_mm_srli_si128(half0, 0x2));
        auto eighth2 = _mm_cvtepi8_epi64(_mm_shufflelo_epi16(half1, 0x3));
        auto eighth3 = _mm_cvtepi8_epi64(_mm_srli_si128(half0, 0x4));
        auto eighth4 = _mm_cvtepi8_epi64(half1);
        auto eighth5 = _mm_cvtepi8_epi64(_mm_srli_si128(half1, 0x2));
        auto eighth6 = _mm_cvtepi8_epi64(_mm_shufflelo_epi16(half1, 0x3));
        auto eighth7 = _mm_cvtepi8_epi64(_mm_srli_si128(half1, 0x4));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), eighth0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), eighth1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), eighth2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), eighth3);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), eighth4);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xA), eighth5);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), eighth6);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xE), eighth7);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi8(whole, wholeb);
        auto half0b = _mm_cmplt_epi8(half0, _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(whole, wholeb);
        auto half1b = _mm_cmplt_epi8(half1, _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi16(half0, half0b);
        auto quarter0d = _mm_cmplt_epi16(quarter0, _mm_setzero_si128());
        auto quarter1 = _mm_unpackhi_epi16(half0, half0b);
        auto quarter1d = _mm_cmplt_epi16(quarter1, _mm_setzero_si128());
        auto quarter2 = _mm_unpacklo_epi16(half1, half1b);
        auto quarter2d = _mm_cmplt_epi16(quarter2, _mm_setzero_si128());
        auto quarter3 = _mm_unpackhi_epi16(half1, half1b);
        auto quarter3d = _mm_cmplt_epi16(quarter3, _mm_setzero_si128());

        auto eighth0 = _mm_unpacklo_epi32(quarter0, quarter0d);
        auto eighth1 = _mm_unpackhi_epi32(quarter0, quarter0d);
        auto eighth2 = _mm_unpacklo_epi32(quarter1, quarter1d);
        auto eighth3 = _mm_unpackhi_epi32(quarter1, quarter1d);
        auto eighth4 = _mm_unpacklo_epi32(quarter2, quarter2d);
        auto eighth5 = _mm_unpackhi_epi32(quarter2, quarter2d);
        auto eighth6 = _mm_unpacklo_epi32(quarter3, quarter3d);
        auto eighth7 = _mm_unpackhi_epi32(quarter3, quarter3d);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), eighth0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), eighth1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), eighth2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), eighth3);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), eighth4);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xA), eighth5);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), eighth6);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xE), eighth7);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64i, 16> convert<vec1x64i, vec16x8i>(vec16x8i x) {
        alignas(128) std::array<vec1x64i, 16> ret;
        #if defined(AVEL_AVX512F)
        __m512i half0 = _mm512_cvtepi8_epi64(decay(x));
        __m512i half1 = _mm512_cvtepi8_epi64(_mm_unpackhi_epi64(decay(x), decay(x)));

        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data() + 0x0), half0);
        _mm512_store_si512(reinterpret_cast<__m512i*>(ret.data() + 0x4), half1);

        #elif defined(AVEL_AVX2)
        __m128i whole = decay(x);

        __m256i quarter0 = _mm256_cvtepi8_epi64(whole);
        __m256i quarter1 = _mm256_cvtepi8_epi64(_mm_srli_si128(whole, 0x4));
        __m256i quarter2 = _mm256_cvtepi8_epi64(_mm_unpackhi_epi64(whole, whole));
        __m256i quarter3 = _mm256_cvtepi8_epi64(_mm_srli_si128(whole, 0xC));

        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x0), quarter0);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x4), quarter1);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0x8), quarter2);
        _mm256_store_si256(reinterpret_cast<__m256i*>(ret.data() + 0xC), quarter3);

        #elif defined(AVEL_SSE41)
        __m128i whole = decay(x);

        __m128i eighth0 = _mm_cvtepi8_epi64(whole);
        __m128i eighth1 = _mm_cvtepi8_epi64(_mm_srli_si128(whole, 0x2));
        __m128i eighth2 = _mm_cvtepi8_epi64(_mm_shuffle_epi32(whole, 0x1));
        __m128i eighth3 = _mm_cvtepi8_epi64(_mm_srli_si128(whole, 0x6));
        __m128i eighth4 = _mm_cvtepi8_epi64(_mm_unpackhi_epi64(whole, whole));
        __m128i eighth5 = _mm_cvtepi8_epi64(_mm_srli_si128(whole, 0xA));
        __m128i eighth6 = _mm_cvtepi8_epi64(_mm_shuffle_epi32(whole, 0x3));
        __m128i eighth7 = _mm_cvtepi8_epi64(_mm_srli_si128(whole, 0xE));

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), eighth0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), eighth1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), eighth2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), eighth3);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), eighth4);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xA), eighth5);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), eighth6);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xE), eighth7);

        #elif defined(AVEL_SSE2)
        __m128i zeros = _mm_setzero_si128();

        __m128i whole = decay(x);
        __m128i wholeb = _mm_cmplt_epi8(whole, zeros);

        __m128i half0 = _mm_unpacklo_epi8(whole, wholeb);
        __m128i halfb0 = _mm_cmplt_epi16(half0, zeros);
        __m128i half1 = _mm_unpackhi_epi8(whole, wholeb);
        __m128i halfb1 = _mm_cmplt_epi16(half1, zeros);

        __m128i quarter0 = _mm_unpacklo_epi16(half0, halfb0);
        __m128i quarterb0 = _mm_cmplt_epi32(quarter0, zeros);
        __m128i quarter1 = _mm_unpackhi_epi16(half0, halfb0);
        __m128i quarterb1 = _mm_cmplt_epi16(quarter1, zeros);
        __m128i quarter2 = _mm_unpacklo_epi16(half1, halfb1);
        __m128i quarterb2 = _mm_cmplt_epi16(quarter2, quarter2);
        __m128i quarter3 = _mm_unpackhi_epi16(half1, halfb1);
        __m128i quarterb3 = _mm_cmplt_epi16(quarter3, zeros);

        __m128i eighth0 = _mm_unpacklo_epi32(quarter0, quarterb0);
        __m128i eighth1 = _mm_unpackhi_epi32(quarter0, quarterb0);
        __m128i eighth2 = _mm_unpacklo_epi32(quarter1, quarterb1);
        __m128i eighth3 = _mm_unpackhi_epi32(quarter1, quarterb1);
        __m128i eighth4 = _mm_unpacklo_epi32(quarter2, quarterb2);
        __m128i eighth5 = _mm_unpackhi_epi32(quarter2, quarterb2);
        __m128i eighth6 = _mm_unpacklo_epi32(quarter3, quarterb3);
        __m128i eighth7 = _mm_unpackhi_epi32(quarter3, quarterb3);

        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x0), eighth0);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x2), eighth1);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x4), eighth2);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x6), eighth3);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0x8), eighth4);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xA), eighth5);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xC), eighth6);
        _mm_store_si128(reinterpret_cast<__m128i*>(ret.data() + 0xE), eighth7);

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif

        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x32f, 16> convert<vec1x32f, vec16x8i>(vec16x8i x) {
        alignas(64) std::array<vec1x32f, 16> ret;

        #if defined(AVEL_AVX512F)
        auto whole = _mm512_cvtepi8_epi32(decay(x));
        _mm512_store_ps(reinterpret_cast<float*>(ret.data()), _mm512_cvtepi32_ps(whole));

        #elif defined(AVEL_AVX)
        auto whole = decay(x);

        auto half0 = _mm256_cvtepi8_epi32(whole);
        auto half1 = _mm256_cvtepi8_epi32(_mm_unpackhi_epi64(whole, whole));

        _mm256_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), _mm256_cvtepi32_ps(half0));
        _mm256_store_ps(reinterpret_cast<float*>(ret.data() + 0x8), _mm256_cvtepi32_ps(half1));

        #elif defined(AVEL_SSE41)
        auto whole = decay(x);

        auto quarter0 = _mm_cvtepi8_epi32(whole);
        auto quarter1 = _mm_cvtepi8_epi32(_mm_srli_si128(whole, 0x4));
        auto quarter2 = _mm_cvtepi8_epi32(_mm_unpackhi_epi64(whole, whole));
        auto quarter3 = _mm_cvtepi8_epi32(_mm_srli_si128(whole, 0xC));

        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), _mm_cvtepi32_ps(quarter0));
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x4), _mm_cvtepi32_ps(quarter1));
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x8), _mm_cvtepi32_ps(quarter2));
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0xC), _mm_cvtepi32_ps(quarter3));

        #elif defined(AVEL_SSE2)
        auto zeros = _mm_setzero_si128();

        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, zeros);

        auto half0 = _mm_unpacklo_epi8(whole, wholeb);
        auto halfb0 = _mm_cmplt_epi16(half0, zeros);
        auto half1 = _mm_unpackhi_epi8(whole, wholeb);
        auto halfb1 = _mm_cmplt_epi16(half1, zeros);

        auto quarter0 = _mm_unpacklo_epi16(half0, halfb0);
        auto quarter1 = _mm_unpackhi_epi16(half0, halfb0);
        auto quarter2 = _mm_unpacklo_epi16(half1, halfb1);
        auto quarter3 = _mm_unpackhi_epi16(half1, halfb1);

        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x0), _mm_cvtepi32_ps(quarter0));
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x4), _mm_cvtepi32_ps(quarter1));
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0x8), _mm_cvtepi32_ps(quarter2));
        _mm_store_ps(reinterpret_cast<float*>(ret.data() + 0xC), _mm_cvtepi32_ps(quarter3));

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec1x64f, 16> convert<vec1x64f, vec16x8i>(vec16x8i x) {
        alignas(128) std::array<vec1x64f, 16> ret;

        #if defined(AVEL_AVX512F)
        auto whole = _mm512_cvtepi8_epi32(decay(x));

        auto half0 = _mm512_cvtepi32_pd(_mm512_extracti64x4_epi64(whole, 0x0));
        auto half1 = _mm512_cvtepi32_pd(_mm512_extracti64x4_epi64(whole, 0x1));

        _mm512_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), half0);
        _mm512_store_pd(reinterpret_cast<double*>(ret.data() + 0x8), half1);

        #elif defined(AVEL_AVX)
        auto whole = decay(x);

        auto half0 = _mm256_cvtepi8_epi32(whole);
        auto half1 = _mm256_cvtepi8_epi32(_mm_unpackhi_epi64(whole, whole));

        auto quarter0 = _mm256_cvtepi32_pd(_mm256_castsi256_si128(half0));
        auto quarter1 = _mm256_cvtepi32_pd(_mm256_extractf128_si256(half0, 0x1));
        auto quarter2 = _mm256_cvtepi32_pd(_mm256_castsi256_si128(half1));
        auto quarter3 = _mm256_cvtepi32_pd(_mm256_extractf128_si256(half1, 0x1));

        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), quarter0);
        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), quarter1);
        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0x8), quarter2);
        _mm256_store_pd(reinterpret_cast<double*>(ret.data() + 0xC), quarter3);

        #elif defined(AVEL_SSE41)
        auto whole = decay(x);

        auto quarter0 = _mm_cvtepi8_epi32(whole);
        auto quarter1 = _mm_cvtepi8_epi32(_mm_srli_si128(whole, 0x4));
        auto quarter2 = _mm_cvtepi8_epi32(_mm_unpackhi_epi64(whole, whole));
        auto quarter3 = _mm_cvtepi8_epi32(_mm_srli_si128(whole, 0xC));

        auto eighth0 = _mm_cvtepi32_pd(quarter0);
        auto eighth1 = _mm_cvtepi32_pd(_mm_unpackhi_epi64(quarter0, quarter0));
        auto eighth2 = _mm_cvtepi32_pd(quarter1);
        auto eighth3 = _mm_cvtepi32_pd(_mm_unpackhi_epi64(quarter1, quarter1));
        auto eighth4 = _mm_cvtepi32_pd(quarter2);
        auto eighth5 = _mm_cvtepi32_pd(_mm_unpackhi_epi64(quarter2, quarter2));
        auto eighth6 = _mm_cvtepi32_pd(quarter3);
        auto eighth7 = _mm_cvtepi32_pd(_mm_unpackhi_epi64(quarter3, quarter3));

        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), eighth0);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x2), eighth1);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), eighth2);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x6), eighth3);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x8), eighth4);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xA), eighth5);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xC), eighth6);
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xE), eighth7);

        #elif defined(AVEL_SSE2)
        auto whole = decay(x);
        auto wholeb = _mm_cmplt_epi8(whole, _mm_setzero_si128());

        auto half0 = _mm_unpacklo_epi8(whole, wholeb);
        auto half0b = _mm_cmplt_epi16(half0, _mm_setzero_si128());
        auto half1 = _mm_unpackhi_epi8(whole, wholeb);
        auto half1b = _mm_cmplt_epi16(half1, _mm_setzero_si128());

        auto quarter0 = _mm_unpacklo_epi16(half0, half0b);
        auto quarter1 = _mm_unpackhi_epi16(half0, half0b);
        auto quarter2 = _mm_unpacklo_epi16(half1, half1b);
        auto quarter3 = _mm_unpackhi_epi16(half1, half1b);

        auto eighth0 = quarter0;
        auto eighth1 = _mm_srli_si128(quarter0, 8);
        auto eighth2 = quarter1;
        auto eighth3 = _mm_srli_si128(quarter1, 8);
        auto eighth4 = quarter2;
        auto eighth5 = _mm_srli_si128(quarter2, 8);
        auto eighth6 = quarter3;
        auto eighth7 = _mm_srli_si128(quarter3, 8);

        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x0), _mm_cvtepi32_pd(eighth0));
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x2), _mm_cvtepi32_pd(eighth1));
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x4), _mm_cvtepi32_pd(eighth2));
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x6), _mm_cvtepi32_pd(eighth3));
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0x8), _mm_cvtepi32_pd(eighth4));
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xA), _mm_cvtepi32_pd(eighth5));
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xC), _mm_cvtepi32_pd(eighth6));
        _mm_store_pd(reinterpret_cast<double*>(ret.data() + 0xE), _mm_cvtepi32_pd(eighth7));

        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
        return ret;
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8u, 1> convert<vec16x8u, vec16x8i>(vec16x8i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8u{decay(x)}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x8u>(vec1x8u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x8i>(vec1x8i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x16u>(vec1x16u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x16i>(vec1x16i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x32u>(vec1x32u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x32i>(vec1x32i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x64u>(vec1x64u x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x64i>(vec1x64i x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x32f>(vec1x32f x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec1x64f>(vec1x64f x) {
        #if defined(AVEL_SSE2)
        return {vec16x8i{_mm_set1_epi8(std::int8_t(decay(x)))}};
        #endif

        #if defined(AVEL_NEON)
        //TODO: Implement
        #endif
    }

    template<>
    [[nodiscard]]
    AVEL_FINL std::array<vec16x8i, 1> convert<vec16x8i, vec16x8u>(vec16x8u x) {
        return {vec16x8i{vec16x8i::primitive(decay(x))}};
    }

}

#endif //AVEL_VEC16X8I_HPP
