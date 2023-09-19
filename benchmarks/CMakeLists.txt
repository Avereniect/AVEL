cmake_minimum_required(VERSION 3.22)


add_executable(AVEL_benchmarks
    AVEL_benchmarks.cpp

    AVEL_shifts.hpp
    AVEL_division.hpp
    AVEL_bit_ceil.hpp
    AVEL_multiplication.hpp
)

target_link_libraries(AVEL_benchmarks PRIVATE AVEL benchmark benchmark_main)
target_include_directories(AVEL_benchmarks PRIVATE ./)
target_compile_features(AVEL_benchmarks PRIVATE cxx_std_20)
target_compile_options(AVEL_benchmarks PRIVATE "")
target_link_options(AVEL_benchmarks PRIVATE "")


include(CheckIPOSupported)
check_ipo_supported(RESULT supported OUTPUT error)


if(supported)
    set_property(TARGET AVEL_benchmarks PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO / LTO not supported: <${error}>")
endif()
