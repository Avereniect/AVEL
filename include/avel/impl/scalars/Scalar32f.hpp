//
// Created by avereniect on 2/22/22.
//

#ifndef AVEL_SCALAR32f_HPP
#define AVEL_SCALAR32f_HPP

namespace avel {

    [[nodiscard]]
    AVEL_FINL float blend(float a, float b, bool m) {
        return (a * float(!m)) + (b * float(m));
    }

}

#endif //AVEL_IMPL_SCALAR32f_HPP
