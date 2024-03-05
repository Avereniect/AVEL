cmake_minimum_required(VERSION 3.16)

add_library(simde INTERFACE)

target_include_directories(simde INTERFACE simde/)

add_library(std_simd INTERFACE)

target_include_directories(std_simd INTERFACE ./std-simd/)

add_library(vectorclass INTERFACE)

target_include_directories(vectorclass INTERFACE ./vectorclass/)

add_subdirectory(eve)
add_subdirectory(highway)

set(SLEEF_BUILD_TESTS OFF)
add_subdirectory(sleef)
