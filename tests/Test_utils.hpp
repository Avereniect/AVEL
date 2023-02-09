#ifndef AVEL_TEST_UTILS_HPP
#define AVEL_TEST_UTILS_HPP

#include "avel/impl/Traits.hpp"
#include "avel/Vector.hpp"

namespace avel {

    template<class V>
    [[nodiscard]]
    bool compare_stored_data(
        const typename V::scalar* p,
        V v,
        Vector<typename to_index_type<typename V::scalar>::type, V::width> indices,
        std::uint32_t j
    ) {
        auto arr = to_array(v);
        auto ind = to_array(indices);

        for (std::uint32_t i = 0; i < V::width; ++i) {
            if (i < j) {
                if (p[ind[i]] != arr[i]) {
                    return false;
                }
            }
        }

        return true;
    }

    template<class V>
    [[nodiscard]]
    bool compare_stored_data(const typename V::scalar* p, V v, std::uint32_t j) {
        auto arr = to_array(v);
        for (std::uint32_t i = 0; i < V::width; ++i) {
            if (i < j) {
                if (p[i] != arr[i]) {
                    return false;
                }
            } else {
                if (p[i] != 0x00) {
                    return false;
                }
            }
        }

        return true;
    }

    template<class T, class U>
    [[nodiscard]]
    T compare_common_bytes(const T& t, const U& u) {
        static_assert(std::is_trivial<T>::value, "Target type must be trivial");
        static_assert(std::is_trivial<U>::value, "Source type must be trivial");

        auto* p0 = reinterpret_cast<const unsigned char*>(&t);
        auto* p1 = reinterpret_cast<const unsigned char*>(&u);

        bool are_equal = true;
        for (std::size_t i = 0; i < std::min(sizeof(T), sizeof(U)); ++i) {
            are_equal &= (p0[i] == p1[i]);
        }

        return are_equal;
    }

}

#endif //AVEL_TEST_UTILS_HPP
