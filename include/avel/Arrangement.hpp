#ifndef AVEL_ARRANGEMENT_HPP
#define AVEL_ARRANGEMENT_HPP

#include <cstdint>

#if __cplusplus >= 202002L
    #include <span>
#endif

namespace avel_impl {

    ///
    ///
    /// \tparam Element_size Size of elements to be transposed
    /// \param ptr
    /// \param elem_count
    /// \param vector_width Width of vectors to process elements with. Should be a power of two.
    template<std::size_t Element_size>
    void transpose(char* ptr, std::size_t elem_count, std::uint32_t vector_width) {
        const std::size_t whole_blocks = elem_count / vector_width;

        const std::size_t whole_end = whole_blocks * vector_width;
        for (std::size_t i = 0; i < whole_end; i += vector_width) {
            char* block_begin = ptr + i * Element_size;


        }
    }

    /*
    template<>
    void transpose<1>(void* ptr, std::size_t elem_count, std::uint32_t vector_width) {
        #if defined(AVEL_AVX512VBMI)


        #elif defined(AVEL_AVX512F)

        #elif defined(AVEL_AVX2)

        #elif defined(AVEL_SSSE3)

        #elif defined(AVEL_SSE2)

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<>
    void transpose<2>(void* ptr, std::size_t elem_count, std::uint32_t vector_width) {
        #if defined(AVEL_AVX512VBMI)

        #elif defined(AVEL_AVX512F)

        #elif defined(AVEL_AVX2)

        #elif defined(AVEL_SSSE3)

        #elif defined(AVEL_SSE2)

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<>
    void transpose<4>(void* ptr, std::size_t elem_count, std::uint32_t vector_width) {
        #if defined(AVEL_AVX512F)

        #elif defined(AVEL_AVX2)

        #elif defined(AVEL_SSE2)

        #endif

        #if defined(AVEL_NEON)

        #endif
    }

    template<>
    void transpose<8>(void* ptr, std::size_t elem_count, std::uint32_t vector_width) {
        #if defined(AVEL_AVX512F)

        #elif defined(AVEL_AVX2)

        #elif defined(AVEL_SSE2)

        #endif

        #if defined(AVEL_NEON)

        #endif
    }
    */

}

namespace avel {

    ///
    /// \tparam T Type of elements to transpose
    /// \param data Pointer to array of elements to transpose
    /// \param size Amount of data which
    /// \param vector_width Should be a power of two.
    template<class T>
    void transpose(T* data, std::size_t size, std::uint32_t vector_width) {
        constexpr bool is_supported_type = std::is_fundamental<T>::value && (std::is_integral<T>::value || std::is_floating_point<T>::value);
        static_assert(is_supported_type, "T must be a primitive integral or floating-point type");

        avel_impl::transpose<sizeof(T)>(
            data,
            size,
            avel::bit_floor(vector_width)
        );
    }


    #if __cplusplus >= 202002L

    template<class T>
    void transpose(std::span<T> elements) {
        transpose(elements.data(), elements.size());
    }

    #endif

}

#endif //AVEL_ARRANGEMENT_HPP
