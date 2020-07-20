#include <cmath>

using namespace std;

class Complex {
public:
    Complex(double re = 0, double im = 0) : re_(re), im_(im) {
    }
    double Re() const {
        return re_;
    }
    double Im() const {
        return im_;
    }
    //Присваивание
    Complex& operator+=(const Complex& rhs) {
        re_ = re_ + rhs.Re();
        im_ = im_ + rhs.Im();
        return *this;
    }
    Complex& operator-=(const Complex& rhs) {
        re_ = re_ - rhs.Re();
        im_ = im_ - rhs.Im();
        return *this;
    }
    //Арифметические
    friend Complex operator+(Complex lhs, const Complex& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Complex operator-(Complex lhs, const Complex& rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend Complex operator*(Complex lhs, const Complex& rhs) {
        auto re_copy = lhs.re_;
        lhs.re_ = rhs.Re() * lhs.Re() - (rhs.Im() * lhs.Im());
        lhs.im_ = rhs.Re() * lhs.Im() + (re_copy * rhs.Im());
        return lhs;
    }
    friend Complex operator/(Complex lhs, const Complex& rhs) {
        double re_copy = lhs.re_;
        lhs.re_ = (rhs.Re() * lhs.Re() + (rhs.Im() * lhs.Im())) /
                  (rhs.re_ * rhs.re_ + rhs.im_ * rhs.im_);
        lhs.im_ = (rhs.Re() * lhs.Im() - (re_copy * rhs.Im())) /
                  (rhs.re_ * rhs.re_ + rhs.im_ * rhs.im_);
        return lhs;
    }

    //Унарные
    Complex operator-() const {
        return {-re_, -im_};
    }
    Complex operator+() const {
        return {re_, im_};
    }
    //Сравнение
    friend bool operator==(const Complex& lhs, const Complex& rhs) {
        bool first = (lhs.Re() == rhs.Re());
        bool second = (lhs.Im() == rhs.Im());
        return first && second;
    }
    friend bool operator!=(const Complex& lhs, const Complex& rhs) {
        bool first = (lhs.Re() != rhs.Re());
        bool second = (lhs.Im() != rhs.Im());
        return first || second;
    }

private:
    double re_, im_;
};
double abs(const Complex& rhs) {
    return sqrt((rhs.Im() * rhs.Im()) + (rhs.Re() * rhs.Re()));
}
