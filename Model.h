

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <istream>
#include "Vector3D.h"


class TGAImage;

struct FaceData {
    int i1;
    int i2;
    int i3;
};


inline std::istream &operator>>(std::istream &is, FaceData &v) {
    is>>v.i1;
    is.ignore(100,' ');
    is>>v.i2;
    is.ignore(100,' ');
    is>>v.i3;
    return is;
}

inline std::ostream &operator<<(std::ostream &os, const FaceData &v) {
  
    os << "{" << v.i1 << "," << v.i2 << "," << v.i3 << "}";
    return os;
}


struct Model {
    std::vector<Vector3D> vectors;
    std::vector<FaceData> faces;
};

bool WireFrameDrawing(std::string fileName, const int width, const int height, TGAImage &framebuffer);


#endif //MODEL_H
