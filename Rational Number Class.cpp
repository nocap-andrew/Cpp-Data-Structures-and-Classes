#include <iostream>
#include <cstdint>
#include <numeric>
#include <stdexcept>

using namespace std;

class Rational {
public:
    // Constructor and member function
    Rational(int numerator = 0, int denominator = 1) :
            num_{denominator < 0 ? -numerator : numerator},
            den_{denominator < 0 ? -denominator : denominator} {
        if (!denominator) {
            throw overflow_error("Divide by zero.");
        }
        reduce();
    }
    int numerator() const noexcept {
        return num_;
    }
    int denominator() const noexcept {
        return den_;
    }
    int& numerator() {
        return num_;
    }
    int& denominator() {
        return den_;
    }
    // Arithmetic Defining Operators
    Rational& operator+=(const Rational& other) noexcept {
        num_ = numerator() * other.denominator() +
               other.numerator() * denominator();
        den_ *= other.denominator();
        reduce();
        return *this;
    }
    Rational& operator-=(const Rational& other) noexcept {
        num_ = numerator() * other.denominator() -
               other.numerator() * denominator();
        den_ *= other.denominator();
        reduce();
        return *this;
    }
    Rational& operator*=(const Rational& other) noexcept {
        num_ = numerator() * other.numerator();
        den_ *= other.denominator();
        reduce();
        return *this;
    }
    Rational& operator/=(const Rational& other) {
        Rational temp;
        if (other.numerator() < 0) {
            temp.numerator() = (-1) * numerator() * other.denominator();
            temp.denominator() = (-1) * denominator() * other.numerator();
            *this = temp;
            reduce();
            return *this;
        } else if (other.numerator() > 0) {
            temp.numerator() = numerator() * other.denominator();
            temp.denominator() = denominator() * other.numerator();
            *this = temp;
            reduce();
            return *this;
        } else {
            throw overflow_error("Divide by zero.");
        }
        return *this;
    }
    // Arithmetic Operators
    friend Rational operator+(Rational left, const Rational& right) noexcept {
        left += right;
        return left;
    }
    friend Rational operator-(Rational left, const Rational& right) noexcept {
        left -= right;
        return left;
    }
    friend Rational operator*(Rational left, const Rational& right) noexcept {
        left *= right;
        return left;
    }
    friend Rational operator/(Rational left, const Rational& right) noexcept {
        left /= right;
        return left;
    }
    // Префиксные и постфиксные операторы + и -
    Rational& operator++() noexcept {  // ++frac;
        num_ += denominator();
        return *this;
    }
    Rational operator++(int) noexcept {  // frac++;
        Rational tmp(*this);
        operator++();
        return tmp;
    }
    Rational& operator--() noexcept {  // ++frac;
        num_ -= denominator();
        return *this;
    }
    Rational operator--(int) noexcept {  // frac++;
        Rational tmp(*this);
        operator--();
        return tmp;
    }
    // Унарные Плюс и Минус
    Rational operator+() const noexcept {  // +rational;
        return *this;
    }
    Rational operator-() const noexcept {  // -rational;
        return {-numerator(), denominator()};
    }

private:
    void reduce() {
        auto temp = gcd(numerator(), denominator());
        num_ /= temp;
        den_ /= temp;
    }
    int num_{}, den_{};
};
// Операторы Сравнения
bool operator==(const Rational& left, const Rational& right) noexcept {
    return left.numerator() == right.numerator() && left.denominator() == right.denominator();
}
bool operator!=(const Rational& left, const Rational& right) noexcept {
    return !(left == right);
}
