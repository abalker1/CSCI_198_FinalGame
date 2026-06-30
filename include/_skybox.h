#ifndef _SKYBOX_H
#define _SKYBOX_H

#include <_common.h>
#include <_texture.h>

class _skybox
{
    public:
        _skybox();
        virtual ~_skybox();

        _texture *texture = new _texture();

        GLuint tex[6]; // all the images
        vec3 pos;      // position of the skybox
        vec3 rotation; // handle skybox rotations
        vec3 boxSize;  // scale the skybox

        float xMin,yMin,xMax,yMax;
        void skyboxInit(); // load images
        void draw();

    protected:

    private:
};

#endif // _SKYBOX_H
