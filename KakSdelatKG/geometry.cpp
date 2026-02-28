#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char* filename) : verts_(), faces_(), norms_(), uv_() {
    std::ifstream in;
    in.open(filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            verts_.push_back(v);
        }
        else if (!line.compare(0, 3, "vn ")) {
            iss >> trash >> trash;
            Vec3f n;
            for (int i = 0; i < 3; i++) iss >> n[i];
            norms_.push_back(n);
        }
        else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            for (int i = 0; i < 2; i++) iss >> uv[i];
            uv_.push_back(uv);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            Vec3i tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2]) {
                for (int i = 0; i < 3; i++) tmp[i]--;
                f.push_back(tmp);
            }
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# " << uv_.size() << " vn# " << norms_.size() << std::endl;
    load_texture(filename, "_diffuse.tga", diffusemap_);
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

std::vector<int> Model::face(int idx) {
    std::vector<int> face;
    for (int i = 0; i < (int)faces_[idx].size(); i++) face.push_back(faces_[idx][i][0]);
    return face;
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

void Model::load_texture(std::string filename, const char* suffix, TGAImage& img) {
    std::string texfile(filename);
    size_t dot = texfile.find_last_of(".");
    if (dot != std::string::npos) {
        texfile = texfile.substr(0, dot) + std::string(suffix);
        std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
        img.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2i uv) {
    int u = std::max(0, std::min(diffusemap_.get_width() - 1, uv.x));
    int v = std::max(0, std::min(diffusemap_.get_height() - 1, uv.y));
    return diffusemap_.get(u, v);
}

Vec2i Model::uv(int iface, int nvert) {
    int idx = faces_[iface][nvert][1];
    int u = (int)(uv_[idx].x * (float)diffusemap_.get_width());
    int v = (int)(uv_[idx].y * (float)diffusemap_.get_height());

    u = std::max(0, std::min(diffusemap_.get_width() - 1, u));
    v = std::max(0, std::min(diffusemap_.get_height() - 1, v));

    return Vec2i(u, v);
}

void Model::compute_vertex_normals() {
    // Пересчитываем нормали всегда (игнорируем загруженные)
    norms_.clear();
    norms_.resize(nverts(), Vec3f(0, 0, 0));

    for (size_t i = 0; i < faces_.size(); i++) {
        std::vector<Vec3i> f = faces_[i];
        Vec3f v0 = verts_[f[0][0]];
        Vec3f v1 = verts_[f[1][0]];
        Vec3f v2 = verts_[f[2][0]];
        Vec3f n = (v2 - v0) ^ (v1 - v0);
        n.normalize();

        for (const auto &vert: f) {
            int idx = vert[0]; // индекс вершины
            norms_[idx] = norms_[idx] + n;
        }
    }

    for (auto &n: norms_) {
        n.normalize();
    }

    // Обновляем индексы нормалей в гранях, чтобы они указывали на индекс вершины
    for (size_t i = 0; i < faces_.size(); i++) {
        for (size_t j = 0; j < faces_[i].size(); j++) {
            faces_[i][j][2] = faces_[i][j][0]; // индекс нормали = индекс вершины
        }
    }
}

Vec3f Model::normal(int iface, int nvert) const {
    int idx = faces_[iface][nvert][2]; // третий компонент — индекс нормали
    return norms_[idx];
}
