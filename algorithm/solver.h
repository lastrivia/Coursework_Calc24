#ifndef CALC_SOLVER_H
#define CALC_SOLVER_H

#include <vector>
#include "rational.h"
#include "operation.h"

namespace calc {

    class solver {

        std::vector<std::vector<rational> > operands;
        int scale;

        struct branch_indicator {
            int index_l, index_r;
            operator_type operation;
        };
        std::vector<branch_indicator> branch_sequence; // when require expression

        bool require_expression;
        bool solved_flag; // set when found answer
        rational target_answer;

        void recursive_solve(int layer);

    public:

        solver(int scale, rational target_answer) :
                require_expression(false),
                solved_flag(false),
                scale(scale),
                target_answer(target_answer) {

            operands.resize(scale);
            for (int i = 0; i < scale; ++i)
                operands[i].resize(scale - i);
            branch_sequence.resize(scale - 1);
        }

        bool set_operands(const std::vector<rational> &new_operands) {
            if (new_operands.size() != scale)
                return false;
            operands[0] = new_operands;
            return true;
        }

        bool solve();

        bool solve(std::string &answer_container);
    };
}

#endif

