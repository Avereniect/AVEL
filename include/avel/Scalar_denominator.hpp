#ifndef AVEL_SCALAR_DENOMINATOR_HPP
#define AVEL_SCALAR_DENOMINATOR_HPP

#include "Scalar.hpp"

#include <cstdint>

namespace avel {

    ///
    /// A class which is used to speed up division.
    ///
    /// Multiplying an object of type T by a Denominator object delivers the same
    /// results as dividing by the value d, the parameter passed to the
    /// constructor of the Denominator object. On average, this multiplication
    /// should perform better than executing an integer division instruction.
    ///
    /// Note, that the constructor of Denominator itself requires the execution
    /// of an integer division. Hence, using this class will likely not lead to
    /// improved performance unless using it to divide multiple numerators by
    /// the same denominator.
    ///
    template<class T>
    class Denominator;

}

#include "impl/denominators/Denominators.hpp"

#endif //AVEL_SCALAR_DENOMINATOR_HPP
