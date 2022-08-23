//
// Created by avereniect on 6/11/22.
//

#ifndef AVEL_SCALARS_HPP
#define AVEL_SCALARS_HPP

namespace avel {

    template<class T>
    T broadcast_bits(bool m);

}

#include "../Capabilities.hpp"
#include "../Constants.hpp"

#include "Scalar8u.hpp"
#include "Scalar8i.hpp"

#include "Scalar16u.hpp"
#include "Scalar16i.hpp"

#include "Scalar32u.hpp"
#include "Scalar32i.hpp"
#include "Scalar32f.hpp"

#include "Scalar64u.hpp"
#include "Scalar64i.hpp"
#include "Scalar64f.hpp"

#endif //AVEL_SCALARS_HPP
