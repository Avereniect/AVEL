#ifndef AVEL_ALIGNED_ALLOCATOR_HPP
#define AVEL_ALIGNED_ALLOCATOR_HPP

#include "Avel.hpp"

#include <memory>
#include <cstddef>
#include <cstring>
#include <cstdint>

namespace avel {

    ///
    /// An allocator meant as a drop-in replacement for std::allocator but with
    /// support for specifying a compiler-time alignment requirement on all
    /// allocations made.
    ///
    /// \tparam T Object type for which allocator provides storage
    /// \tparam Alignment Minimum alignment that each allocation made should
    ///     have
    template<class T, std::size_t Alignment = alignof(T)>
    class Aligned_allocator {
        static_assert(
            Alignment >= alignof(T),
            "Allocator alignment cannot be less than the alignment required for T"
        );

        static_assert(
            !(Alignment & (Alignment - 1)),
            "Alignment must be a power of two"
        );

    public:

        //=================================================
        // Type aliases
        //=================================================

        using pointer = T*;
        using const_pointer = const T*;

        using void_pointer = void*;
        using const_void_pointer = const void*;

        using value_type = T;

        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        template<class U>
        class rebind {
            using other = Aligned_allocator<U>;
        };

        //=================================================
        // Static constants
        //=================================================

        static constexpr size_type alignment = Alignment;

        static constexpr std::true_type is_always_equal{};
        static constexpr std::false_type propagate_on_container_copy_assignment{};
        static constexpr std::false_type propagate_on_container_move_assignment{};
        static constexpr std::false_type propagate_on_container_swap{};

        //=================================================
        // -ctors
        //=================================================

        Aligned_allocator() = default;
        Aligned_allocator(const Aligned_allocator&) = default;
        Aligned_allocator(Aligned_allocator&&) noexcept = default;
        ~Aligned_allocator() = default;

        //=================================================
        // Assignment operators
        //=================================================

        Aligned_allocator& operator=(const Aligned_allocator&) = default;
        Aligned_allocator& operator=(Aligned_allocator&&) noexcept = default;

        //=================================================
        // Comparison operators
        //=================================================

        [[nodiscard]]
        bool operator==(const Aligned_allocator& rhs) const noexcept {
            (void)rhs;
            return true;
        }

        [[nodiscard]]
        bool operator!=(const Aligned_allocator& rhs) const noexcept {
            (void)rhs;
            return false;
        }

        //=================================================
        // Allocation/Deallocation
        //=================================================

        pointer allocate(size_type n) {
            #if defined(AVEL_SSE)
            void* allocation = _mm_malloc(sizeof(T) * n, alignment);
            return reinterpret_cast<pointer>(allocation);

            #elif 201703L <= __cplusplus
            std::size_t required_size = n * sizeof(T);
            std::size_t adjusted_size = (required_size / alignment) * alignment + bool(required_size % alignment) * alignment;
            void* allocation = std::aligned_alloc(alignment, adjusted_size);
            return reinterpret_cast<pointer>(allocation);

            #else
            // Idea is to over-allocate by some amount and use std::align
            // to find a suballocation of the desired alignment.
            // This approach will however require keeping track of what
            // the original pointer produced by the underlying
            // allocation mechanism was.
            // This information is stored as a byte offset at the end of the
            // allocation which is applied to the aligned pointer to restore the
            // original pointer

            if (alignment <= alignof(std::max_align_t)) {
                // malloc's alignment is at least equal to
                // alignof(std::max_align_t) so no further work necessary
                void* allocation = malloc(sizeof(T) * n);
                return reinterpret_cast<pointer>(allocation);
            }

            std::size_t elements_size = sizeof(T) * n;

            std::size_t alignment_overhead = alignment;
            std::size_t offset_overhead = sizeof(std::size_t);
            std::size_t allocation_size = elements_size + alignment_overhead + offset_overhead;

            void* unaligned_allocation = malloc(allocation_size);
            void* aligned_allocation = unaligned_allocation;
            std::align(alignment, elements_size, aligned_allocation, allocation_size);
            std::size_t alignment_offset =
                reinterpret_cast<char*>(aligned_allocation) -
                reinterpret_cast<char*>(unaligned_allocation);

            auto* offset_location =
                reinterpret_cast<std::size_t*>(
                    reinterpret_cast<char*>(aligned_allocation) + elements_size
                );

            new(offset_location) std::size_t{alignment_offset};

            return reinterpret_cast<pointer>(aligned_allocation);

            #endif
        }

        pointer allocate(size_type n, const_void_pointer) {
            return allocate(n);
        }

        void deallocate(pointer ptr, size_type n) {
            #if defined(AVEL_SSE)
            return _mm_free(ptr);

            #elif 201703L <= __cplusplus
            free(ptr);

            #else
            if (alignment <= alignof(std::max_align_t)) {
                free(ptr);
                return;
            }

            std::size_t offset;
            std::memcpy(&offset, ptr + n, sizeof(std::size_t));

            char* p = reinterpret_cast<char*>(ptr);
            p -= offset;

            free(p);
            #endif
        }

        //=================================================
        // Element Construction/Destruction
        //=================================================

        template<class...Args>
        void construct(pointer ptr, Args&&...args) {
            new(ptr) T(std::forward<Args>(args)...);
        }

        void destroy(pointer ptr) {
            ptr->~T();
        }

        //=================================================
        // Accessors
        //=================================================

        size_type max_size() const noexcept {
            return std::numeric_limits<size_type>::max();
        }

        //=================================================
        // Misc. Allocator Interface methods
        //=================================================

        Aligned_allocator select_on_container_copy_construction() const noexcept {
            return {};
        }

    };

}

#endif //AVEL_ALIGNED_ALLOCATOR_HPP
