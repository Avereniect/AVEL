#ifndef AVEL_MASK64XT_HPP
#define AVEL_MASK64XT_HPP

namespace avel_impl {

    class Vector_mask64xT {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask64;

    protected:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask64xT(primitive content):
            content(content) {}

        AVEL_FINL explicit Vector_mask64xT(bool x):
            content(x ? _cvtu64_mask64(-1ll) : _cvtu64_mask64(0ll)) {}

        AVEL_FINL explicit Vector_mask64xT(const std::array<bool, 64>& arr):
            content(
                _mm512_cmplt_epu8_mask(
                    _mm512_setzero_si512(),
                    _mm512_load_si512(arr.data())
                )
            ) {}

        Vector_mask64xT() = default;
        Vector_mask64xT(const Vector_mask64xT&) = default;
        Vector_mask64xT(Vector_mask64xT&&) noexcept = default;
        ~Vector_mask64xT() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask64xT& operator=(bool x) {
            content = x ? 0xFFFFFFFFFFFFFFFF : 0x0000000000000000;
            return *this;
        }

        AVEL_FINL Vector_mask64xT& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask64xT& operator=(const Vector_mask64xT&) = default;
        Vector_mask64xT& operator=(Vector_mask64xT&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask64xT lhs, Vector_mask64xT rhs) noexcept {
            return _cvtmask64_u64(lhs.content) == _cvtmask64_u64(rhs.content);
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask64xT lhs, Vector_mask64xT rhs) noexcept {
            return _cvtmask64_u64(lhs.content) != _cvtmask64_u64(rhs.content);
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask64xT& operator&=(Vector_mask64xT rhs) {
            content = _kand_mask64(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask64xT& operator|=(Vector_mask64xT rhs) {
            content = _kor_mask64(content, rhs.content);
            return *this;
        }

        AVEL_FINL Vector_mask64xT& operator^=(Vector_mask64xT rhs) {
            content = _kxor_mask64(content, rhs.content);
            return *this;
        }

        //=================================================
        // Bitwise operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask64xT operator!() const {
            return Vector_mask64xT{_knot_mask64(content)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask64xT operator&(Vector_mask64xT lhs, Vector_mask64xT rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask64xT operator&&(Vector_mask64xT lhs, Vector_mask64xT rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask64xT operator|(Vector_mask64xT lhs, Vector_mask64xT rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask64xT operator||(Vector_mask64xT lhs, Vector_mask64xT rhs) {
            return lhs | rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask64xT operator^(Vector_mask64xT lhs, Vector_mask64xT rhs) {
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

#endif //AVEL_MASK64XT_HPP
