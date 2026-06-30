#include "_uiObject.h"

_uiObject::_uiObject()
{
    // Initialize member values
    this->tex = new _texture();
    this->cx = 0;
    this->cy = 0;
    this->w  = 0;
    this->h  = 0;
    this->visible = false; // UI stuff shouldn't be visible until it's had it's setFrame called
}

_uiObject::~_uiObject()
{
    delete tex;
}

// Sets the frame of the UI object so that it can be drawn on screen
void _uiObject::setFrame(float cx, float cy, float w, float h)
{
    // Update position values (frame) of UI object
    this->cx = cx;
    this->cy = cy;
    this->w  = w;
    this->h  = h;
    this->visible = true;
}

// Get the aspect ratio of the UI object to ensure proper resizing of UI objects when the game is resized
float _uiObject::getAspectRatio()
{
    // Return a default value of 1 if the tex has improper width or height
    // This is just so things don't completely break, but they still wont render right
    if (tex->width <= 0 || tex->height <= 0)
    {
        return 1.0f;
    }

    // If the width and height are sensible values, return the aspect ratio
    return static_cast<float>(tex->height) / static_cast<float>(tex->width);
}

// Initialize the UI object - This needs to be overwritten for classes with extra functionality that need to have more values initialized
void _uiObject::init(const char* texPath)
{
    tex->load(texPath);
}

// Draws the UI Object
void _uiObject::draw()
{
    // Don't draw invisible objects, dummy.
    if (!visible) {
        return;
    }

    // Calculate positions for quad coordinates
    float hw = w * 0.5;
    float hh = h * 0.5;
    float l = cx - hw;
    float r = cx + hw;
    float t = cy - hh;
    float b = cy + hh;

    glPushMatrix();
        // Set color to opaque white so UI Object renders its tex as expected
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        tex->bind();
        // It's making the buttons ugly by smoothing them out
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // Start drawing the quad
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(l, b); // Bottom left
            glTexCoord2f(1.0f, 1.0f); glVertex2f(r, b); // Bottom right
            glTexCoord2f(1.0f, 0.0f); glVertex2f(r, t); // Top right
            glTexCoord2f(0.0f, 0.0f); glVertex2f(l, t); // Top left
        glEnd();
    glPopMatrix();
}
