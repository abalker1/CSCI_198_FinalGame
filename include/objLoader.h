#pragma once
#include <_common.h>

struct OBJVertex { float x, y, z; };
struct OBJTexCoord { float u, v; };
struct OBJNormal { float nx, ny, nz; };

struct OBJFace {
    vector<int> vIndex;
    vector<int> tIndex;
    vector<int> nIndex;
};

class OBJModel {
public:
    vector<OBJVertex> vertices;
    vector<OBJTexCoord> texCoords;
    vector<OBJNormal> normals;
    vector<OBJFace> faces;

    bool load(const string& filename);
    void draw();
};

