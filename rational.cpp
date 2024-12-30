#include "rational.h"

namespace calc {

    int gcd(int a, int b) {
        if (a < 0)
            a = -a;
        if (b < 0)
            b = -b;
        int t;
        while (b) {
            t = b;
            b = a % b;
            a = t;
        }
        return a;
    }
}