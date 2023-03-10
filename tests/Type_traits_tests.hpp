#ifndef AVEL_TYPE_TRAITS_TESTS_HPP
#define AVEL_TYPE_TRAITS_TESTS_HPP

#include <avel/Vector.hpp>
#include <type_traits>

namespace avel {

    static_assert(!avel::is_vector<int>::value, "");
    static_assert(!avel::is_vector<float>::value, "");

    static_assert(!avel::is_vector<int*>::value, "");
    static_assert(!avel::is_vector<float*>::value, "");

    static_assert(!avel::is_vector<const int*>::value, "");
    static_assert(!avel::is_vector<const float*>::value, "");

    static_assert(!avel::is_vector<int&>::value, "");
    static_assert(!avel::is_vector<float&>::value, "");

    static_assert(avel::is_vector<avel::Vector<std::uint32_t, 4>>::value, "");
    static_assert(avel::is_vector<avel::Vector<float, 4>>::value, "");

    #if defined(AVEL_SSE2) || defined(AVEL_NEON)

    static_assert(std::is_trivial<mask16x8u>::value, "");
    static_assert(std::is_trivial<vec16x8u>::value, "");

    static_assert(std::is_trivial<mask8x16u>::value, "");
    static_assert(std::is_trivial<vec8x16u>::value, "");

    static_assert(std::is_trivial<mask4x32u>::value, "");
    static_assert(std::is_trivial<vec4x32u>::value, "");

    static_assert(std::is_trivial<mask2x64u>::value, "");
    static_assert(std::is_trivial<vec2x64u>::value, "");

    //static_assert(std::is_trivial<mask4x32f>::value, "");
    //static_assert(std::is_trivial<vec4x32f>::value, "");

    //static_assert(std::is_trivial<mask2x64f>::value, "");
    //static_assert(std::is_trivial<vec2x64f>::value, "");

    #endif



    /*
    #if defined(AVEL_AVX)

    static_assert(std::is_trivial<mask8x32f>::value, "");
    static_assert(std::is_trivial<vec8x32f>::value, "");

    static_assert(std::is_trivial<mask4x64f>::value, "");
    static_assert(std::is_trivial<vec4x64f>::value, "");

    #endif



    #if defined(AVEL_AVX2)

    static_assert(std::is_trivial<mask32x8u>::value, "");
    static_assert(std::is_trivial<vec32x8u>::value, "");

    static_assert(std::is_trivial<mask16x16u>::value, "");
    static_assert(std::is_trivial<vec16x16u>::value, "");

    static_assert(std::is_trivial<mask8x32u>::value, "");
    static_assert(std::is_trivial<vec8x32u>::value, "");

    static_assert(std::is_trivial<mask4x64u>::value, "");
    static_assert(std::is_trivial<vec4x64u>::value, "");

    #endif



    #if defined(AVEL_AVX512F)

    static_assert(std::is_trivial<mask16x32u>::value, "");
    static_assert(std::is_trivial<vec16x32u>::value, "");

    //static_assert(std::is_trivial<mask8x64u>::value, "");
    //static_assert(std::is_trivial<vec8x64u>::value, "");

    static_assert(std::is_trivial<mask16x32f>::value, "");
    static_assert(std::is_trivial<vec16x32f>::value, "");

    //static_assert(std::is_trivial<mask8x64f>::value, "");
    //static_assert(std::is_trivial<vec8x64f>::value, "");

    #endif



    #if defined(AVEL_AVX512BW)

    static_assert(std::is_trivial<mask64x8u>::value, "");
    static_assert(std::is_trivial<vec64x8u>::value, "");

    static_assert(std::is_trivial<mask32x16u>::value, "");
    static_assert(std::is_trivial<vec32x16u>::value, "");

    #endif
    */

}

#endif //AVEL_TYPE_TRAITS_HPP
