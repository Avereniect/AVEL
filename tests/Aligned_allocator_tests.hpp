#ifndef AVEL_ALIGNED_ALLOCATOR_TESTS_HPP
#define AVEL_ALIGNED_ALLOCATOR_TESTS_HPP

#include <avel/Aligned_allocator.hpp>

#include "AVEL_tests.hpp"

namespace avel_tests {

    static constexpr std::size_t alloc_iterations = 32;

    //Run with address sanitizer to check for memory leaks and other such errors

    TEST(Aligned_allocator, Alignment1_allocations_random) {
        using alloc_type = avel::Aligned_allocator<std::uint8_t, 1>;
        alloc_type allocator{};

        for (std::size_t i = 0; i < alloc_iterations; ++i) {
            auto size = random16u();
            std::uint8_t* p = allocator.allocate(size);
            allocator.deallocate(p, size);
        }
    }

    TEST(Aligned_allocator, Alignment2_allocations_random) {
        using alloc_type = avel::Aligned_allocator<std::uint8_t, 2>;
        alloc_type allocator{};

        for (std::size_t i = 0; i < alloc_iterations; ++i) {
            auto size = random16u();
            std::uint8_t* p = allocator.allocate(size);
            allocator.deallocate(p, size);
        }
    }

    TEST(Aligned_allocator, Alignment4_allocations_random) {
        using alloc_type = avel::Aligned_allocator<std::uint8_t, 4>;
        alloc_type allocator{};

        for (std::size_t i = 0; i < alloc_iterations; ++i) {
            auto size = random16u();
            std::uint8_t* p = allocator.allocate(size);
            allocator.deallocate(p, size);
        }
    }

    TEST(Aligned_allocator, Alignment8_allocations_random) {
        using alloc_type = avel::Aligned_allocator<std::uint8_t, 8>;
        alloc_type allocator{};

        for (std::size_t i = 0; i < alloc_iterations; ++i) {
            auto size = random16u();
            std::uint8_t* p = allocator.allocate(size);
            allocator.deallocate(p, size);
        }
    }

    TEST(Aligned_allocator, Alignment16_allocations_random) {
        using alloc_type = avel::Aligned_allocator<std::uint8_t, 16>;
        alloc_type allocator{};

        for (std::size_t i = 0; i < alloc_iterations; ++i) {
            auto size = random16u();
            std::uint8_t* p = allocator.allocate(size);
            allocator.deallocate(p, size);
        }
    }

    TEST(Aligned_allocator, Alignment32_allocations_random) {
        using alloc_type = avel::Aligned_allocator<std::uint8_t, 32>;
        alloc_type allocator{};

        for (std::size_t i = 0; i < alloc_iterations; ++i) {
            auto size = random16u();
            std::uint8_t* p = allocator.allocate(size);
            allocator.deallocate(p, size);
        }
    }

    TEST(Aligned_allocator, Alignment64_allocations_random) {
        using alloc_type = avel::Aligned_allocator<std::uint8_t, 64>;
        alloc_type allocator{};

        for (std::size_t i = 0; i < alloc_iterations; ++i) {
            auto size = random16u();
            std::uint8_t* p = allocator.allocate(size);
            allocator.deallocate(p, size);
        }
    }

    TEST(Aligned_allocator, Alignment128_allocations_random) {
        using alloc_type = avel::Aligned_allocator<std::uint8_t, 128>;
        alloc_type allocator{};

        for (std::size_t i = 0; i < alloc_iterations; ++i) {
            auto size = random16u();
            std::uint8_t* p = allocator.allocate(size);
            allocator.deallocate(p, size);
        }
    }

}

#endif //AVEL_ALIGNED_ALLOCATOR_TESTS_HPP
