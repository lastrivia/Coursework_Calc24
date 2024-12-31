#include <string>
#include <stdexcept>
#include "poker.h"

namespace calc {

    int poker_to_int(const std::string &str) {

        if (str == "J")
            return 11;
        if (str == "Q")
            return 12;
        if (str == "K")
            return 13;

        try {
            int num = std::stoi(str);
            if (num >= 1 && num <= 10) {
                return num;
            }
        } catch (const std::invalid_argument&) {
            return -1;
        }

        return -1;
    }

    std::string int_to_poker(int n) {
        if (n >= 1 && n <= 10) {
            return std::to_string(n);
        } else if (n == 11) {
            return "J";
        } else if (n == 12) {
            return "Q";
        } else if (n == 13) {
            return "K";
        }

        // 非法整数，返回空字符串
        return "";
    }
}
