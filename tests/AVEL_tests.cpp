#include <gtest/gtest.h>

#include <random>
#include <algorithm>
#include <cstdint>

#include "AVEL_tests.hpp"

#include "Type_traits_tests.hpp"

#include "Aligned_allocator_tests.hpp"

#include "Denominator_tests.hpp"
#include "Denominator_vector_tests.hpp"

using namespace avel;

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
