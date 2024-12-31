#include "expression.h"
#include "rational.h"

namespace calc {

    template<typename T>
    T expression<T>::value() const {
        switch (node_type) {

            case operator_node:
                switch (operation) {
                    case operator_add:
                        return sub_left->value() + sub_right->value();
                    case operator_sub:
                        return sub_left->value() - sub_right->value();
                    case operator_mul:
                        return sub_left->value() * sub_right->value();
                    case operator_div:
                        return sub_left->value() / sub_right->value();
                }

            case operand_node:
                return operand;
        }
        return T();
    }
}