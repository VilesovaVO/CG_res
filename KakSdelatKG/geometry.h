#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <algorithm>

template <class t> struct Vec2 {
    t x, y;

    Vec2() : x(t()), y(t()) {}
    Vec2(t _x, t _y) : x(_x), y(_y) {}

    template <class u> Vec2(const Vec2<u>& v) : x(t(v.x)), y(t(v.y)) {}

    Vec2<t>& operator=(const Vec2<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
        }
        return *this;
    }

    Vec2<t> operator +(const Vec2<t>& V) const { return Vec2<t>(x + V.x, y + V.y); }
    Vec2<t> operator -(const Vec2<t>& V) const { return Vec2<t>(x - V.x, y - V.y); }
    Vec2<t> operator *(float f) const { return Vec2<t>(x * f, y * f); }

    t& operator[](const int i) {
        if (i <= 0) return x;
        else return y;
    }

    const t& operator[](const int i) const {
        if (i <= 0) return x;
        else return y;
    }
};

template <class t> struct Vec3 {
    t x, y, z;

    Vec3() : x(t()), y(t()), z(t()) {}
    Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}

    template <class u> Vec3(const Vec3<u>& v) : x(t(v.x)), y(t(v.y)), z(t(v.z)) {}

    Vec3<t>& operator=(const Vec3<t>& v) {
        if (this != &v) {
            x = v.x;
            y = v.y;
            z = v.z;
        }
        return *this;
    }

    Vec3<t> operator ^(const Vec3<t>& v) const {
        return Vec3<t>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }

    Vec3<t> operator +(const Vec3<t>& v) const {
        return Vec3<t>(x + v.x, y + v.y, z + v.z);
    }

    Vec3<t> operator -(const Vec3<t>& v) const {
        return Vec3<t>(x - v.x, y - v.y, z - v.z);
    }

    Vec3<t> operator *(float f) const {
        return Vec3<t>(x * f, y * f, z * f);
    }

    friend Vec3<t> operator*(float f, const Vec3<t>& v) {
        return Vec3<t>(v.x * f, v.y * f, v.z * f);
    }

    Vec3<t> operator/(float f) const {
        return Vec3<t>(x / f, y / f, z / f);
    }

    t operator *(const Vec3<t>& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    float norm() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3<t>& normalize(t l = 1) {
        *this = (*this) * (l / norm());
        return *this;
    }

    Vec3<t> reflect(const Vec3<t>& normal) const {
        //R = I - 2*(I·N)*N
        float dot_product = (*this) * normal;
        return *this - normal * (2.0f * dot_product);
    }

    t& operator[](const int i) {
        if (i == 0) return x;
        else if (i == 1) return y;
        else return z;
    }

    const t& operator[](const int i) const {
        if (i == 0) return x;
        else if (i == 1) return y;
        else return z;
    }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;

template <class t>
std::ostream& operator<<(std::ostream& s, const Vec2<t>& v) {
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}

template <class t>
std::ostream& operator<<(std::ostream& s, const Vec3<t>& v) {
    s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return s;
}

class Matrix {
private:
    std::vector<std::vector<float> > m;
    int rows, cols;

public:
    Matrix(int r = 4, int c = 4) : rows(r), cols(c) {
        m.resize(rows);
        for (int i = 0; i < rows; i++) {
            m[i].resize(cols, 0);
        }
    }

    static Matrix identity(int dimensions) {
        Matrix E(dimensions, dimensions);
        for (int i = 0; i < dimensions; i++) {
            E[i][i] = 1;
        }
        return E;
    }

    int nrows() const { return rows; }
    int ncols() const { return cols; }

    std::vector<float>& operator[](const int i) {
        assert(i >= 0 && i < rows);
        return m[i];
    }

    const std::vector<float>& operator[](const int i) const {
        assert(i >= 0 && i < rows);
        return m[i];
    }


    Matrix operator*(const Matrix& a) const {
        assert(cols == a.rows);
        Matrix result(rows, a.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < a.cols; j++) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < cols; k++) {
                    result.m[i][j] += m[i][k] * a.m[k][j];
                }
            }
        }
        return result;
    }

    Vec3f operator*(const Vec3f& v) const {
        assert(rows == 4 && cols == 4);
        float x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        float y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        float z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        float w = m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3];

        if (w != 0.0f) {
            x /= w;
            y /= w;
            z /= w;
        }

        return Vec3f(x, y, z);
    }

    Matrix transpose() const {
        Matrix result(cols, rows);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                result[j][i] = m[i][j];
            }
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& s, const Matrix& m) {
        for (int i = 0; i < m.nrows(); i++) {
            for (int j = 0; j < m.ncols(); j++) {
                s << m[i][j] << "\t";
            }
            s << "\n";
        }
        return s;
    }
};

#endif // GEOMETRY_H
