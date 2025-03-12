#include "shape_drawing.h"
#include "tgaimage.h"
#include <algorithm>
#include <assert.h>
#include <iostream>


double signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5 * ((by - ay) * (bx + ax) + (cy - by) * (cx + bx) + (ay - cy) * (ax + cx));
}

void triangle_with_texture(Triangle triangle, double *zbuffer, TGAImage &framebuffer, TGAImage &texturebuffer,
                           int width,
                           double intensity) {
    auto a = triangle.vertices[0];
    auto b = triangle.vertices[1];
    auto c = triangle.vertices[2];

    int bbminx = std::min(std::min(a.x, b.x), c.x); // bounding box for the triangle
    int bbminy = std::min(std::min(a.y, b.y), c.y); // defined by its top left and bottom right corners
    int bbmaxx = std::max(std::max(a.x, b.x), c.x);
    int bbmaxy = std::max(std::max(a.y, b.y), c.y);
    double total_area = signed_triangle_area(a.x, a.y, b.x, b.y, c.x, c.y);

    if (total_area < 1) return; // backface culling + discarding triangles that cover less than a pixel


#pragma omp parallel for
    for (int x = bbminx; x <= bbmaxx; x++) {
        for (int y = bbminy; y <= bbmaxy; y++) {
            double alpha = signed_triangle_area(x, y, b.x, b.y, c.x, c.y) / total_area;
            double beta = signed_triangle_area(x, y, c.x, c.y, a.x, a.y) / total_area;
            double gamma = signed_triangle_area(x, y, a.x, a.y, b.x, b.y) / total_area;

            if (alpha < -1e-6 || beta < -1e-6 || gamma < -1e-6) continue;

            double weights[3] = {alpha, beta, gamma};

            auto z = 0.0;
            auto u = 0.0;
            auto v = 0.0;
            for (int i = 0; i < 3; i++) {
                // interpolate z on the triangle using the barometric weights
                z += triangle.vertices[i].z * weights[i];
                // interpolate u and v on the triangle using u and w
                u += triangle.texture_coords[i].x * weights[i];
                v += triangle.texture_coords[i].y * weights[i];
            }


            // map the u and v to the actual texture pixels
            int tex_x = u * (texturebuffer.width() - 1);
            int tex_y = v * (texturebuffer.height() - 1);


            assert(("x is in range", tex_x< texturebuffer.width()));
            assert(("y is in range", tex_y< texturebuffer.height()));

            // get the pixels and apply intensity to them
            auto color = texturebuffer.get(tex_x, tex_y);
            color[0] *= intensity;
            color[1] *= intensity;
            color[2] *= intensity;
            color[3] = 255;


            const int zIndex = int(x + y * width);
            if (zbuffer[zIndex] < z) {
                zbuffer[zIndex] = z;
                framebuffer.set(x, y, color);
            }
        }
    }
}

void triangle_with_colors(Triangle triangle, TGAImage &framebuffer, TGAColor colors[]) {
    auto a = triangle.vertices[0];
    auto b = triangle.vertices[1];
    auto c = triangle.vertices[2];

    int bbminx = std::min(std::min(a.x, b.x), c.x); // bounding box for the triangle
    int bbminy = std::min(std::min(a.y, b.y), c.y); // defined by its top left and bottom right corners
    int bbmaxx = std::max(std::max(a.x, b.x), c.x);
    int bbmaxy = std::max(std::max(a.y, b.y), c.y);
    double total_area = signed_triangle_area(a.x, a.y, b.x, b.y, c.x, c.y);

    if (total_area < 1) return; // backface culling + discarding triangles that cover less than a pixel


#pragma omp parallel for
    for (int x = bbminx; x <= bbmaxx; x++) {
        for (int y = bbminy; y <= bbmaxy; y++) {
            double alpha = signed_triangle_area(x, y, b.x, b.y, c.x, c.y) / total_area;
            double beta = signed_triangle_area(x, y, c.x, c.y, a.x, a.y) / total_area;
            double gamma = signed_triangle_area(x, y, a.x, a.y, b.x, b.y) / total_area;

            if (alpha < -1e-6 || beta < -1e-6 || gamma < -1e-6) continue;

            double weights[3] = {alpha, beta, gamma};

            auto z = 0.0;
            TGAColor color{};

            for (int i = 0; i < 3; i++) {
                // interpolate z on the triangle using the barometric weights
                // interpolate u and v on the triangle using u and w
                for (int j = 0; j < 3; j++) {
                    color[i] += colors[j][i] * weights[j];
                }
            }

            framebuffer.set(x, y, color);
        }
    }
}


void line(Vector2DInt a, Vector2DInt b, TGAImage &framebuffer, TGAColor color) {
    bool steep = std::abs(a.x - b.x) < std::abs(a.y - b.y);
    if (steep) {
        // if the line is steep, we transpose the image
        std::swap(a, b);
    }
    if (a.x > b.x) {
        // make it left−to−right
        std::swap(a, b);
    }
    int y = a.y;
    int ierror = 0;
    for (int x = a.x; x <= b.x; x++) {
        if (steep) // if transposed, de−transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        ierror += 2 * std::abs(b.y - a.y);
        if (ierror > b.x - a.x) {
            y += b.y > a.y ? 1 : -1;
            ierror -= 2 * (b.x - a.x);
        }
    }
}
