#include "_texture.h"

_texture::_texture()
{
    //ctor
    textureID = 0; // No garbage values
}

_texture::~_texture()
{
    //dtor
}

GLuint _texture::load(const char* filename)
{
    glGenTextures(1,&textureID);
    glBindTexture(GL_TEXTURE_2D,textureID);

    img = SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA);

    if(!img) {
        std::cerr << "[ERROR] File did not load: " << filename; // Rewrote this part because if we're going to print an error we might as well use std::cerr instead of std::cout.
    }

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,img);
    SOIL_free_image_data(img);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    return textureID;
}

void _texture::bind()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textureID);
}

