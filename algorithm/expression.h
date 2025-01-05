#ifndef CALC_EXPRESSION_H
#define CALC_EXPRESSION_H

#include <string>
#include <sstream>
#include <stack>
#include "operation.h"

namespace calc {

    enum expression_node_type {
        operator_node, operand_node, bad_node
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

        expression(const expression<T> &other) {
            node_type = other.node_type;
            switch (node_type) {
                case operand_node:
                    operand = other.operand;
                    break;
                case operator_node:
                    operation = other.operation;
                    sub_left = new expression<T>(*other.sub_left);
                    sub_right = new expression<T>(*other.sub_right);
                    break;
                case bad_node:
                    break;
            }
        }

        expression &operator=(const expression<T> &other) {
            if (&other == this)
                return *this;
            if (node_type == operator_node) {
                delete sub_left;
                delete sub_right;
            }
            node_type = other.node_type;
            switch (node_type) {
                case operand_node:
                    operand = other.operand;
                    break;
                case operator_node:
                    operation = other.operation;
                    sub_left = new expression<T>(*other.sub_left);
                    sub_right = new expression<T>(*other.sub_right);
                    break;
                case bad_node:
                    break;
            }
            return *this;
        }

        template<typename U>
        operator expression<U>() const { // NOLINT
            expression<U> result((U()));
            result.node_type = node_type;
            switch (node_type) {
                case operand_node:
                    return expression<U>(static_cast<U>(operand));
                case operator_node:
                    result.operation = operation;
                    result.sub_left = new expression<U>(*sub_left);
                    result.sub_right = new expression<U>(*sub_right);
                    break;
                case bad_node:
                    break;
            }
            return result;
        }

        bool bad() const { // NOLINT
            return node_type == bad_node;
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

                case bad_node:
                    return T();
            }
            return T();
        }

        std::vector<T> extract_operands() const {
            std::vector<T> operands;
            std::stack<expression<T> const *> recursive_stack;
            recursive_stack.push(this);

            expression<T> const *current;
            while (!recursive_stack.empty()) {
                current = recursive_stack.top();
                recursive_stack.pop();

                switch (current->node_type) {
                    case operator_node:
                        recursive_stack.push(current->sub_left);
                        recursive_stack.push(current->sub_right);
                        break;
                    case operand_node:
                        operands.push_back(current->operand);
                        break;
                    case bad_node:
                        break;
                }
            }
            return operands;
        }

        void print_expression_to(std::ostream &out, bool reversed = false) const {
            switch (node_type) {
                case operator_node:
                    if (sub_left->node_type == operator_node) {
                        if (operation_priority[operation] > operation_priority[sub_left->operation]) {

                            out << '(';
                            sub_left->print_expression_to(out);
                            out << ')';
                        }
                        else if (operation_priority[operation] == operation_priority[sub_left->operation])
                            sub_left->print_expression_to(out, reversed);
                        else
                            sub_left->print_expression_to(out);
                    }
                    else
                        sub_left->print_expression_to(out);

                    if (reversed)
                        out << operator_char[reversed_operation[operation]];
                    else
                        out << operator_char[operation];

                    if (sub_right->node_type == operator_node) {
                        if (operation_priority[operation] > operation_priority[sub_right->operation]) {

                            out << '(';
                            sub_right->print_expression_to(out);
                            out << ')';
                        }
                        else if (operation_priority[operation] == operation_priority[sub_right->operation])
                            sub_right->print_expression_to(out, reversed ^ (!is_commutative[operation]));
                        else
                            sub_right->print_expression_to(out);
                    }
                    else
                        sub_right->print_expression_to(out);

                    break;

                case operand_node:
                    out << operand;
                    break;

                case bad_node:
                    break;
            }
        }

        friend std::ostream &operator<<(std::ostream &out, const expression<T> &exp) {
            exp.print_expression_to(out);
            return out;
        }

        operator std::string() const { // NOLINT
            std::stringstream ss;
            print_expression_to(ss);
            return ss.str();
        }

        expression(std::string str) { // NOLINT
            std::stringstream ss(str);
            std::stack<int> operator_stack;
            std::stack<expression<T> *> operand_stack;

            T current_operand;
            expression<T> *expression_left, *expression_right;

            try {
                while (!ss.eof()) {
                    if (ss.peek() == EOF)
                        break;
                    int op = char_operator((char) ss.peek());
                    switch (op) {
                        case -1: // operand
                            ss >> current_operand;
                            if (ss.fail())
                                throw std::invalid_argument(""); // invalid operand format
                            operand_stack.push(new expression<T>(current_operand));
                            break;
                        case operator_add:
                        case operator_sub:
                        case operator_mul:
                        case operator_div:
                            ss.ignore();
                            while (true) {
                                if (operator_stack.empty() ||
                                    operator_stack.top() == parenthesis_left ||
                                    operation_priority[op] >= operation_priority[operator_stack.top()]) {

                                    operator_stack.push(op);
                                    break;
                                }
                                if (operand_stack.size() < 2)
                                    throw std::invalid_argument(""); // redundant operators
                                expression_right = operand_stack.top();
                                operand_stack.pop();
                                expression_left = operand_stack.top();
                                operand_stack.pop();
                                operator_type top_op = static_cast<operator_type>(operator_stack.top());
                                operator_stack.pop();
                                operand_stack.push(new expression<T>(
                                        expression_left, expression_right, top_op));
                            }
                            break;
                        case parenthesis_left:
                            ss.ignore();
                            operator_stack.push(op);
                            break;
                        case parenthesis_right:
                            ss.ignore();
                            while (true) {
                                if (operator_stack.empty())
                                    throw std::invalid_argument(""); // parentheses not matched
                                if (operator_stack.top() == parenthesis_left) {
                                    operator_stack.pop();
                                    break;
                                }
                                if (operand_stack.size() < 2)
                                    throw std::invalid_argument(""); // redundant operators
                                expression_right = operand_stack.top();
                                operand_stack.pop();
                                expression_left = operand_stack.top();
                                operand_stack.pop();
                                operator_type top_op = static_cast<operator_type>(operator_stack.top());
                                operator_stack.pop();
                                operand_stack.push(new expression<T>(
                                        expression_left, expression_right, top_op));
                            }
                            break;
                        default:
                            break;
                    }
                }
                // expression input finished

                while (!operator_stack.empty()) {
                    if (operand_stack.size() < 2)
                        throw std::invalid_argument(""); // redundant operators
                    expression_right = operand_stack.top();
                    operand_stack.pop();
                    expression_left = operand_stack.top();
                    operand_stack.pop();
                    operator_type top_op = static_cast<operator_type>(operator_stack.top());
                    operator_stack.pop();
                    operand_stack.push(new expression<T>(
                            expression_left, expression_right, top_op));
                }

                if (operand_stack.size() > 1)
                    throw std::invalid_argument(""); // redundant operands

                *this = *operand_stack.top();
                delete operand_stack.top();
            }
            catch (std::exception &e) { // invalid input, clear memory and return empty expression
                while (!operand_stack.empty()) {
                    delete operand_stack.top();
                    operand_stack.pop();
                }
                node_type = bad_node;
            }
        }
    };
}

#endif
