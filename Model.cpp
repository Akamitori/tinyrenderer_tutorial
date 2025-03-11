#include "Model.h"
#include <sstream>
#include "Vector2D.h"
#include "tgaimage.h"
#include "triangle_drawing.h"
#include <iostream>

Vector2DInt project(Vector3D v, int width, int height) {
    Vector2DInt result = {
        static_cast<int>((v.x + 1.) * width / 2),
        static_cast<int>((v.y + 1.) * height / 2),
    };


    return result;
}

bool WireFrameDrawing(std::string fileName, const int width, const int height, TGAImage &framebuffer) {
    //"data/diablo_pose.obj"
    std::ifstream in2(fileName, std::ios_base::in);


    if (!in2) {
        std::cerr << "Failed to open file!" << std::endl;
        return false;
    }

    if (!in2.is_open()) {
        return false;
    }

    std::istringstream s;

    Model m{};


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


    Vector3D light_dir(0, 0, -1);

    for (auto const &face: m.faces) {
        auto a = m.vectors[face.i1];
        auto b = m.vectors[face.i2];
        auto c = m.vectors[face.i3];

        Vector2DInt screen_coords[3] = {};
        Vector3D world_coords[3] = {a, b, c};

        Triangle t={project(a, width, height),
            project(b, width, height),
          project(c, width, height),
        };
        
        Vector3D triangle_normal = cross((world_coords[2] - world_coords[0]), world_coords[1] - world_coords[0]);
        triangle_normal = normalize(triangle_normal);

        double intensity = dot(triangle_normal, light_dir);

        if (intensity > 0) {
            auto c = TGAColor{static_cast<uint8_t>(intensity * 255),
                static_cast<uint8_t>(intensity * 255),
                static_cast<uint8_t>(intensity * 255),
                255};
            triangle(t, framebuffer, c);
        }
    }
    
    framebuffer.write_tga_file("framebuffer.tga");
    return true;
}
