#ifndef AVEL_IMPL_CONSTANTS_HPP
#define AVEL_IMPL_CONSTANTS_HPP

#include "Capabilities.hpp"
#include "../Misc.hpp"

#include <cstdint>
#include <array>

namespace avel {

    //=====================================================
    // Cache line constants
    //=====================================================

    constexpr std::array<std::size_t, 3> cache_line_sizes {
        AVEL_L1_CACHE_LINE_SIZE,
        AVEL_L2_CACHE_LINE_SIZE,
        AVEL_L3_CACHE_LINE_SIZE
    };

    //=====================================================
    // Floating-point constants
    //
    // Used in implementation of math functions
    // Errors are positive for overestimates and negative
    // for underestimates
    //=====================================================

    // Float-component masks

    const std::uint32_t float_sign_bit_mask_bits = 0x80000000;
    const auto float_sign_bit_mask = avel::bit_cast<float>(float_sign_bit_mask_bits);

    const std::uint32_t float_exponent_mask_bits = 0x7f800000;
    const auto float_exponent_mask = avel::bit_cast<float>(float_exponent_mask_bits);

    const std::uint32_t float_mantissa_mask_bits = 0x007fffff;
    const auto float_mantissa_mask = avel::bit_cast<float>(float_mantissa_mask_bits);

    // Notable floating-point values

    const std::uint32_t float_subnormal_range_begin_bits = 0x00000000;
    const auto float_subnormal_range_begin = avel::bit_cast<float>(float_subnormal_range_begin_bits);

    const std::uint32_t float_subnormal_range_end_bits = 0x800000;
    const auto float_subnormal_range_end = avel::bit_cast<float>(float_subnormal_range_end_bits);

    const std::uint32_t float_normal_range_begin_bits = 0x800000;
    const auto float_normal_range_begin = avel::bit_cast<float>(float_normal_range_begin_bits);

    const std::uint32_t float_normal_range_end_bits = 0x7f800000;
    const auto float_normal_range_end = avel::bit_cast<float>(float_normal_range_end_bits);

    // Trigonometric constants

    const std::uint32_t quarter_pi_bits = 0x3f490fdb;
    const auto quarter_pi = avel::bit_cast<float>(quarter_pi_bits);

    const std::uint32_t quarter_bit_error_bits = 0x32bbbd2e;
    const auto quarter_pi_error = avel::bit_cast<float>(quarter_bit_error_bits);


    const std::uint32_t half_pi_bits = 0x3fc90fdb;
    const auto half_pi = avel::bit_cast<float>(half_pi_bits);

    const std::uint32_t half_pi_error_bits = 0x333bbd2e;
    const auto half_pi_error = avel::bit_cast<float>(half_pi_error_bits);


    const std::uint32_t three_quarter_pi_bits = 0x4016cbe4;
    const auto three_quarter_pi = avel::bit_cast<float>(three_quarter_pi_bits);

    const std::uint32_t three_quarter_pi_error_bits = 0x31ccde2e;
    const auto three_quarter_pi_error = avel::bit_cast<float>(half_pi_error_bits);


    const std::uint32_t pi_bits = 0x40490fdb;
    const auto pi = avel::bit_cast<float>(pi_bits);

    const std::uint32_t pi_error_bits = 0x33bbbd2b;
    const auto pi_error = avel::bit_cast<float>(pi_error_bits);


    const std::uint32_t tau_bits = 0x40c90fdb;
    const auto tau = avel::bit_cast<float>(tau_bits);

    const std::uint32_t tau_error_bits = 0x343bbd2e;
    const auto tau_error = avel::bit_cast<float>(tau_error_bits);


    const std::uint32_t rcp_pi_bits = 0x3ea2f983;
    const auto rcp_pi = avel::bit_cast<float>(rcp_pi_bits);

    const std::uint32_t rcp_pi_error_bits = 0xb25c9c88;
    const auto rcp_pi_error = avel::bit_cast<float>(rcp_pi_error_bits);


    const std::uint32_t pi_plus_bits = 0x40490fdb;
    const auto p_plus = avel::bit_cast<float>(pi_bits);

    const std::uint32_t w_plus_bits = 0x3ebbbd2e;
    const auto w_plus = avel::bit_cast<float>(w_plus_bits);


    const std::uint32_t pi_minus_bits = 0x40490fda;
    const auto p_minus = avel::bit_cast<float>(pi_bits);

    const std::uint32_t w_minus_bits = 0x3f222169;
    const auto w_minus = avel::bit_cast<float>(w_minus_bits);


    const std::uint32_t recip_pi_plus_bits = 0x3ea2f984;
    const float recip_pi_plus = avel::bit_cast<float>(recip_pi_plus_bits);

    const std::uint32_t recip_pi_minus_bits = 0x3ea2f983;
    const float recip_pi_minus = avel::bit_cast<float>(recip_pi_minus_bits);

}

#endif //AVEL_CONSTANTS_HPP
