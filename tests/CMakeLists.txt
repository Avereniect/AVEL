cmake_minimum_required(VERSION 3.16)


# Use CompilerCache when available
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()



## AVEL test groups list
set(AVEL_TEST_GROUPS
    AVEL_ENABLE_VEC1X8I_TESTS;
    AVEL_ENABLE_VEC1X8U_TESTS;

    AVEL_ENABLE_VEC1X16I_TESTS;
    AVEL_ENABLE_VEC1X16U_TESTS;

    AVEL_ENABLE_VEC1X32I_TESTS;
    AVEL_ENABLE_VEC1X32U_TESTS;

    AVEL_ENABLE_VEC1X64I_TESTS;
    AVEL_ENABLE_VEC1X64U_TESTS;

    AVEL_ENABLE_VEC1X32F_TESTS;
    AVEL_ENABLE_VEC1X64F_TESTS;




    AVEL_ENABLE_VEC16X8U_TESTS;
    AVEL_ENABLE_VEC16X8I_TESTS;

    AVEL_ENABLE_VEC8X16U_TESTS;
    AVEL_ENABLE_VEC8X16I_TESTS;

    AVEL_ENABLE_VEC4X32U_TESTS;
    AVEL_ENABLE_VEC4X32I_TESTS;

    AVEL_ENABLE_VEC2X64U_TESTS;
    AVEL_ENABLE_VEC2X64I_TESTS;

    AVEL_ENABLE_VEC4X32F_TESTS;
    AVEL_ENABLE_VEC2X64F_TESTS;



    AVEL_ENABLE_VEC32X8U_TESTS;
    AVEL_ENABLE_VEC32X8I_TESTS;

    AVEL_ENABLE_VEC16X16U_TESTS;
    AVEL_ENABLE_VEC16X16I_TESTS;

    AVEL_ENABLE_VEC8X32U_TESTS;
    AVEL_ENABLE_VEC8X32I_TESTS;

    AVEL_ENABLE_VEC4X64U_TESTS;
    AVEL_ENABLE_VEC4X64I_TESTS;

    AVEL_ENABLE_VEC8X32F_TESTS;
    AVEL_ENABLE_VEC4X64F_TESTS;



    AVEL_ENABLE_VEC64X8U_TESTS;
    AVEL_ENABLE_VEC64X8I_TESTS;

    AVEL_ENABLE_VEC32X16U_TESTS;
    AVEL_ENABLE_VEC32X16I_TESTS;

    AVEL_ENABLE_VEC16X32U_TESTS;
    AVEL_ENABLE_VEC16X32I_TESTS;

    AVEL_ENABLE_VEC8X64U_TESTS;
    AVEL_ENABLE_VEC8X64I_TESTS;

    AVEL_ENABLE_VEC16X32F_TESTS;
    AVEL_ENABLE_VEC8X64F_TESTS;
)

## Create test group options
foreach (test_group IN LISTS AVEL_TEST_GROUPS)
    if (NOT DEFINED())
        set(${test_group} ON)
    endif()
endforeach()

## Create list of requested test groups
foreach(test_group IN LISTS AVEL_TEST_GROUPS)
    if (${test_group})
        list(APPEND AVEL_REQUESTED_TEST_GROUPS "${test_group}=1 ")
    else()
        list(APPEND AVEL_REQUESTED_TEST_GROUPS "${test_group}=0 ")
    endif()
endforeach()



# AVEL_TESTS
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

    scalars/Scalar32f_tests.hpp
    scalars/Scalar64f_tests.hpp

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

    vectors/Vec4x32f_tests.hpp
    vectors/Vec2x64f_tests.hpp


    vectors/Vec32x8u_tests.hpp
    vectors/Vec32x8i_tests.hpp

    vectors/Vec16x16u_tests.hpp
    vectors/Vec16x16i_tests.hpp

    vectors/Vec8x32u_tests.hpp
    vectors/Vec8x32i_tests.hpp

    vectors/Vec4x64u_tests.hpp
    vectors/Vec4x64i_tests.hpp

    vectors/Vec8x32f_tests.hpp
    vectors/Vec4x64f_tests.hpp



    vectors/Vec64x8u_tests.hpp
    vectors/Vec64x8i_tests.hpp

    vectors/Vec32x16u_tests.hpp
    vectors/Vec32x16i_tests.hpp

    vectors/Vec16x32u_tests.hpp
    vectors/Vec16x32i_tests.hpp

    vectors/Vec8x64u_tests.hpp
    vectors/Vec8x64i_tests.hpp

    vectors/Vec16x32f_tests.hpp
    vectors/Vec8x64f_tests.hpp


    vectors/Vector_tests.cpp

    Denominator_vector_tests.hpp
)

target_compile_definitions(AVEL INTERFACE ${AVEL_REQUESTED_TEST_GROUPS})

if (UNIX)
    set(AVEL_TESTS_LIBRARIES "gtest" "pthread")
else()
    set(AVEL_TESTS_LIBRARIES "gtest")
endif()

target_link_libraries(AVEL_TESTS PUBLIC AVEL ${AVEL_TESTS_LIBRARIES})
target_compile_features(AVEL_TESTS PRIVATE "cxx_std_11")

#TODO: Set compilation flags based on enabled features
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(AVEL_ROUNDING_FLAG "-frounding-math")
else()
    set(AVEL_ROUNDING_FLAG "")
endif()

# Necessary for successful compilation
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(AVEL_MSVC_SPECIFIC_FLAGS "/bigobj"
        $<IF:$<CONFIG:Debug>, "/MTd", "/MT">
    )
else()
    set(AVEL_MSVC_SPECIFIC_FLAGS "")
endif()

target_compile_options(AVEL_TESTS PRIVATE ${AVEL_ROUNDING_FLAG} ${AVEL_MSVC_SPECIFIC_FLAGS})
target_link_options(AVEL_TESTS PRIVATE "")

target_include_directories(AVEL_TESTS PRIVATE ./include/)

## Handle precompiled headers
if(AVEL_USE_PRECOMPILED_HEADERS AND (${CMAKE_VERSION} VERSION_LESS "3.16.0"))
    message(FATAL_ERROR "Precompiled headers require CMake 3.16.0")
endif()

if(AVEL_USE_PRECOMPILED_HEADERS)
    target_precompile_headers(AVEL_TESTS
        PRIVATE
        AVEL_tests.hpp
    )
endif()
