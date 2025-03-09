#include "tgaimage.h"
#include <cmath>
#include <iostream>
#include "Vector3D.h"
#include "FaceData.h"
#include <iostream>
#include <sstream>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void line(int ax, int ay, int bx, int by, TGAImage &framebuffer, TGAColor color) {
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) {
        std::swap(ax, ay);
        std::swap(bx, by);
    }

    if (ax > bx) {
        // make it left−to−right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    for (float x = ax; x <= bx; x++) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round(ay + (by - ay) * t);

        if (steep) {
            framebuffer.set(y, x, color);
        } else {
            framebuffer.set(x, y, color);
        }
    }
}

Vector3DInt project(Vector3D v, int width, int height) {
    Vector3DInt result= {
        static_cast<int>((v.x + 1.) * width / 2),
        static_cast<int>((v.y + 1.) * height / 2),
        0
    };


    return result;
}

struct Model {
    std::vector<Vector3D> vectors;
    std::vector<FaceData> faces;
};

int main(int argc, char **argv) {
    std::ifstream in2("data/diablo_pose.obj", std::ios_base::in);


    if (!in2) {
        std::cerr << "Failed to open file!" << std::endl;
        return -1;
    }

    if (!in2.is_open()) {
        throw;
    }

    std::istringstream s;

    Model m{};
    m.faces.reserve(6000);
    m.vectors.reserve(3000);
    

    for (std::string line; std::getline(in2, line);) {
        if (!line.starts_with("v ") && !line.starts_with("f ")) {
            continue;
        }

        s.clear();
        if (line.starts_with("v ")) {
            s.str(line.substr(1));
            Vector3D x{};
            s >> x;
            m.vectors.push_back(x);
        }

        if (line.starts_with("f ")) {
            s.str(line.substr(1));
            FaceData x{};
            s >> x;
            x.i1--;
            x.i2--;
            x.i3--;
            m.faces.push_back(x);
        }
    }


    constexpr int width = 800;
    constexpr int height = 800;
    TGAImage framebuffer(width, height, TGAImage::RGB);


    for (auto &f: m.faces) {
        auto a = m.vectors[f.i1];
        auto b = m.vectors[f.i2];
        auto c = m.vectors[f.i3];

        auto a_i = project(a, width, height);
        auto b_i = project(b, width, height);
        auto c_i = project(c, width, height);

        auto ax = a_i.x;
        auto ay = a_i.y;

        auto bx = b_i.x;
        auto by = b_i.y;

        auto cx = c_i.x;
        auto cy = c_i.y;
        
        line(ax, ay, bx, by, framebuffer, red);
        line(bx, by, cx, cy, framebuffer, red);
        line(cx, cy, ax, ay, framebuffer, red);
    }

    for (auto &v: m.vectors) {
        Vector3DInt a = project(v, width, height);
        framebuffer.set(a.x, a.y, white);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
