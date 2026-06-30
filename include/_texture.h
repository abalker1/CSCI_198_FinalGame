#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <_common.h>
#include <SOIL2.h>

class _texture
{
    public:
        _texture();
        virtual ~_texture();

        GLuint load(const char *a); // Load texture
        void bind(); // Bind texture

        unsigned char *img; // Handle image data
        int width, height;
        GLuint textureID;

    protected:

    private:
};

#endif // _TEXTURE_H
