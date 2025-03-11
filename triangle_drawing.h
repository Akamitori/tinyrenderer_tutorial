#ifndef TRIANGLE_DRAWING_H
#define TRIANGLE_DRAWING_H

#include "Vector2D.h"
class TGAImage;
class TGAColor;

struct Triangle{
    Vector2DInt vertices[3];
};

void triangle_scanline(Triangle triangle, TGAImage &framebuffer, TGAColor color);
void triangle_bounding(Triangle triangle, TGAImage &framebuffer, TGAColor color);
void triangle(Triangle triangle, TGAImage &framebuffer, TGAColor color);





#endif //TRIANGLE_DRAWING_H
