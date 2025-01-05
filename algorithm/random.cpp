#include <random>
#include <vector>

#include "random.h"
#include "solver.h"

namespace calc {

    std::vector<rational> randomized_integers(int size, int l_limit, int u_limit) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(l_limit, u_limit);

        std::vector<rational> v(size);
        for (auto &i: v)
            i = dis(gen);
        return v;
    }

    std::vector<rational> randomized_integers(int size, int l_limit, int u_limit, rational target_answer) {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(l_limit, u_limit);

        std::vector<rational> v(size);
        solver solver_instance(size, target_answer);

        while (true) {
            for (auto &i: v)
                i = dis(gen);
            solver_instance.set_operands(v);
            if(solver_instance.solve())
                return v;
        }
    }
}
