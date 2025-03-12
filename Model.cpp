#include "Model.h"
#include <sstream>
#include "Vector2D.h"
#include "tgaimage.h"
#include "shape_drawing.h"
#include <iostream>

Vector2DInt project(Vector3D v, int width, int height) {
    Vector2DInt result = {
        static_cast<int>((v.x + 1.) * width / 2),
        static_cast<int>((v.y + 1.) * height / 2),
    };


    return result;
}

bool WireFrameDrawing(const std::string &fileName, const std::string &texture, const int width, const int height,
                      TGAImage &framebuffer) {
    //"data/diablo_pose.obj"
    std::ifstream in2(fileName, std::ios_base::in);

    TGAImage texture_buffer(1, 1, TGAImage::RGB);
    texture_buffer.read_tga_file(texture);
    texture_buffer.flip_vertically();

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
            m.faces.push_back(x);
        }

        if (line.starts_with("vt")) {
            s.str(line.substr(2));
            Vector3D x{};
            s >> x;
            m.texture_coords.push_back(x);
        }
    }

    std::cout << "vt are " << m.texture_coords.size() << std::endl;

    auto *zbuffer = new double[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = std::numeric_limits<double>::min();
    }


    Vector3D light_dir(0, 0, -1);

    for (auto const &face: m.faces) {
        auto a = m.vectors[face.vector_i1];
        auto b = m.vectors[face.vector_i2];
        auto c = m.vectors[face.vector_i3];

        Vector2DInt screen_coords[3] = {};
        Vector3D world_coords[3] = {a, b, c};


        auto proj_a = project(a, width, height);
        auto proj_b = project(b, width, height);
        auto proj_c = project(c, width, height);

        Vector3D v_a{(double) proj_a.x, (double) proj_a.y, a.z};
        Vector3D v_b{(double) proj_b.x, (double) proj_b.y, b.z};
        Vector3D v_c{(double) proj_c.x, (double) proj_c.y, c.z};

        auto t_a = m.texture_coords[face.texture_i1];
        auto t_b = m.texture_coords[face.texture_i2];
        auto t_c = m.texture_coords[face.texture_i3];

        Triangle t = {v_a, v_b, v_c, t_a, t_b, t_c};

        Vector3D triangle_normal = cross((world_coords[2] - world_coords[0]), world_coords[1] - world_coords[0]);
        triangle_normal = normalize(triangle_normal);

        double intensity = dot(triangle_normal, light_dir);

        if (intensity > 0) {
            triangle_with_texture(t, zbuffer, framebuffer, texture_buffer, width, intensity);
        }
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return true;
}
