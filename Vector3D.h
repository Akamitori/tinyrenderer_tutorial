#ifndef VECTOR3D_H
#define VECTOR3D_H


#include <istream>

struct Vector3D {
    float x;
    float y;
    float z;
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


#endif //VECTOR3D_H
