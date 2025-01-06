#ifndef CALC_UTILS_H
#define CALC_UTILS_H

#include <string>
#include <sstream>
#include <vector>

namespace calc {

    int poker_to_int(const std::string &str);

    std::string int_to_poker(int n);

    class poker_int {
        int value;

    public:
        poker_int() = default;

        poker_int(int x) : value(x) {} // NOLINT

        operator int() const { return value; } // NOLINT

        friend std::istream &operator>>(std::istream &in, poker_int &x) {
            char peek = (char) in.peek();
            bool negative_flag = false;
            if (peek == '-') {
                negative_flag = true;
                in.ignore();
                peek = (char) in.peek();
            }
            if (peek >= '0' && peek <= '9')
                in >> x.value;
            else {
                switch (peek) {
                    case 'A':
                    case 'a':
                        x.value = 1;
                        in.ignore();
                        break;
                    case 'J':
                    case 'j':
                        x.value = 11;
                        in.ignore();
                        break;
                    case 'Q':
                    case 'q':
                        x.value = 12;
                        in.ignore();
                        break;
                    case 'K':
                    case 'k':
                        x.value = 13;
                        in.ignore();
                        break;
                    case EOF:
                        in.setstate(std::ios_base::eofbit);
                        break;
                    default:
                        in.setstate(std::ios_base::failbit);
                        break;
                }
            }
            if (negative_flag)
                x.value = -x;
            return in;
        }
    };

    template<typename T, typename U>
    bool same_elements(const std::vector<T> &a, const std::vector<U> &b) {
        if (a.size() != b.size())
            return false;
        std::vector<T> a_copy = a;
        std::vector<U> b_copy = b;
        std::sort(a_copy.begin(), a_copy.end());
        std::sort(b_copy.begin(), b_copy.end());
        for (int i = 0; i < a_copy.size(); ++i)
            if (a_copy[i] != b_copy[i])
                return false;
        return true;
    }

}

#endif
