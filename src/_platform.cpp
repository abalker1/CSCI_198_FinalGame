#include "_platform.h"

_platform::_platform(float x, float y, float z,
                    float w, float h, float d,
                    float u, float v) :

                    x(x), y(y), z(z),
                    w(w), h(h), d(d),
                    tileU(u), tileV(v) {}

_platform::~_platform()
{
    //dtor
}

void _platform::draw() const
{
    glPushMatrix();

        glTranslatef(x, y, z);
        glEnable(GL_TEXTURE_2D);

        const float hx = 0.5f * w, hy = 0.5f * h, hz = 0.5f * d;

        auto tex = [&](float u, float v) {
            glTexCoord2f(u * tileU, v * tileV);
        };

        glBegin(GL_QUADS);
        // Right
        glNormal3f( 1, 0, 0);
        tex(0.0f, 1.0f); glVertex3f( hx,-hy,-hz);
        tex(1.0f, 1.0f); glVertex3f( hx,-hy, hz);
        tex(1.0f, 0.0f); glVertex3f( hx, hy, hz);
        tex(0.0f, 0.0f); glVertex3f( hx, hy,-hz);
        // Left
        glNormal3f(-1, 0, 0);
        tex(0.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        tex(1.0f, 1.0f); glVertex3f(-hx,-hy,-hz);
        tex(1.0f, 0.0f); glVertex3f(-hx, hy,-hz);
        tex(0.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Top
        glNormal3f( 0, 1, 0);
        tex(0.0f, 1.0f); glVertex3f(-hx, hy,-hz);
        tex(1.0f, 1.0f); glVertex3f( hx, hy,-hz);
        tex(1.0f, 0.0f); glVertex3f( hx, hy, hz);
        tex(0.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Bottom
        glNormal3f( 0,-1, 0);
        tex(0.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        tex(1.0f, 1.0f); glVertex3f( hx,-hy, hz);
        tex(1.0f, 0.0f); glVertex3f( hx,-hy,-hz);
        tex(0.0f, 0.0f); glVertex3f(-hx,-hy,-hz);
        // Front
        glNormal3f( 0, 0, 1);
        tex(0.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        tex(1.0f, 1.0f); glVertex3f( hx,-hy, hz);
        tex(1.0f, 0.0f); glVertex3f( hx, hy, hz);
        tex(0.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Back
        glNormal3f( 0, 0,-1);
        tex(0.0f, 1.0f); glVertex3f( hx,-hy,-hz);
        tex(1.0f, 1.0f); glVertex3f( hx, hy,-hz);
        tex(1.0f, 0.0f); glVertex3f(-hx, hy,-hz);
        tex(0.0f, 0.0f); glVertex3f(-hx,-hy,-hz);
        glEnd();

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}
