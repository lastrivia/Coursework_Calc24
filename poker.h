#ifndef CALC_POKER_H
#define CALC_POKER_H

#include <string>

namespace calc {

    int poker_to_int(const std::string &str);

    std::string int_to_poker(int n);

}

#endif
