#include <gtest/gtest.h>

#include <random>
#include <algorithm>
#include <cstdint>

#include "AVEL_tests.hpp"

#include "Type_traits_tests.hpp"

#ifndef AVEL_ENABLE_ALLOCATOR_TESTS
    #define AVEL_ENABLE_ALLOCATOR_TESTS true
#endif

#if AVEL_ENABLE_ALLOCATOR_TESTS
    #include "Aligned_allocator_tests.hpp"
#endif



#ifndef AVEL_ENABLE_DENOMINATOR_TESTS
    #define AVEL_ENABLE_DENOMINATOR_TESTS true
#endif

#if AVEL_ENABLE_DENOMINATOR_TESTS
    #include "Denominator_tests.hpp"
    #include "Denominator_vector_tests.hpp"
#endif

using namespace avel;

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
