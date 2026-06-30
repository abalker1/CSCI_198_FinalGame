#include "_fallAssignment.h"

// Behold, an abomination
_fallAssignment::_fallAssignment(float x, float y, float z,
                                 float w, float h, float d,
                                 float s) :
                    x(x), y(y), z(z),
                    w(w), h(h), d(d),
                    s(s) {}

_fallAssignment::~_fallAssignment()
{
    //dtor
}


void _fallAssignment::update(float dt)
{
    y -= s * dt;
}

void _fallAssignment::draw() const
{
    glPushMatrix();

        glTranslatef(x, y, z);
        glEnable(GL_TEXTURE_2D);

        const float hx = 0.5f * w, hy = 0.5f * h, hz = 0.5f * d;

        glBegin(GL_QUADS);
        // Right
        glNormal3f( 1, 0, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( hx,-hy,-hz);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( hx,-hy, hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( hx, hy, hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( hx, hy,-hz);
        // Left
        glNormal3f(-1, 0, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-hx,-hy,-hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-hx, hy,-hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Top
        glNormal3f( 0, 1, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-hx, hy,-hz);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( hx, hy,-hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( hx, hy, hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Bottom
        glNormal3f( 0,-1, 0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( hx,-hy, hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( hx,-hy,-hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx,-hy,-hz);
        // Front
        glNormal3f( 0, 0, 1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( hx,-hy, hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( hx, hy, hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Back
        glNormal3f( 0, 0,-1);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( hx,-hy,-hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( hx, hy,-hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, hy,-hz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-hx,-hy,-hz);
        glEnd();

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}
