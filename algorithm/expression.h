#ifndef CALC_EXPRESSION_H
#define CALC_EXPRESSION_H

#include <string>
#include <sstream>
#include "operation.h"

namespace calc {

    enum expression_node_type {
        operator_node, operand_node
    };

    template<typename T>
    class expression {
    public:
        expression_node_type node_type;

        operator_type operation;
        expression<T> *sub_left, *sub_right;
        // operator_node

        T operand;
        // operand_node

        expression(const T &operand) : // NOLINT
                node_type(operand_node), operand(operand) {}

        expression(expression<T> *sub_left, expression<T> *sub_right, operator_type operation) : // suffix
                node_type(operator_node), sub_left(sub_left), sub_right(sub_right), operation(operation) {}

        expression(expression<T> *sub_left, operator_type operation, expression<T> *sub_right) : // infix
                node_type(operator_node), sub_left(sub_left), sub_right(sub_right), operation(operation) {}

        ~expression() {
            if (node_type == operator_node) {
                delete sub_left;
                delete sub_right;
            }
        }

        T value() const;

        friend std::ostream &operator<<(std::ostream &out, const expression<T> &exp) {
            switch (exp.node_type) {
                case operator_node:
                    if (exp.sub_left->node_type == operator_node &&
                        operation_priority[exp.operation] > operation_priority[exp.sub_left->operation])
                        out << '(' << *exp.sub_left << ')';
                    else
                        out << *exp.sub_left;

                    out << operator_char[exp.operation];

                    if (exp.sub_right->node_type == operator_node &&
                        operation_priority[exp.operation] >= operation_priority[exp.sub_right->operation])
                        out << '(' << *exp.sub_right << ')';
                    else
                        out << *exp.sub_right;
                    break;

                case operand_node:
                    out << exp.operand;
                    break;
            }
            return out;
        }

        operator std::string() const { // NOLINT
            std::stringstream ss;
            ss << *this;
            return ss.str();
        }

        expression(std::string()) { // NOLINT
            // TODO convert from infix
        }
    };
}

#endif
