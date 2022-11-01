//
// Created by avereniect on 7/27/22.
//

#ifndef AVEL_VECTOR_RECIPROCALS_HPP
#define AVEL_VECTOR_RECIPROCALS_HPP

#if defined(AVEL_SSE2) || defined(AVEL_ARM)
    #include "Reciprocal4x32u.hpp"
    #include "Reciprocal4x32i.hpp"

    #include "Reciprocal8x16u.hpp"

    #include "Reciprocal16x8u.hpp"
#endif

#endif //AVEL_VECTOR_RECIPROCALS_HPP
