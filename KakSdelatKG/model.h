#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"
#include "tgaimage.h"

class Model {
private:
    std::vector<Vec3f> verts_;  // vershins (x, y, z)
    std::vector<std::vector<Vec3i> > faces_;   // grani
    std::vector<Vec3f> norms_; // normali vershin
    std::vector<Vec2f> uv_;  // texture coordinats (u, v)
    TGAImage diffusemap_; // diffusnai texture
    void load_texture(std::string filename, const char* suffix, TGAImage& img);
public:
    Model(const char* filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vert(int i);
    Vec2i uv(int iface, int nvert);
    TGAColor diffuse(Vec2i uv);
    std::vector<int> face(int idx);
    Vec3f normal(int iface, int nvert) const; // нормаль для грани iface, вершины nvert (индекс 0,1,2)
    void compute_vertex_normals(); // гарантирует наличие нормалей (если их нет в файле — вычислит)
};

#endif //__MODEL_H__
