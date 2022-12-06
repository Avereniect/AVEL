#ifndef AVEL_MASK8XT_HPP
#define AVEL_MASK8XT_HPP

namespace avel_impl {

    class Vector_mask8xT {
    public:

        //=================================================
        // Type aliases
        //=================================================

        using primitive = __mmask8;

    protected:

        //=================================================
        // Instance members
        //=================================================

        primitive content;

    public:

        //=================================================
        // Constructor
        //=================================================

        AVEL_FINL explicit Vector_mask8xT(primitive p):
            content(p) {}

        AVEL_FINL explicit Vector_mask8xT(bool x):
            content(x ? _cvtu32_mask16(0xFF) : _cvtu32_mask16(0x00)) {}

        Vector_mask8xT() = default;
        Vector_mask8xT(const Vector_mask8xT&) = default;
        Vector_mask8xT(Vector_mask8xT&&) noexcept = default;
        ~Vector_mask8xT() = default;

        //=================================================
        // Assignment operators
        //=================================================

        AVEL_FINL Vector_mask8xT& operator=(bool x) {
            content = x ? 0xFF : 0x00;
            return *this;
        }

        AVEL_FINL Vector_mask8xT& operator=(primitive p) {
            content = p;
            return *this;
        }

        Vector_mask8xT& operator=(const Vector_mask8xT&) = default;
        Vector_mask8xT& operator=(Vector_mask8xT&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        AVEL_FINL friend bool operator==(Vector_mask8xT lhs, Vector_mask8xT rhs) noexcept {
            return lhs.content == rhs.content;
        }

        [[nodiscard]]
        AVEL_FINL friend bool operator!=(Vector_mask8xT lhs, Vector_mask8xT rhs) noexcept {
            return lhs.content != rhs.content;
        }

        //=================================================
        // Bitwise assignment operators
        //=================================================

        AVEL_FINL Vector_mask8xT& operator&=(Vector_mask8xT rhs) {
            content &= rhs.content;
            return *this;
        }

        AVEL_FINL Vector_mask8xT& operator|=(Vector_mask8xT rhs) {
            content |= rhs.content;
            return *this;
        }

        AVEL_FINL Vector_mask8xT& operator^=(Vector_mask8xT rhs) {
            content ^= rhs.content;
            return *this;
        }

        //=================================================
        // Bitwise operations
        //=================================================

        [[nodiscard]]
        AVEL_FINL Vector_mask8xT operator!() const {
            return Vector_mask8xT{primitive(~content)};
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask8xT operator&(Vector_mask8xT lhs, Vector_mask8xT rhs) {
            lhs &= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask8xT operator&&(Vector_mask8xT lhs, Vector_mask8xT rhs) {
            return lhs & rhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask8xT operator|(Vector_mask8xT lhs, Vector_mask8xT rhs) {
            lhs |= rhs;
            return lhs;
        }

        [[nodiscard]]
        AVEL_FINL friend Vector_mask8xT operator||(Vector_mask8xT lhs, Vector_mask8xT rhs) {
            return lhs | rhs;
        }

        //=================================================
        // Conversion operators
        //=================================================

        AVEL_FINL explicit operator primitive() const {
            return content;
        }

    };

}

#endif //AVEL_MASK8XT_HPP
