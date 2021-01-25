#ifndef AVEL_MATH_HPP
#define AVEL_MATH_HPP

namespace avel {

    //=====================================================
    // Arithmetic functions
    //=====================================================

    template<class Vector>
    Vector fma(Vector m, Vector x, Vector b);

    template<class Vector>
    Vector fms(Vector m, Vector x, Vector b);

    template<class Vector>
    Vector abs(Vector);

    //=====================================================
    // Rounding functions
    //=====================================================

    template<class Vector>
    Vector floor(Vector);

    template<class Vector>
    Vector ceil(Vector);

    template<class Vector>
    Vector round(Vector);

    //=====================================================
    // Exponentiation functions
    //=====================================================

    template<class Vector>
    Vector sqrt(Vector);

}

#endif //AVEL_MATH_HPP
