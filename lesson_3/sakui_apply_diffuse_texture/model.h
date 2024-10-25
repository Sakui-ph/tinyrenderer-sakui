#ifndef __MODEL_H__
#define __MODEL_H__

#include <vector>
#include "geometry.h"

class Model {
private:
	std::vector<Vec3f> verts_;
	std::vector<std::vector<int> > faces_;
	std::vector<Vec3f> verttex_;
	std::vector<std::vector<int> > facetex_;
public:
	Model(const char *filename);
	~Model();
	int nverts();
	int nfaces();
	int nverttex();
	int nfacetex();
	Vec3f vert(int i);
	std::vector<int> face(int idx);
	Vec3f verttex(int i);
	std::vector<int> facetex(int idx);
};

#endif //__MODEL_H__
