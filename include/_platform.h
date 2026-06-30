#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <_common.h>

class _platform
{
    public:
        _platform(float x, float y, float z, float w, float h, float d, float u, float v);
        virtual ~_platform();

        float x, y, z;  // Pos
        float w, h, d;  // Dim
        vec3 halfExtents = {0.5f * w, 0.5f * h, 0.5f * d};
        float topY = y + 0.5f * h;
        float tileU, tileV; // Determines tiling of textures on platform

        void draw() const;

    protected:

    private:
};

#endif // _PLATFORM_H
