#include "operation.h"

namespace calc {

    int char_operator(char c) {
        for (int i = 0; i < 6; ++i)
            if (c == operator_char[i])
                return i;
        return -1;
    }
}