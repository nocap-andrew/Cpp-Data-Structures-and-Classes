#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <utility>

template <typename T>
class Polynomial {
private:
    std::map<size_t, T> _polynomial;

    void normalize(std::map<size_t, T> &coefs) {
        for (auto iter = coefs.begin(); iter  != coefs.end();) {
            if (iter->second == T(0)) {
                iter = coefs.erase(iter);
            } else {
                iter++;
            }
        }
    }

public:
    Polynomial<T>(const std::vector<T> &coefs) {
        for (size_t i = 0; i != coefs.size(); ++i) {
            _polynomial[i];
            _polynomial[i] = coefs[i];
        }
        normalize(_polynomial);
    }
    template <typename Iter>
    Polynomial<T>(Iter first, Iter last) {
        _polynomial.clear();
        size_t degree = 0;
        for (; first != last; ++degree) {
            _polynomial[degree];
            _polynomial[degree] = *first++;
        }
        normalize(_polynomial);
    }
    Polynomial<T>(const T &num = T()) {
        _polynomial.clear();
        _polynomial[0];
        _polynomial[0] = num;
        normalize(_polynomial);
    }
    bool operator == (const Polynomial<T> &other) {
        return _polynomial == other._polynomial;
    }
    bool operator != (const Polynomial<T> &other) {
        return _polynomial != other._polynomial;
    }
    bool operator == (const T &num) {
        return _polynomial == Polynomial<T>(num)._polynomial;
    }
    bool operator != (const T &num) {
        return _polynomial != Polynomial<T>(num)._polynomial;
    }
    T operator[](size_t i) const {
        if (_polynomial.find(i) != _polynomial.end()) {
            return _polynomial.at(i);
        } else {
            return T(0);
        }
    }
    T operator () (const T& point) const {
        if (_polynomial.empty()) {
            return T(0);
        } else {
            T ans = T(0);
            T x = T(1);
            for (int i = 0; i != Degree() + 1; ++i) {
                if (_polynomial.count(i)) {
                    ans += _polynomial.at(i) * x;
                }
                x *= point;
            }
            return ans;
        }
    }
    Polynomial& operator *= (const Polynomial& other) {
        if (_polynomial.empty() || other._polynomial.empty()) {
            Polynomial<T> temp(T(0));
            _polynomial = temp._polynomial;
            return *this;
        }
        size_t deg = Degree() + other.Degree() + 2;
        std::vector <T> temp(deg + 1);
        for (size_t i = 0; i != temp.size(); ++i) {
            temp[i] = T(0);
        }
        for (auto it1 = other._polynomial.begin(); it1 != other._polynomial.end(); ++it1) {
            for (auto it2 = _polynomial.begin(); it2 != _polynomial.end(); ++it2) {
                temp[it1->first + it2->first] += it1->second * it2->second;
            }
        }
        Polynomial<T> copy(temp);
        _polynomial = copy._polynomial;
        normalize(_polynomial);
        return *this;
    }
    Polynomial& operator *= (const T& other) {
        for (auto it = _polynomial.begin(); it != _polynomial.end(); ++it) {
            it->second *= other;
        }
        normalize(_polynomial);
        return *this;
    }
    int Degree() const {
        if (_polynomial.empty()) {
            return -1;
        } else {
            return static_cast<int>(_polynomial.rbegin()->first);
        }
    }
    Polynomial<T>& operator += (const Polynomial<T> &other) {
        for (auto el : other._polynomial) {
            _polynomial[el.first];
            _polynomial[el.first] += el.second;
        }
        normalize(_polynomial);
        return *this;
    }
    Polynomial<T>& operator -= (const Polynomial<T> &other) {
        for (auto el : other._polynomial) {
            _polynomial[el.first];
            _polynomial[el.first] -= el.second;
        }
        normalize(_polynomial);
        return *this;
    }
    friend std::ostream& operator << (std::ostream& out, const Polynomial<T> &pol) {
        bool state = false;
        for (auto iter = pol._polynomial.rbegin(); iter != pol._polynomial.rend(); ++iter) {
            T coefs = iter->second;
            size_t degree = iter->first;
            if (coefs > T(0) && state) {
                out << '+';
            }
            state = true;
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
        if (pol._polynomial.size() == 0) {
            out << 0;
        }
        return out;
    }
    friend Polynomial<T> operator & (const Polynomial<T>& lhs, const Polynomial<T>& rhs) {
        Polynomial<T> res;
        if (lhs._polynomial.find(0) != lhs._polynomial.end()) {
            res += lhs._polynomial.find(0)->second;
        }
        Polynomial<T> dupl_rhs = rhs;
        size_t degree = 1;
        for (auto iter = lhs._polynomial.begin(); iter != lhs._polynomial.end(); ++iter) {
            if (iter->first != 0) {
                while (degree != iter->first) {
                    dupl_rhs *= rhs;
                    ++degree;
                }
                res += dupl_rhs * iter->second;
            }
        }
        return res;
    }
    Polynomial<T> operator/=(const Polynomial<T> &other) {
        Polynomial<T> res(T(0));
        while (this->Degree() >= other.Degree()) {
            int deg = this->Degree() - other.Degree();
            T k = _polynomial.rbegin()->second / other._polynomial.rbegin()->second;
            std::map<size_t, T> temp;
            temp[deg] = k;
            Polynomial<T> pol;
            pol._polynomial = temp;
            res += pol;
            *this -= other * pol;
        }
        *this = res;
        return *this;
    }
    Polynomial<T> &operator%=(const Polynomial<T> &other) {
        Polynomial<T> temp = *this / other;
        *this -= other * temp;
        normalize(_polynomial);
        return *this;
    }
    friend Polynomial<T> operator,(const Polynomial<T>& first, const Polynomial<T>& second) {
        Polynomial<T> gcd = first, dupl_second = second;
        while (dupl_second._polynomial.size() != 0) {
            gcd %= dupl_second;
            std::swap(gcd, dupl_second);
        }
        if (gcd._polynomial.size() != 0) {
            Polynomial<T> div(gcd._polynomial.rbegin()->second);
            gcd /= div;
        }
        return gcd;
    }
    auto begin() const {
        return _polynomial.begin();
    }
    auto end() const {
        return _polynomial.end();
    }
};

template <typename T>
Polynomial<T> operator +(Polynomial<T> lhs, const Polynomial<T> &rhs) {
    return lhs += rhs;
}
template <typename T>
Polynomial<T> operator -(Polynomial<T> lhs, const Polynomial<T> &rhs) {
    return lhs -= rhs;
}
template <typename T>
Polynomial<T> operator *(Polynomial<T> lhs, const Polynomial<T> &rhs) {
    return lhs *= rhs;
}
template<typename T>
Polynomial<T> operator /(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
    auto copy = lhs;
    copy /= rhs;
    return copy;
}
template<typename T>
Polynomial<T> operator %(const Polynomial<T> &lhs, const Polynomial<T> &rhs) {
    auto copy = lhs;
    copy %= rhs;
    return copy;
}
template <typename T>
Polynomial<T> operator +(Polynomial<T> polyChlen, const T &num) {
    return polyChlen += Polynomial<T>(num);
}
template <typename T>
Polynomial<T> operator +(const T &num, Polynomial<T> polyChlen) {
    return polyChlen += Polynomial<T>(num);
}
template <typename T>
Polynomial<T> operator -(Polynomial<T> polyChlen, const T &num) {
    return polyChlen -= Polynomial<T>(num);
}
template <typename T>
Polynomial<T> operator -(const T &num, Polynomial<T> polyChlen) {
    return Polynomial<T>(num) -= polyChlen;
}
template <typename T>
Polynomial<T> operator *(Polynomial<T> polyChlen, const T &num) {
    return polyChlen *= Polynomial<T>(num);
}
template <typename T>
Polynomial<T> operator *(const T &num, Polynomial<T> polyChlen) {
    return polyChlen *= Polynomial<T>(num);
}