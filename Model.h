#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <istream>
#include "Vector3D.h"


class TGAImage;

struct FaceData {
    int vector_i1;
    int vector_i2;
    int vector_i3;

    int texture_i1;
    int texture_i2;
    int texture_i3;
};


inline std::istream &operator>>(std::istream &is, FaceData &v) {
    is >> v.vector_i1;
    is.ignore(10, '/');
    is >> v.texture_i1;

    is.ignore(100, ' ');
    is >> v.vector_i2;
    is.ignore(10, '/');
    is >> v.texture_i2;

    is.ignore(100, ' ');
    is >> v.vector_i3;
    is.ignore(10, '/');
    is >> v.texture_i3;
    
    --v.vector_i1;
    --v.vector_i2;
    --v.vector_i3;
    --v.texture_i1;
    --v.texture_i2;
    --v.texture_i3;
    
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const FaceData &v) {
    os << "{" << v.vector_i1 << "," << v.vector_i2 << "," << v.vector_i3 << "}";
    return os;
}


struct Model {
    std::vector<Vector3D> vectors;
    std::vector<FaceData> faces;

    std::vector<Vector3D> texture_coords;
};

bool WireFrameDrawing(const std::string& fileName, const std::string& texture, int width, int height, TGAImage &framebuffer);


#endif //MODEL_H
