#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "utils.h"

namespace calc {

    int poker_to_int(const std::string &str) {

        if (str == "A" || str == "a")
            return 1;
        if (str == "J" || str == "j")
            return 11;
        if (str == "Q" || str == "q")
            return 12;
        if (str == "K" || str == "k")
            return 13;

        try {
            int num = std::stoi(str);
            if (num >= 1 && num <= 9 && str.length() == 1)
                return num;
            if (num >= 10 && num <= 13  && str.length() == 2)
                return num;
        } catch (const std::invalid_argument &) {
            return -1;
        }

        return -1;
    }

    std::string int_to_poker(int n) {
        static constexpr std::string poker_str[] = {
                "", "1", "2", "3", "4", "5", "6",
                "7", "8", "9", "10", "J", "Q", "K"
        };
        if (n >= 1 && n <= 13)
            return poker_str[n];

        return "";
    }
}
