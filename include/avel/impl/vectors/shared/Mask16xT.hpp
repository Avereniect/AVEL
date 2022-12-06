#ifndef AVEL_MASK16XT_HPP
#define AVEL_MASK16XT_HPP

namespace avel_impl {

    class Vector_mask16xT {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask16;

    protected:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask16xT(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask16xT(bool x):
            content(x ? _cvtu32_mask16(0xFFFF) : _cvtu32_mask16(0x0000)) {}

        AVEL_FINL explicit Vector_mask16xT(const std::array<bool, 16>& arr):
            content(
                _mm_cmplt_epu8_mask(
                    _mm_setzero_si128(),
                    _mm_load_si128(reinterpret_cast<const __m128i*>(arr.data()))
                )
            ) {}

        Vector_mask16xT() = default;
        Vector_mask16xT(const Vector_mask16xT&) = default;
        Vector_mask16xT(Vector_mask16xT&&) noexcept = default;
        ~Vector_mask16xT() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask16xT& operator=(bool x) {
            content = x ? _cvtu32_mask16(0xFFFF) : _cvtu32_mask16(0x0000);
            return *this;
        }

        AVEL_FINL Vector_mask16xT& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask16xT& operator=(const Vector_mask16xT&) = default;
        Vector_mask16xT& operator=(Vector_mask16xT&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask16xT lhs, Vector_mask16xT rhs) noexcept {
            return _mm512_mask2int(lhs.content) == _mm512_mask2int(rhs.content);
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask16xT lhs, Vector_mask16xT rhs) noexcept {
            return _mm512_mask2int(lhs.content) != _mm512_mask2int(rhs.content);
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask16xT& operator&=(Vector_mask16xT rhs) {
            content = _kand_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask16xT& operator|=(Vector_mask16xT rhs) {
            content = _kor_mask16(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask16xT& operator^=(Vector_mask16xT rhs) {
            content = _kxor_mask16(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask16xT operator!() const {
            return Vector_mask16xT{_knot_mask16(content)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask16xT operator&(Vector_mask16xT lhs, Vector_mask16xT rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask16xT operator&&(Vector_mask16xT lhs, Vector_mask16xT rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask16xT operator|(Vector_mask16xT lhs, Vector_mask16xT rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask16xT operator||(Vector_mask16xT lhs, Vector_mask16xT rhs) {
            return lhs | rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask16xT operator^(Vector_mask16xT lhs, Vector_mask16xT rhs) {
            lhs ^= rhs;
            return lhs;
        }

        //=================================================
        // Conversion operators
        //=================================================

        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

}

#endif //AVEL_MASK16XT_HPP
