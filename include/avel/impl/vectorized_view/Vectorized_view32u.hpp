//
// Created by avereniect on 8/4/22.
//

#ifndef AVEL_VECTOR_SPAN32U_HPP
#define AVEL_VECTOR_SPAN32U_HPP

namespace avel {

    using Vectorized_view32u = Vectorized_view<std::uint32_t, Dynamic_extent, 4>;

    template<std::size_t Extent, std::size_t Alignment>
    class Vectorized_view<const std::uint32_t, Extent, Alignment> : Vectorized_view_base<Extent> {
    public:

        static_assert(
            alignof(std::uint32_t) <= Alignment,
            "Alignment of view must be at least the alignment of std::uint32_t"
        );

        //=================================================
        // Type aliases
        //=================================================

        using element_type = std::uint32_t;

        using value_type = const std::uint32_t;

        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using pointer = const value_type*;
        using const_pointer = const value_type*;

        using reference = const std::uint32_t&;
        using const_reference = const std::uint32_t&;

        //=================================================
        // -ctors
        //=================================================

        Vectorized_view() = default;
        Vectorized_view(const Vectorized_view&) = default;
        Vectorized_view(Vectorized_view&&) noexcept = default;
        ~Vectorized_view() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vectorized_view& operator=(const Vectorized_view&) = default;
        Vectorized_view& operator=(Vectorized_view&&) noexcept = default;

        //=================================================
        // Accessors
        //=================================================

        //=================================================
        // Accessors
        //=================================================

        [[nodiscard]]
        const_pointer data() const {
            return data;
        }

    private:

        //=================================================
        // Instance members
        //=================================================

        const_pointer ptr = nullptr;

    };

    template<std::size_t Extent, std::size_t Alignment>
    class Vectorized_view<std::uint32_t, Extent, Alignment> : Vectorized_view_base<Extent> {
    public:

        //=================================================
        // -ctors
        //=================================================

        Vectorized_view();

        //=================================================
        // Assignment operators
        //=================================================

        //=================================================
        // Accessors
        //=================================================

    private:

        //=================================================
        // Instance members
        //=================================================



    };

}

#endif //AVEL_VECTOR_SPAN32U_HPP
