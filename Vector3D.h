#ifndef VECTOR3D_H
#define VECTOR3D_H


#include <istream>

struct Vector3D {
    double x;
    double y;
    double z;
};


inline std::istream &operator>>(std::istream &is, Vector3D &v) {
    is >> v.x >> v.y >> v.z;
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vector3D &v) {
    os << "{" << v.x << "," << v.y << "," << v.z << "}";
    return os;
}

struct Vector3DInt {
    int x;
    int y;
    int z;
};


inline std::istream &operator>>(std::istream &is, Vector3DInt &v) {
    is >> v.x >> v.y >> v.z;
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const Vector3DInt &v) {
    os << "{" << v.x << "," << v.y << "," << v.z << "}";
    return os;
}

inline Vector3D operator-(const Vector3D &a, const Vector3D &b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vector3D operator*(const Vector3D &a, const double v) {
    return {a.x * v, a.y * v, a.z * v};
}


inline Vector3D cross(const Vector3D &a, const Vector3D &b) {
    return Vector3D(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
}

inline double magnitude(const Vector3D &a) {
    return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

inline Vector3D normalize(const Vector3D &a) {
    const double div = 1 / magnitude(a);

    return a * div;
}

inline double dot(const Vector3D &a, const Vector3D &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}


#endif //VECTOR3D_H
