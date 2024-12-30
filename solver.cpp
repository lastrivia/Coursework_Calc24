#include "solver.h"
#include "expression.h"

namespace calc {

    void solver::recursive_solve(int layer) {
        int n = (int) operands[layer].size();

        if (n == 1 && operands[layer][0] == target_answer) {
            solved_flag = true;
            return;
        }

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                for (auto op: operator_types) {

                    for (int k = 0, m = 0; k < n; ++k)
                        if (k != i && k != j)
                            operands[layer + 1][m++] = operands[layer][k];

                    operands[layer + 1][n - 2] = operation(operands[layer][i], operands[layer][j], op);
                    if (!operands[layer + 1][n - 2].infinity()) // div by 0
                        recursive_solve(layer + 1);
                    if (solved_flag) {
                        if (require_expression)
                            branch_sequence[layer] = branch_indicator{i, j, op};
                        return;
                    }

                    if (!is_commutative[op]) { // '-' or '/'
                        operands[layer + 1][n - 2] = operation(operands[layer][j], operands[layer][i], op);
                        if (!operands[layer + 1][n - 2].infinity()) // div by 0
                            recursive_solve(layer + 1);
                        if (solved_flag) {
                            if (require_expression)
                                branch_sequence[layer] = branch_indicator{j, i, op};
                            return;
                        }
                    }

                }
            }
        }
    }

    bool solver::solve() {
        require_expression = false;
        solved_flag = false;
        recursive_solve(0);
        return solved_flag;
    }

    bool solver::solve(std::string &answer_container) {
        require_expression = true;
        solved_flag = false;
        recursive_solve(0);

        if (solved_flag) {
            std::vector<expression<rational> *> e(scale);
            for (int i = 0; i < scale; ++i)
                e[i] = new expression<rational>(operands[0][i]);

            int l, r;
            for (int i = 0; i < scale - 1; ++i) {
                l = branch_sequence[i].index_l;
                r = branch_sequence[i].index_r;
                auto e_l = e[l], e_r = e[r];
                for(int j = 0, m = 0; j < scale - i; ++j)
                    if(j != l && j != r)
                        e[m++] = e[j];
                e[scale - i - 2] = new expression<rational>(e_l, e_r, branch_sequence[i].operation);
            }

            answer_container = (std::string) *e[0];
            delete e[0]; // other newed pointers are inserted into e[0] and will be deleted recursively
        }
        else
            answer_container = "";


        return solved_flag;
    }
}


