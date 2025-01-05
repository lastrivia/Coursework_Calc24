#ifndef CALC_RANDOM_H
#define CALC_RANDOM_H

#include <vector>

#include "rational.h"

namespace calc {

    std::vector<rational> randomized_integers (int size, int l_limit, int u_limit);

    std::vector<rational> randomized_integers (int size, int l_limit, int u_limit, rational target_answer);
}

#endif
