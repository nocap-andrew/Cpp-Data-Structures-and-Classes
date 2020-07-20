#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>


template <typename T>
class Polynomial {
private:
    std::vector<T> _polynomial;
    void _normalize(std::vector<T> &coefs) {
        for (auto iter = coefs.rbegin(); iter != coefs.rend(); ++iter) {
            if (*iter == T(0)) {
                coefs.pop_back();
            } else {
                break;
            }
        }
    }

public:
    // Vector constructor
    Polynomial<T>(const std::vector<T> &a):_polynomial(a) {
        _normalize(_polynomial);
    }
    // Pair of Iterators constructor
    template <typename Iter>
    Polynomial<T>(Iter first, Iter last) :_polynomial(first, last) {
        _normalize(_polynomial);
    }
    // Coefficient Constructor
    Polynomial<T>(const T &num = T()) {
        _polynomial.push_back(num);
        _normalize(_polynomial);
    }
    T operator[](size_t i) const {
        if (i >= _polynomial.size()) {
            return T(0);
        } else {
            return _polynomial[i];
        }
    }
    int Degree() const {
        if (_polynomial.empty()) {
            return -1;
        } else {
            return static_cast<int>(_polynomial.size()) - 1;
        }
    }
    // Comparison operators
    bool operator==(const Polynomial<T> &other) const {
        if (_polynomial.size() != other._polynomial.size()) {
            return false;
        }
        for (size_t i = 0; i != _polynomial.size(); ++i) {
            if (_polynomial[i] != other[i]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Polynomial<T> &other) const {
        return !(*this == other);
    }
    bool operator==(const T &num) {
        return *this == Polynomial<T>(num);
    }
    bool operator!=(const T &num) {
        return *this != Polynomial<T>(num);
    }
    // Binary operators calc and def
    Polynomial<T>& operator+=(const Polynomial<T> &other) {
        _polynomial.resize(std::max(other._polynomial.size(), _polynomial.size()), T(0));
        for (size_t i = 0; i != std::min(_polynomial.size(), other._polynomial.size()); ++i) {
            _polynomial[i] += other._polynomial[i];
        }
        _normalize(_polynomial);
        return *this;
    }
    Polynomial<T>& operator-=(const Polynomial<T> &other) {
        _polynomial.resize(std::max(other._polynomial.size(), _polynomial.size()), T(0));
        for (size_t i = 0; i != std::min(_polynomial.size(), other._polynomial.size()); ++i) {
            _polynomial[i] -= other._polynomial[i];
        }
        _normalize(_polynomial);
        return *this;
    }
    Polynomial<T>& operator+=(const T &num) {
        *this += Polynomial<T>(num);
        _normalize(_polynomial);
        return *this;
    }
    Polynomial<T>& operator-=(const T &num) {
        *this -= Polynomial<T>(num);
        _normalize(_polynomial);
        return *this;
    }
    Polynomial<T>& operator*=(const Polynomial<T> &other) {
        std::vector<T> temp(_polynomial.size() + other._polynomial.size(), T(0));
        for (size_t i = 0; i != _polynomial.size(); ++i) {
            for (size_t j = 0; j != other._polynomial.size(); ++j) {
                temp[i + j] += _polynomial[i] * other._polynomial[j];
            }
        }
        _normalize(temp);
        *this = Polynomial(temp);
        return *this;
    }
    Polynomial<T>& operator*=(const T &num) {
        for (size_t i = 0; i != _polynomial.size(); ++i) {
            _polynomial[i] *= num;
        }
        _normalize(_polynomial);
        return *this;
    }
    T operator()(const T &point) const {
        T ans = T(0);
        for (auto iter = _polynomial.rbegin(); iter != _polynomial.rend(); ++iter) {
            ans += *iter;
            if ((iter + 1) != _polynomial.rend()) {
                ans *= point;
            }
        }
        return ans;
    }
    friend std::ostream& operator<<(std::ostream& out, const Polynomial<T> &pol) {
        bool flag = false;
        unsigned long long degree = pol._polynomial.size() - 1;
        for (auto iter = pol._polynomial.rbegin();
             iter != pol._polynomial.rend(); ++iter, --degree) {
            T coefs = *iter;
            if (coefs != T(0)) {
                if (coefs > T(0) && flag) {
                    out << '+';
                }
                flag = true;
                if (degree == 0) {
                    out << coefs;
                } else if (coefs == T(1)) {
                    out << 'x';
                } else if (coefs == T(-1)) {
                    out << "-x";
                } else {
                    out << coefs << "*x";
                }
                if (degree > 1) {
                    out << '^' << degree;
                }
            }
        }
        if (pol._polynomial.size() == 0) {
            out << 0;
        }
        return out;
    }
    friend Polynomial<T> operator&(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
        Polynomial<T> comp(lhs._polynomial.at(0));
        Polynomial<T> dupl_rhs(rhs._polynomial);
        size_t iter = 1;
        for (size_t degree = 1; degree != lhs._polynomial.size(); ++degree) {
            for (; iter != degree; ++iter) {
                dupl_rhs *= rhs;
            }
            comp += dupl_rhs * lhs._polynomial[degree];
        }
        return comp;
    }
    Polynomial<T> &operator/=(const Polynomial<T> &other) {
        Polynomial<T> priv(T(0));
        while (_polynomial.size() >= other._polynomial.size()) {
            T coefs = _polynomial.back() / other._polynomial.back();
            size_t degree = _polynomial.size() - other._polynomial.size();
            std::vector<T> div(degree + 1);
            div.back() = coefs;
            Polynomial<T> temp(div);
            *this -= other * temp;
            priv += temp;
        }
        _polynomial = priv._polynomial;
        return *this;
    }
    Polynomial<T> &operator%=(const Polynomial<T> &other) {
        Polynomial<T> quotient = *this / other;
        *this -= other * quotient;
        return *this;
    }
    auto begin() const {
        return _polynomial.begin();
    }
    auto end() const {
        return _polynomial.end();
    }
    friend Polynomial<T> operator,(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
        Polynomial<T> gcd = lhs;
        Polynomial<T> copy = rhs;
        while (copy._polynomial.size() != 0) {
            gcd %= copy;
            std::swap(gcd, copy);
        }
        if (gcd._polynomial.size() != 0) {
            Polynomial<T> temp(gcd[gcd._polynomial.size() - 1]);
            gcd /= temp;
        }
        return gcd;
    }
};

template <typename T>
Polynomial<T> operator*(Polynomial<T> lhs, const Polynomial<T> &rhs) {
    return lhs *= rhs;
}

template <typename T>
Polynomial<T> operator+(Polynomial<T> lhs, const Polynomial<T> &rhs) {
    return lhs += rhs;
}

template <typename T>
Polynomial<T> operator-(Polynomial<T> lhs, const Polynomial<T> &rhs) {
    return lhs -= rhs;
}

template<typename T>
Polynomial<T> operator/(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
    auto copy = lhs;
    copy /= rhs;
    return copy;
}

template<typename T>
Polynomial<T> operator%(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
    auto copy = lhs;
    copy %= rhs;
    return copy;
}

template <typename T>
Polynomial<T> operator+(Polynomial<T> poly, const T &num) {
    return poly += Polynomial<T>(num);
}

template <typename T>
Polynomial<T> operator+(const T &num, Polynomial<T> poly) {
    return poly += Polynomial<T>(num);
}

template <typename T>
Polynomial<T> operator-(Polynomial<T> poly, const T &num) {
    return poly -= Polynomial<T>(num);
}

template <typename T>
Polynomial<T> operator-(const T &num, Polynomial<T> poly) {
    return Polynomial<T>(num) -= poly;
}

template <typename T>
Polynomial<T> operator*(Polynomial<T> poly, const T &num) {
    return  poly *= Polynomial<T>(num);
}

template <typename T>
Polynomial<T> operator*(const T &num, Polynomial<T> poly) {
    return  poly *= Polynomial<T>(num);
}