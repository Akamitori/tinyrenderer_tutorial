#ifndef TRIANGLE_DRAWING_H
#define TRIANGLE_DRAWING_H

#include "Vector2D.h"
#include "Vector3D.h"
class TGAImage;
class TGAColor;

struct Triangle {
    Vector3D vertices[3] = {};
    Vector3D texture_coords[3] = {};
};


void triangle_with_texture(Triangle triangle, double *zbuffer, TGAImage &framebuffer, TGAImage &texturebuffer, int width,double intensity);
void triangle_with_colors(Triangle triangle, TGAImage &framebuffer, TGAColor colors[]);

void line(Vector2D a, Vector2D b, TGAImage &framebuffer, TGAColor color);


#endif //TRIANGLE_DRAWING_H
