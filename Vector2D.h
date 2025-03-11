#ifndef VECTOR2D_H
#define VECTOR2D_H


struct Vector2D {
    double x;
    double y;
};


struct Vector2DInt {
    int x;
    int y;
};

inline Vector2DInt operator-(const Vector2DInt &a, const Vector2DInt &b) {
    return {
        a.x - b.x,
        a.y - b.y
    };
}

inline Vector2DInt operator /(const Vector2DInt &a, const int b) {
    return {
        a.x / b,
        a.y / b
    };
}


#endif //VECTOR2D_H
