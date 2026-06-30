#ifndef _PARALLAX_H
#define _PARALLAX_H

#include<_common.h>
#include<_texture.h>

class _parallax
{
    public:
        _parallax();
        virtual ~_parallax();

        _texture *texture = new _texture();         // Texture loader to load, store, and bind textures

        void draw(float z);                         // Draws full screen parallax image at depth z
        void init(char* filename);                  // Initialize and load image
        void scroll(string dir , float s, float dt);// Direction, speed, time normalization

        void drawParallax(float w, float h);        // Width & Height (CarLevel compatibility)
        void drawParallaxRoad(float w, float h);    // Width & Height (CarLevel compatibility)

        float xMin, xMax, yMin,yMax;                // Texture coordinates

    protected:

    private:
};

#endif // _PARALLAX_H
