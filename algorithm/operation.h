#ifndef CALC_OPERATION_H
#define CALC_OPERATION_H

#include <string>
#include <array>

namespace calc {

    enum operator_type {
        operator_add, operator_sub, operator_mul, operator_div
    };

    static constexpr std::array<operator_type, 4> operator_types = {
            operator_add, operator_sub, operator_mul, operator_div
    };

    static constexpr char operator_char[] = {
            '+', '-', '*', '/'
    };

    static constexpr std::string operator_wchar[] = {
            "＋", "－", "×", "÷"
    };

    static constexpr bool is_commutative[] = {
            true, false, true, false
    };

    static constexpr int operation_priority[] = {
            0, 0, 1, 1
    };

    template<typename T>
    inline T operation(const T &a, const T &b, operator_type op) {
        switch (op) {
            case operator_add:
                return a + b;
            case operator_sub:
                return a - b;
            case operator_mul:
                return a * b;
            case operator_div:
                return a / b;
        }
        return T();
    }
}

#endif
