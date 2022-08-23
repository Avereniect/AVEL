namespace avel {

    template<std::size_t Extent, std::size_t Alignment>
    class Vector_span<const std::uint32_t, Extent, Alignment> : Vector_span_base<Extent> {
        using base = Vector_span_base<Extent>;
    public:

        //=================================================
        // Type aliases
        //=================================================

        using element_type = const std::uint32_t;
        using value_type = std::uint32_t;

        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using pointer = const std::uint32_t*;
        using const_pointer = const std::uint32_t*;

        using reference = const std::uint32_t&;
        using const_reference = const std::uint32_t&;

        using iterator = pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;

        //=================================================
        // Static members
        //=================================================

        static constexpr std::size_t extent = Extent;

        //=================================================
        // -ctors
        //=================================================

        explicit Vector_span(const std::uint32_t* ptr):
            ptr(ptr) {}

        Vector_span() = default;
        Vector_span(const Vector_span&) = default;
        Vector_span(Vector_span&&) noexcept = default;
        ~Vector_span() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Vector_span& operator=(const Vector_span&) = default;
        Vector_span& operator=(Vector_span&&) noexcept = default;

        //=================================================
        // Iterator methods
        //=================================================

        iterator begin() const noexcept {
            return ptr;
        }

        iterator end() const noexcept {
            return ptr + base::size();
        }

        reverse_iterator rbegin() const noexcept {
            return reverse_iterator{end()};
        }

        reverse_iterator rend() const noexcept {
            return reverse_iterator{begin()};
        }

        //=================================================
        // Accessors
        //=================================================

        std::size_t size() const {
            return base::size();
        }

        std::size_t size_bytes() const {
            return size() * sizeof(std::uint32_t);
        }

        bool empty() const noexcept {
            return ptr == nullptr;
        }

        //TODO: Add subviews

        const std::uint32_t* data() const {
            return ptr;
        }

        //=================================================
        // Mutators
        //=================================================

    private:

        //=================================================
        // Instance members
        //=================================================

        const std::uint32_t* ptr = nullptr;

    };

    template<std::size_t length, std::size_t Alignment>
    std::uint32_t max_element(Vector_span<const std::uint32_t, length, Alignment> span) {
        //TODO: Implement
    }

    template<std::size_t length, std::size_t Alignment>
    std::uint32_t min_element(Vector_span<const std::uint32_t, length, Alignment> span) {
        //TODO: Implement
    }

    template<std::size_t length, std::size_t Alignment>
    std::uint32_t reduce_add(Vector_span<const std::uint32_t, length, Alignment> span) {
        //TODO: Implement
    }

    template<std::size_t length, std::size_t Alignment>
    std::uint32_t reduce_mul(Vector_span<const std::uint32_t, length, Alignment> span) {
        //TODO: Implement
    }

    /*
    template<std::size_t length, std::size_t Alignment>
    class Vector_span<std::uint32_t, length, Alignment> : Vector_span_base<const std::uint32_t, length, Alignment> {
    public:



    private:



    };
    */

}
