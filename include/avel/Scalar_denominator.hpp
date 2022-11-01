//
// Created by avereniect on 6/21/22.
//

#ifndef AVEL_SCALAR_RECIPROCAL_HPP
#define AVEL_SCALAR_RECIPROCAL_HPP

#include "Scalar.hpp"

#include <cstdint>

namespace avel {

    ///
    ///
    /// A class which is used to speed up division by using
    /// Granlund-Montgomery's technique for divison by invariant integers using
    /// multiplication.
    ///
    /// Multiplying an object of type T by a Reciprocal object delivers the same
    /// results as dividing by the value d, the parameter passed to the
    /// constructor of the Reciprocal object. On average, this multiplication
    /// should perform better than executing an integer division instruction.
    ///
    /// The constructor of Reciprocal itself requires the execution of an
    /// integer division so the purpose of this class is to speed up the case
    /// where various integer values have to all be divided by a common
    /// denominator.
    ///
    template<class T>
    class Reciprocal;

}

#include "impl/reciprocals/Scalar_reciprocals.hpp"

#endif //AVEL_SCALAR_RECIPROCAL_HPP
