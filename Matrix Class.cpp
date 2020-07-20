#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

template <typename T>
class Matrix {
private:
    std::vector<std::vector<T>> mat;
    size_t rows = mat.size();
    size_t cols = mat[0].size();
    std::vector<T> vecmat;

public:
    Matrix(const std::vector<std::vector<T>>& matrix) : mat(matrix) {
        for (auto& v : mat) {
            for (T el : v) {
                vecmat.push_back(el);
            }
        }
    }
    size_t Rows() const noexcept {
        return mat.size();
    }
    size_t Cols() const noexcept {
        return mat[0].size();
    }
    std::vector<T> operator[] (size_t i) const {
        return mat[i];
    }
    std::vector<T>& operator[] (size_t i) {
        return mat[i];
    }
    std::pair<size_t, size_t> size() const {
        return std::make_pair(mat.size(), mat[0].size());
    }
    Matrix& operator=(const Matrix& rhs) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                mat[i][j] = rhs[i][j];
            }
        }
        return *this;
    }
    Matrix& operator+=(const Matrix& rhs) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                mat[i][j] += rhs[i][j];
            }
        }
        return *this;
    }
    Matrix operator+(const Matrix& rhs) const {
        Matrix result = mat;
        result += rhs;
        return result;
    }
    Matrix operator*(const Matrix& other) const {
        assert(Cols() == other.Rows());
        Matrix<T> temp = mat;
        temp *= other;
        return temp;
    }
    Matrix& operator*=(const Matrix& other) {
        assert(Cols() == other.Rows());
        Matrix<T> temp = mat;
        size_t maxCol = std::max(Cols(), other.Cols());
        for (auto& v : mat) {
            v.resize(maxCol);
        }
        T sum_elems;
        for (size_t i = 0; i < Rows(); ++i) {
            for (size_t j = 0; j < other.Cols(); ++j) {
                sum_elems = 0;
                for (size_t k = 0; k < other.Rows(); ++k) {
                    sum_elems += temp[i][k] * other[k][j];
                }
                mat[i][j] = sum_elems;
            }
        }
        mat.resize(Rows());
        for (auto& v : mat) {
            v.resize(other.Cols());
        }
        return *this;
    }
    Matrix& operator*=(const T& scalar) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                mat[i][j] *= scalar;
            }
        }
        return *this;
    }
    Matrix operator*(const T& scalar) const {
        Matrix temp = mat;
        temp *= scalar;
        return temp;
    }
    Matrix transposed() const {
        std::vector<std::vector<T>> temp(Cols(), std::vector<T>(Rows(), 0));
        Matrix newMat = temp;
        for (size_t i = 0; i < Rows(); ++i) {
            for (size_t j = 0; j < Cols(); ++j) {
                temp[j][i] = mat[i][j];
            }
        }
        return temp;
    }
    Matrix& transpose() {
        size_t n = std::max(Rows(), Cols());
        size_t r = Rows();
        size_t c = Cols();
        mat.resize(n);
        for (size_t i = 0; i < n; ++i) {
            mat[i].resize(n);
        }
        for (size_t i = 0; i < n; i++) {
            for (size_t j = i + 1; j < n; j++) {
                std::swap(mat[i][j], mat[j][i]);
            }
        }
        mat.resize(c);
        for (size_t k = 0; k < mat.size(); k++) {
            mat[k].resize(r);
        }
        return *this;
    }
    typename std::vector<T>::iterator begin() {
        return vecmat.begin();
    }
    typename std::vector<T>::iterator end() {
        return vecmat.end();
    }
    const typename std::vector<T>::const_iterator begin() const {
        return vecmat.begin();
    }
    const typename std::vector<T>::const_iterator end() const {
        return vecmat.end();
    }
};
template<typename T>
std::ostream& operator<<(std::ostream& out, const Matrix<T>& m) {
    for (size_t i = 0; i < m.Rows(); ++i) {
        for (size_t j = 0; j < m.Cols(); ++j) {
            if (j == m.Cols() - 1) {
                out << m[i][j];
            } else {
                out << m[i][j] << '\t';
            }
        }
        if (i != m.Rows() - 1) {
            out <<"\n";
        } else {
        }
    }
    return out;
}