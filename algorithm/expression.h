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

        T value() const {
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

        void print(std::ostream &out, bool reversed = false) const {
            switch (node_type) {
                case operator_node:
                    if (sub_left->node_type == operator_node) {
                        if (operation_priority[operation] > operation_priority[sub_left->operation]) {

                            out << '(';
                            sub_left->print(out);
                            out << ')';
                        }
                        else if (operation_priority[operation] == operation_priority[sub_left->operation])
                            sub_left->print(out, reversed);
                        else
                            sub_left->print(out);
                    }
                    else
                        sub_left->print(out);

                    if (reversed)
                        out << operator_char[reversed_operation[operation]];
                    else
                        out << operator_char[operation];

                    if (sub_right->node_type == operator_node) {
                        if (operation_priority[operation] > operation_priority[sub_right->operation]) {

                            out << '(';
                            sub_right->print(out);
                            out << ')';
                        }
                        else if (operation_priority[operation] == operation_priority[sub_right->operation])
                            sub_right->print(out, reversed ^ (!is_commutative[operation]));
                        else
                            sub_right->print(out);
                    }
                    else
                        sub_right->print(out);

                    break;

                case operand_node:
                    out << operand;
                    break;
            }
        }

        friend std::ostream &operator<<(std::ostream &out, const expression<T> &exp) {
            exp.print(out);
            return out;
        }

        operator std::string() const { // NOLINT
            std::stringstream ss;
            print(ss);
            return ss.str();
        }

        expression(std::string()) { // NOLINT
            // TODO convert from infix
        }
    };
}

#endif
