cmake_minimum_required(VERSION 3.13)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Clear variables from cache for reuse of build-directory with different
# feature sets
foreach (extension IN LISTS AVEL_OPTIONS)
    unset(${extension} CACHE)
endforeach()

add_executable(AVEL_TESTS
        Type_traits_tests.hpp

        AVEL_tests.hpp
        AVEL_tests.cpp



        scalars/Scalar8u_tests.hpp
        scalars/Scalar8i_tests.hpp

        scalars/Scalar16u_tests.hpp
        scalars/Scalar16i_tests.hpp

        scalars/Scalar32u_tests.hpp
        scalars/Scalar32i_tests.hpp

        scalars/Scalar64u_tests.hpp
        scalars/Scalar64i_tests.hpp

        scalars/Scalar_tests.cpp

        Denominator_tests.hpp


        vectors/Vec16x8u_tests.hpp
        vectors/Vec16x8i_tests.hpp

        vectors/Vec8x16u_tests.hpp
        vectors/Vec8x16i_tests.hpp

        vectors/Vec4x32u_tests.hpp
        vectors/Vec4x32i_tests.hpp

        vectors/Vec2x64u_tests.hpp
        vectors/Vec2x64i_tests.hpp

        vectors/Vector_tests.cpp

        Denominator_vector_tests.hpp
)

target_link_libraries(AVEL_TESTS PUBLIC AVEL gtest pthread)
target_compile_features(AVEL_TESTS PRIVATE "cxx_std_11")

#TODO: Set compilation flags based on enabled features
target_compile_options(AVEL_TESTS PRIVATE "")
target_link_options(AVEL_TESTS PRIVATE "")

target_precompile_headers(AVEL_TESTS
    PRIVATE
    AVEL_tests.hpp
)

target_include_directories(AVEL_TESTS PRIVATE ./include/)
