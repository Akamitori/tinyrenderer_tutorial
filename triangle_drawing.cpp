#include "triangle_drawing.h"
#include "tgaimage.h"
#include <algorithm>

void triangle(Triangle triangle, TGAImage &framebuffer, TGAColor color) {
#ifdef BOUNDING
    triangle_bounding(triangle, framebuffer, color);
#else
    triangle_scanline(triangle, framebuffer, color);
#endif
}


void triangle_scanline(Triangle triangle, TGAImage &framebuffer, TGAColor color) {
    constexpr struct {
        bool operator()(const Vector2DInt a, const Vector2DInt b) const { return a.y < b.y; }
    }
            sort_by_height;

    std::sort(&triangle.vertices[0], &triangle.vertices[3], sort_by_height);

    auto a = triangle.vertices[0];
    auto b = triangle.vertices[1];
    auto c = triangle.vertices[2];

    int total_height = c.y - a.y;

    if (a.y != b.y) {
        int segment_height = b.y - a.y;

        for (int y = a.y; y <= b.y; y++) {
            int x1 = a.x + ((c.x - a.x) * (y - a.y)) / total_height;
            int x2 = a.x + ((b.x - a.x) * (y - a.y)) / segment_height;

            for (int x = std::min(x1, x2); x < std::max(x1, x2); x++) // draw a horizontal line
                framebuffer.set(x, y, color);
        }
    }
    if (b.y != c.y) {
        // if the upper half is not degenerate
        int segment_height = c.y - b.y;
        for (int y = b.y; y <= c.y; y++) {
            // sweep the horizontal line from by to cy
            int x1 = a.x + ((c.x - a.x) * (y - a.y)) / total_height;
            int x2 = b.x + ((c.x - b.x) * (y - b.y)) / segment_height;
            for (int x = std::min(x1, x2); x < std::max(x1, x2); x++) // draw a horizontal line
                framebuffer.set(x, y, color);
        }
    }
}

double signed_triangle_area(int ax, int ay, int bx, int by, int cx, int cy) {
    return .5 * ((by - ay) * (bx + ax) + (cy - by) * (cx + bx) + (ay - cy) * (ax + cx));
}

void triangle_bounding(Triangle triangle, TGAImage &framebuffer, TGAColor color) {
    auto a = triangle.vertices[0];
    auto b = triangle.vertices[1];
    auto c = triangle.vertices[2];

    int bbminx = std::min(std::min(a.x, b.x), c.x); // bounding box for the triangle
    int bbminy = std::min(std::min(a.y, b.y), c.y); // defined by its top left and bottom right corners
    int bbmaxx = std::max(std::max(a.x, b.x), c.x);
    int bbmaxy = std::max(std::max(a.y, b.y), c.y);
    double total_area = signed_triangle_area(a.x, a.y, b.x, b.y, c.x, c.y);

    //if (total_area < 1) return; // backface culling + discarding triangles that cover less than a pixel


#pragma omp parallel for
    for (int x = bbminx; x <= bbmaxx; x++) {
        for (int y = bbminy; y <= bbmaxy; y++) {
            double alpha = signed_triangle_area(x, y, b.x, b.y, c.x, c.y) / total_area;
            double beta = signed_triangle_area(x, y, c.x, c.y, a.x, a.y) / total_area;
            double gamma = signed_triangle_area(x, y, a.x, a.y, b.x, b.y) / total_area;
            if (alpha < 0 || beta < 0 || gamma < 0) continue;
            // negative barycentric coordinate => the pixel is outside the triangle
            framebuffer.set(x, y, color);
        }
    }
}
