cmake_minimum_required(VERSION 3.16)
project(AVEL)

set(CMAKE_CXX_STANDARD 11)

# AVEL
add_library(AVEL INTERFACE)
target_include_directories(AVEL INTERFACE include)


# AVEL tests
#add_subdirectory(vendor/googletest)
#add_executable(AVEL_TESTS tests/AVEL_tests.cpp)
#target_link_libraries(AVEL_TESTS PUBLIC gtest gtest_main pthread)

#target_include_directories(AVEL_TESTS PRIVATE
#    ./include/
#)
#target_compile_options(AVEL_TESTS PRIVATE "-march=native")


# AVEL benchmarks
#add_subdirectory(vendor/Celero)
#add_executable(AVEL_BENCHMARKS benchmarks/AVEL_benchmarks.cpp)
#target_link_libraries(AVEL_BENCHMARKS celero)
#target_compile_options(AVEL_BENCHMARKS PRIVATE "-msse2" "-msse" "-mavx")

#target_include_directories(AVEL_BENCHMARKS PRIVATE
#    ./include/
#    ./vendor/Celero/include
#)
