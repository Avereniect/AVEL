//
// Created by avereniect on 2/22/22.
//

#ifndef AVEL_SCALAR32U_HPP
#define AVEL_SCALAR32U_HPP

namespace avel {

    [[nodiscard]]
    constexpr inline bool is_pow2(const std::uint32_t v) {
        return v && !(v & (v - 1));
    }

}

#endif //AVEL_SCALAR32U_HPP
