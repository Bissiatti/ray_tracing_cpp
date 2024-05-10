#include <iostream>
#include <cmath>
#pragma once

class vec4 {
public:
    float x, y, z, w;
    vec4() : x(0), y(0), z(0), w(0) {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    vec4 operator+(const vec4& other) const {
        return vec4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    vec4 operator-(const vec4& other) const {
        return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    vec4 operator*(const vec4& other) const {
        return vec4(x * other.x, y * other.y, z * other.z, w * other.w);
    }

    vec4 operator/(const vec4& other) const {
        return vec4(x / other.x, y / other.y, z / other.z, w / other.w);
    }

    vec4& operator+=(const vec4& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
    }

    vec4& operator-=(const vec4& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
    }

    vec4& operator*=(const vec4& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
    }

    vec4& operator/=(const vec4& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
    }

    bool operator==(const vec4& other) const {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool operator!=(const vec4& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& stream, const vec4& vector) {
        stream << "[" << vector.x << ", " << vector.y << ", " << vector.z << ", "
                << vector.w << "]";
        return stream;
    };

    vec4 operator*(float scalar) const {
        return vec4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    vec4 operator/(float scalar) const {
        return vec4(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    vec4& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        w *= scalar;
        return *this;
    }

    vec4& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        w /= scalar;
        return *this;
    }

    float length() const {
        return sqrt(x * x + y * y + z * z + w * w);
    }

    vec4 unit() const {
        return *this / length();
    }

    float dot(const vec4& other) const {
        return x * other.x + y * other.y + z * other.z + w * other.w;
    }
};

class vec3 {
public:
    float x, y, z;
    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    float get(int i) const {
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        } else if (i == 2) {
            return z;
        }
    }

    vec3 operator+(const vec3& other) const {
        return vec3(x + other.x, y + other.y, z + other.z);
    }

    vec3 operator-(const vec3& other) const {
        return vec3(x - other.x, y - other.y, z - other.z);
    }

    vec3 operator*(const vec3& other) const {
        return vec3(x * other.x, y * other.y, z * other.z);
    }

    vec3 cross(const vec3& other) const {
        return vec3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }

    float dot(const vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float max() const {
        return std::max(x, std::max(y, z));
    }

    float imax() const {
        return x > y ? (x > z ? 0 : 2) : (y > z ? 1 : 2);
    }

    float min() const {
        return std::min(x, std::min(y, z));
    }

    float imin() const {
        return x < y ? (x < z ? 0 : 2) : (y < z ? 1 : 2);
    }

    float maxAbs() const {
        return std::max(std::abs(x), std::max(std::abs(y), std::abs(z)));
    }

    float imaxAbs() const {
        return std::abs(x) > std::abs(y) ? (std::abs(x) > std::abs(z) ? 0 : 2) : (std::abs(y) > std::abs(z) ? 1 : 2);
    }

    float minAbs() const {
        return std::min(std::abs(x), std::min(std::abs(y), std::abs(z)));
    }

    float iminAbs() const {
        return std::abs(x) < std::abs(y) ? (std::abs(x) < std::abs(z) ? 0 : 2) : (std::abs(y) < std::abs(z) ? 1 : 2);
    }

    vec3 operator/(const vec3& other) const {
        return vec3(x / other.x, y / other.y, z / other.z);
    }

    vec3& operator+=(const vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    vec3& operator-=(const vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    vec3& operator*=(const vec3& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
    }

    vec3& operator/=(const vec3& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
    }

    bool operator==(const vec3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool operator!=(const vec3& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& stream, const vec3& vector) {
        stream << "[" << vector.x << ", " << vector.y << ", " << vector.z << "]";
        return stream;
    };

    vec3 operator*(float scalar) const {
        return vec3(x * scalar, y * scalar, z * scalar);
    }

    vec3 operator/(float scalar) const {
        return vec3(x / scalar, y / scalar, z / scalar);
    }

    vec3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    vec3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    vec3 max(const vec3& other) const {
        if (x+ y + z > other.x + other.y + other.z) {
            return *this;
        } else {
            return other;
        }
    }

    float length() const {
        return sqrt(x * x + y * y + z * z);
    }

    vec3 unit() const {
        return *this / length();
    }
}; 

class matrix3 {
public:
    vec3 rows[3];
    float d = INFINITY;
    matrix3() : rows{vec3(), vec3(), vec3()} {}
    matrix3(vec3 row1, vec3 row2, vec3 row3) : rows{row1, row2, row3} {}

    matrix3(float m00, float m01, float m02,
            float m10, float m11, float m12,
            float m20, float m21, float m22) :
            rows{vec3(m00, m01, m02), vec3(m10, m11, m12), vec3(m20, m21, m22)} {}

    matrix3 ones () {
        return matrix3(1, 1, 1, 1, 1, 1, 1, 1, 1);
    };

    matrix3 zeros () {
        return matrix3(0, 0, 0, 0, 0, 0, 0, 0, 0);
    };

    matrix3 diag(vec3 diag) {
        return matrix3(diag.x, 0, 0, 0, diag.y, 0, 0, 0, diag.z);
    };

    matrix3 diag(float diag) {
        return matrix3(diag, 0, 0, 0, diag, 0, 0, 0, diag);
    };
        
    matrix3 operator+(const matrix3& other) const {
        return matrix3(rows[0] + other.rows[0], rows[1] + other.rows[1], rows[2] + other.rows[2]);
    }

    matrix3 operator-(const matrix3& other) const {
        return matrix3(rows[0] - other.rows[0], rows[1] - other.rows[1], rows[2] - other.rows[2]);
    }

    matrix3 operator*(const matrix3& other) const {
        matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    result.rows[i].x += rows[i].x * other.rows[k].x;
                    result.rows[i].y += rows[i].y * other.rows[k].y;
                    result.rows[i].z += rows[i].z * other.rows[k].z;
                }
            }
        }
        return result;
    }

    matrix3 operator/(const matrix3& other) const {
        matrix3 result;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    result.rows[i].x /= rows[i].x / other.rows[k].x;
                    result.rows[i].y /= rows[i].y / other.rows[k].y;
                    result.rows[i].z /= rows[i].z / other.rows[k].z;
                }
            }
        }
        return result;
    }

    matrix3& operator+=(const matrix3& other) {
        rows[0] += other.rows[0];
        rows[1] += other.rows[1];
        rows[2] += other.rows[2];
        return *this;
    }

    matrix3& operator-=(const matrix3& other) {
        rows[0] -= other.rows[0];
        rows[1] -= other.rows[1];
        rows[2] -= other.rows[2];
        return *this;
    }

    matrix3& operator*=(const matrix3& other) {
        *this = *this * other;
        return *this;
    }

    matrix3& operator/=(const matrix3& other) {
        *this = *this / other;
        return *this;
    }

    bool operator==(const matrix3& other) const {
        return rows[0] == other.rows[0] && rows[1] == other.rows[1] && rows[2] == other.rows[2];
    }

    bool operator!=(const matrix3& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& stream, const matrix3& matrix) {
        stream << "[" << endl;
        stream << matrix.rows[0] << endl;
        stream << matrix.rows[1] << endl;
        stream << matrix.rows[2] << endl;
        stream << "]";

        return stream;
    };

    static matrix3 identity() {
        return matrix3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    }

    matrix3 T(){
        return matrix3(rows[0].x, rows[1].x, rows[2].x, rows[0].y, rows[1].y, rows[2].y, rows[0].z, rows[1].z, rows[2].z);
    }

    float det(){
        d = rows[0].x * (rows[1].y * rows[2].z - rows[1].z * rows[2].y) - rows[0].y * (rows[1].x * rows[2].z - rows[1].z * rows[2].x) + rows[0].z * (rows[1].x * rows[2].y - rows[1].y * rows[2].x);
        return d;
    };

    matrix3 inv(){
        matrix3 result;
        if (d == INFINITY){
            det();
        }
        if (d == 0){
            std::cout << "Matrix is singular" << std::endl;
            return result;
        }
         
        result.rows[0].x = (rows[1].y * rows[2].z - rows[1].z * rows[2].y) / d;
        result.rows[0].y = (rows[0].z * rows[2].y - rows[0].y * rows[2].z) / d;
        result.rows[0].z = (rows[0].y * rows[1].z - rows[0].z * rows[1].y) / d;
        result.rows[1].x = (rows[1].z * rows[2].x - rows[1].x * rows[2].z) / d;
        result.rows[1].y = (rows[0].x * rows[2].z - rows[0].z * rows[2].x) / d;
        result.rows[1].z = (rows[0].z * rows[1].x - rows[0].x * rows[1].z) / d;
        result.rows[2].x = (rows[1].x * rows[2].y - rows[1].y * rows[2].x) / d;
        result.rows[2].y = (rows[0].y * rows[2].x - rows[0].x * rows[2].y) / d;
        result.rows[2].z = (rows[0].x * rows[1].y - rows[0].y * rows[1].x) / d;
        return result;
    }
};

// matrix3 * vec3

vec3 operator*(const matrix3& matrix, const vec3& vector) {
    return vec3(matrix.rows[0].dot(vector), matrix.rows[1].dot(vector), matrix.rows[2].dot(vector));
}

// vec3 operator*(const vec3& vector, const matrix3& matrix) {
//     return vec3(matrix.rows[0].dot(vector), matrix.rows[1].dot(vector), matrix.rows[2].dot(vector));
// }

class matrix4 {
public:
    vec4 rows[4];
    float d = INFINITY;
    matrix4() : rows{vec4(), vec4(), vec4(), vec4()} {}
    matrix4(vec4 row1, vec4 row2, vec4 row3, vec4 row4) : rows{row1, row2, row3, row4} {}

    matrix4(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) :
            rows{vec4(m00, m01, m02, m03), vec4(m10, m11, m12, m13), vec4(m20, m21, m22, m23), vec4(m30, m31, m32, m33)} {}

    matrix4 ones () {
        return matrix4(1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1 ,1, 1, 1, 1, 1);
    };

    matrix4 zeros () {
        return matrix4(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
    };

    matrix4 diag(vec4 diag) {
        return matrix4(diag.x, 0, 0, 0, 0, diag.y, 0, 0, 0, 0, diag.z, 0, 0, 0, 0, diag.w);
    };

    matrix4 diag(float diag) {
        return matrix4(diag, 0, 0, 0, 0, diag, 0, 0, 0, 0, diag, 0, 0, 0, 0, diag);
    };

    matrix4 operator+(const matrix4& other) const {
        return matrix4(rows[0] + other.rows[0], rows[1] + other.rows[1], rows[2] + other.rows[2], rows[3] + other.rows[3]);
    }

    matrix4 operator-(const matrix4& other) const {
        return matrix4(rows[0] - other.rows[0], rows[1] - other.rows[1], rows[2] - other.rows[2], rows[3] - other.rows[3]);
    }

    matrix4 operator*(const matrix4& other) const {
        matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    result.rows[i].x += rows[i].x * other.rows[k].x;
                    result.rows[i].y += rows[i].y * other.rows[k].y;
                    result.rows[i].z += rows[i].z * other.rows[k].z;
                    result.rows[i].w += rows[i].w * other.rows[k].w;
                }
            }
        }
        return result;
    }

    matrix4 operator/(const matrix4& other) const {
        matrix4 result;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                for (int k = 0; k < 4; k++) {
                    result.rows[i].x /= rows[i].x / other.rows[k].x;
                    result.rows[i].y /= rows[i].y / other.rows[k].y;
                    result.rows[i].z /= rows[i].z / other.rows[k].z;
                    result.rows[i].w /= rows[i].w / other.rows[k].w;
                }
            }
        }
        return result;
    }

    matrix4& operator+=(const matrix4& other) {
        rows[0] += other.rows[0];
        rows[1] += other.rows[1];
        rows[2] += other.rows[2];
        rows[3] += other.rows[3];
        return *this;
    }

    matrix4& operator-=(const matrix4& other) {
        rows[0] -= other.rows[0];
        rows[1] -= other.rows[1];
        rows[2] -= other.rows[2];
        rows[3] -= other.rows[3];
        return *this;
    }

    matrix4& operator*=(const matrix4& other) {
        *this = *this * other;
        return *this;
    }

    matrix4& operator/=(const matrix4& other) {
        *this = *this / other;
        return *this;
    }

    bool operator==(const matrix4& other) const {
        return rows[0] == other.rows[0] && rows[1] == other.rows[1] && rows[2] == other.rows[2] && rows[3] == other.rows[3];
    }

    bool operator!=(const matrix4& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& stream, const matrix4& matrix) {
        stream << "[" << endl;
        stream << matrix.rows[0] << endl;
        stream << matrix.rows[1] << endl;
        stream << matrix.rows[2] << endl;
        stream << matrix.rows[3] << endl;
        stream << "]";

        return stream;
    };

    static matrix4 identity() {
        return matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }

    matrix4 T(){
        return matrix4(rows[0].x, rows[1].x, rows[2].x, rows[3].x, rows[0].y, rows[1].y, rows[2].y, rows[3].y, rows[0].z, rows[1].z, rows[2].z, rows[3].z, rows[0].w, rows[1].w, rows[2].w, rows[3].w);
    }

    float det(){
        return rows[0].x * (rows[1].y * rows[2].z - rows[1].z * rows[2].y) - rows[0].y * (rows[1].x * rows[2].z - rows[1].z * rows[2].x) + rows[0].z * (rows[1].x * rows[2].y - rows[1].y * rows[2].x);
    };

    matrix4 inv(){
        matrix4 result;
        if (d == INFINITY){
            det();
        }
        if (d == 0){
            std::cout << "Matrix is singular" << std::endl;
            return result;
        }
        result.rows[0].x = (rows[1].y * rows[2].z - rows[1].z * rows[2].y) / d;
        result.rows[0].y = (rows[0].z * rows[2].y - rows[0].y * rows[2].z) / d;
        result.rows[0].z = (rows[0].y * rows[1].z - rows[0].z * rows[1].y) / d;
        result.rows[1].x = (rows[1].z * rows[2].x - rows[1].x * rows[2].z) / d;
        result.rows[1].y = (rows[0].x * rows[2].z - rows[0].z * rows[2].x) / d;
        result.rows[1].z = (rows[0].z * rows[1].x - rows[0].x * rows[1].z) / d;
        result.rows[2].x = (rows[1].x * rows[2].y - rows[1].y * rows[2].x) / d;
        result.rows[2].y = (rows[0].y * rows[2].x - rows[0].x * rows[2].y) / d;
        result.rows[2].z = (rows[0].x * rows[1].y - rows[0].y * rows[1].x) / d;
        return result;
    }
};

