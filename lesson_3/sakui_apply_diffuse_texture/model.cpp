#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), faces_(), verttex_(), facetex_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<int> f, ft;
            int itrash, idx, ft_idx;
            iss >> trash;
            while (iss >> idx >> trash >> ft_idx >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                ft_idx--;
                ft.push_back(ft_idx);
                f.push_back(idx);
            }
            faces_.push_back(f);
            facetex_.push_back(ft);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash;
            Vec3f vt;
            for (int i=0;i<3;i++) iss >> vt.raw[i];
            verttex_.push_back(vt);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << " vt# " << verttex_.size() << " ft# " << facetex_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

int Model::nverttex() {
    return (int)verttex_.size();
}

int Model::nfacetex() {
    return (int)facetex_.size();
}

std::vector<int> Model::face(int idx) {
    return faces_[idx];
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec3f Model::verttex(int i) {
    return verttex_[i];
}

std::vector<int> Model::facetex(int idx) {
    return facetex_[idx];
}

