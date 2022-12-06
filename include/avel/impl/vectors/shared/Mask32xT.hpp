#ifndef AVEL_MASK32XT_HPP
#define AVEL_MASK32XT_HPP

namespace avel_impl {

    class Vector_mask32xT {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask32;

    protected:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask32xT(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask32xT(bool x):
            content(x ? _cvtu32_mask32(-1) : _cvtu32_mask32(0)) {}

        AVEL_FINL explicit Vector_mask32xT(const std::array<bool, 32>& arr):
            content(
                _mm256_cmplt_epu8_mask(
                    _mm256_setzero_si256(),
                    _mm256_load_si256(reinterpret_cast<const __m256i*>(arr.data()))
                )
            ) {}

        Vector_mask32xT() = default;
        Vector_mask32xT(const Vector_mask32xT&) = default;
        Vector_mask32xT(Vector_mask32xT&&) noexcept = default;
        ~Vector_mask32xT() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask32xT& operator=(bool x) {
            content = x ? 0xFFFFFFFF : 0x00000000;
            return *this;
        }

        AVEL_FINL Vector_mask32xT& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask32xT& operator=(const Vector_mask32xT&) = default;
        Vector_mask32xT& operator=(Vector_mask32xT&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask32xT lhs, Vector_mask32xT rhs) noexcept {
            return _cvtmask32_u32(lhs.content) == _cvtmask32_u32(rhs.content);
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask32xT lhs, Vector_mask32xT rhs) noexcept {
            return _cvtmask32_u32(lhs.content) != _cvtmask32_u32(rhs.content);
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask32xT& operator&=(Vector_mask32xT rhs) {
            content = _kand_mask32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask32xT& operator|=(Vector_mask32xT rhs) {
            content = _kor_mask32(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask32xT& operator^=(Vector_mask32xT rhs) {
            content = _kxor_mask32(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask32xT operator!() const {
            return Vector_mask32xT{_knot_mask32(content)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask32xT operator&(Vector_mask32xT lhs, Vector_mask32xT rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask32xT operator&&(Vector_mask32xT lhs, Vector_mask32xT rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask32xT operator|(Vector_mask32xT lhs, Vector_mask32xT rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask32xT operator||(Vector_mask32xT lhs, Vector_mask32xT rhs) {
            return lhs | rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask32xT operator^(Vector_mask32xT lhs, Vector_mask32xT rhs) {
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

#endif //AVEL_MASK32XT_HPP
