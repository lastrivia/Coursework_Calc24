#ifndef CALC_UTILS_H
#define CALC_UTILS_H

#include <string>
#include <vector>

namespace calc {

    int poker_to_int(const std::string &str);

    std::string int_to_poker(int n);

    template<typename T, typename U>
    bool same_elements(const std::vector<T> &a, const std::vector<U> &b) {
        if (a.size() != b.size())
            return false;
        std::vector<T> a_copy = a;
        std::vector<U> b_copy = b;
        std::sort(a_copy.begin(), a_copy.end());
        std::sort(b_copy.begin(), b_copy.end());
        for (int i = 0; i < a_copy.size(); ++i)
            if (a_copy[i] != b_copy[i])
                return false;
        return true;
    }

}

#endif
