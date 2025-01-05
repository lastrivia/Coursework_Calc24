#ifndef CALC_RATIONAL_H
#define CALC_RATIONAL_H

#include <ostream>

namespace calc {

    int gcd(int a, int b);

    class rational {
        int numer, denom; // allow denom = 0 as infinity flag

        void reduce() {
            if (denom != 1) {
                int d = gcd(numer, denom);
                if (denom < 0) {
                    numer = -numer;
                    denom = -denom;
                }
                if (d != 0) {
                    numer /= d;
                    denom /= d;
                }
            }
        }

    public:
        rational() : numer(0), denom(1) {}

        rational(int x) : numer(x), denom(1) {} // NOLINT

        rational(int numer, int denom) : numer(numer), denom(denom) {
            reduce();
        }

        operator int() const { return numer / denom; } // NOLINT

        explicit operator double() const { return (double) numer / (double) denom; }

        bool infinity() const { // NOLINT
            return denom == 0;
        }

        rational &operator+=(const rational &other) {
            numer = numer * other.denom + other.numer * denom;
            denom *= other.denom;
            reduce();
            return *this;
        }

        rational &operator-=(const rational &other) {
            numer = numer * other.denom - other.numer * denom;
            denom *= other.denom;
            reduce();
            return *this;
        }

        rational &operator*=(const rational &other) {
            numer *= other.numer;
            denom *= other.denom;
            reduce();
            return *this;
        }

        rational &operator/=(const rational &other) {
            numer *= other.denom;
            denom *= other.numer;
            reduce();
            return *this;
        }

        rational operator+(const rational &other) const {
            rational result = *this;
            result += other;
            return result;
        }

        rational operator-(const rational &other) const {
            rational result = *this;
            result -= other;
            return result;
        }

        rational operator*(const rational &other) const {
            rational result = *this;
            result *= other;
            return result;
        }

        rational operator/(const rational &other) const {
            rational result = *this;
            result /= other;
            return result;
        }

        rational operator-() const {
            rational result = *this;
            result.numer = -numer;
            return result;
        }

        bool operator==(const rational &other) const {
            return numer == other.numer && denom == other.denom;
        }

        bool operator<(const rational &other) const {
            return (long long) (numer) * other.denom < (long long) (denom) * other.numer;
        }

        bool operator<=(const rational &other) const {
            return (long long) (numer) * other.denom <= (long long) (denom) * other.numer;
        }

        bool operator>(const rational &other) const {
            return (long long) (numer) * other.denom > (long long) (denom) * other.numer;
        }

        bool operator>=(const rational &other) const {
            return (long long) (numer) * other.denom >= (long long) (denom) * other.numer;
        }

        friend std::ostream &operator<<(std::ostream &out, const rational &r) {
            switch (r.denom) {
                case 0:
                    out << "∞";
                    break;
                case 1:
                    out << r.numer;
                    break;
                default:
                    out << r.numer << '/' << r.denom;
                    break;
            }
            return out;
        }
    };

}

#endif
