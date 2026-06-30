#include "objLoader.h"
#include <_common.h>
#include <sstream>

bool OBJModel::load(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open OBJ file: " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string type;
        iss >> type;

        if (type == "v") {
            OBJVertex v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        }
        else if (type == "vt") {
            OBJTexCoord t;
            iss >> t.u >> t.v;
            t.v = 1.0f - t.v;
            texCoords.push_back(t);
        }
        else if (type == "vn") {
            OBJNormal n;
            iss >> n.nx >> n.ny >> n.nz;
            normals.push_back(n);
        }
        else if (type == "f") {
            OBJFace f;
            string vert;
            while (iss >> vert) {
                int vIdx = 0, tIdx = 0, nIdx = 0;
                sscanf(vert.c_str(), "%d/%d/%d", &vIdx, &tIdx, &nIdx);
                f.vIndex.push_back(vIdx - 1);
                f.tIndex.push_back(tIdx - 1);
                f.nIndex.push_back(nIdx - 1);
            }
            faces.push_back(f);
        }
    }
    return true;
}

void OBJModel::draw() {
    for (const auto& f : faces) {
        if (f.vIndex.size() == 3)
            glBegin(GL_TRIANGLES);
        else if (f.vIndex.size() == 4)
            glBegin(GL_QUADS);
        else
            continue;

        for (size_t i = 0; i < f.vIndex.size(); i++) {
            if (f.nIndex[i] >= 0 && f.nIndex[i] < (int)normals.size()) {
                const auto& n = normals[f.nIndex[i]];
                glNormal3f(n.nx, n.ny, n.nz);
            }
            if (f.tIndex[i] >= 0 && f.tIndex[i] < (int)texCoords.size()) {
                const auto& t = texCoords[f.tIndex[i]];
                glTexCoord2f(t.u, t.v);
            }
            if (f.vIndex[i] >= 0 && f.vIndex[i] < (int)vertices.size()) {
                const auto& v = vertices[f.vIndex[i]];
                glVertex3f(v.x, v.y, v.z);
            }
        }
        glEnd();
    }
}

