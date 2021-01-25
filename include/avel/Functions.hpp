#ifndef AVEL_FUNCTIONS_HPP
#define AVEL_FUNCTIONS_HPP

namespace avel {

    //=====================================================
    // I/O functions
    //=====================================================

    template<class Vector>
    Vector aligned_load(const typename Vector::scalar_type* ptr);

    template<class Vector>
    Vector unaligned_load(const typename Vector::scalar_type* ptr);

    template<class Vector>
    Vector stream_load(const typename Vector::scalar_type* ptr);



    template<class Vector>
    void aligned_store(typename Vector::scalar_type* ptr, Vector);

    template<class Vector>
    void unaligned_store(typename Vector::scalar_type* ptr, Vector);

    template<class Vector>
    void stream_store(typename Vector::scalar_type* ptr, Vector);

}

#include "impl/functions/Functions.hpp"

#endif //AVEL_FUNCTIONS_HPP
