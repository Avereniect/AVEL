#ifndef AVEL_BASELINE_HPP
#define AVEL_BASELINE_HPP

#include <boost/multiprecision/cpp_bin_float.hpp>

namespace baseline_generator {

    using hp_float = boost::multiprecision::cpp_bin_float_50;

    class Baseline {
    public:

        Baseline(std::uint32_t begin, std::uint32_t end);

    private:

        std::uint32_t x;

    };



}

#endif //AVEL_BASELINE_HPP
