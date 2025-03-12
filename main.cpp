#include "tgaimage.h"
#include "Model.h"
#include "shape_drawing.h"


constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};


void rasterize(Vector2DInt a, Vector2DInt b, TGAImage &, TGAColor, int *buffer);

void rasterise(Vector3D p0, Vector3D p1, TGAImage &image, TGAColor color, int *zbuffer);


int main(int argc, char **argv) {
    constexpr int width = 800;
    constexpr int height = 800;

    TGAImage render(width, height, TGAImage::RGB);

    TGAImage texture(width, height, TGAImage::RGB);
    TGAColor colors[3] = {red, green, blue};


    constexpr Vector3D a{0, 0, 0};
    constexpr Vector3D b   {800, 0, 0};
    constexpr Vector3D c{400, 800, 0.0};
    Triangle t = {  a,b,c};
    
    triangle_with_colors(t,render, colors );
    render.write_tga_file("triangle.tga");

    //WireFrameDrawing("data/african_head.obj", "data/african_head_diffuse.tga", width, height, render);

    return 0;
}
