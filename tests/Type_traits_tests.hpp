#ifndef AVEL_TYPE_TRAITS_TESTS_HPP
#define AVEL_TYPE_TRAITS_TESTS_HPP

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

}

#endif //AVEL_TYPE_TRAITS_HPP
