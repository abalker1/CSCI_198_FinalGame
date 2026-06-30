#include "_parallax.h"

_parallax::_parallax()
{
    // Set default texture coordinates
    xMin = yMin = 0.0;
    xMax = yMax = 1.0;

}

_parallax::~_parallax()
{
    // Make sure we clean up after ourselves
    delete texture;
}

// Draw a parallax image to the screen at a depth of z (This implementation normalizes the size of the parallax to the size of the image rather than the screen)
void _parallax::draw(float z)
{
    // Set color to white so it draws with the expected colors and bind the texture
    glColor3f(1.0, 1.0, 1.0);
    texture->bind();

    // Calculate aspect ratio normalized coordinates for parallax drawing
    float textureAR = static_cast<float>(texture->width) / static_cast<float>(texture->height);
    float quadHalfHeight = 1.0;
    float quadHalfWidth = quadHalfHeight * textureAR;

    // Draw the parallax texture to a polygon on screen
    glBegin(GL_POLYGON);

        glTexCoord2f(xMin,yMax); glVertex3f(-quadHalfWidth, -quadHalfHeight, z);

        glTexCoord2f(xMax,yMax); glVertex3f(quadHalfWidth, -quadHalfHeight, z);

        glTexCoord2f(xMax,yMin); glVertex3f(quadHalfWidth, quadHalfHeight, z);

        glTexCoord2f(xMin,yMin); glVertex3f(-quadHalfWidth, quadHalfHeight, z);

    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

}

// Initialize the texture to be used for the parallax image
void _parallax::init(char* fileName)
{
    texture->load(fileName);
}

// Scroll the parallax in the specified direction
void _parallax::scroll(string dir , float s, float dt)
{
    // Normalize speed of parallax movement using time delta
    s *= dt;

    // Determine proper direction for movement based on passed string
    if(dir == "left"){xMin +=s;xMax += s;}
    else if(dir == "right"){xMin -=s;xMax -= s;}
    else if(dir == "up"){yMin -=s;yMax-=s;}
    else if(dir == "down"){yMin +=s;yMax+=s;}

}

// Car level compatibility methods

void _parallax::drawParallax(float w, float h)
{
    glColor3f(1.0,1.0,1.0);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    texture->bind();


   glBegin(GL_POLYGON);
    //bottom left
    glTexCoord2f(xMin,yMax);
    glVertex3f(-w/h, -1,0);
    //bottom right
    glTexCoord2f(xMax,yMax);
    glVertex3f(w/h, -1,0);

    //top right
    glTexCoord2f(xMax,yMin);
    glVertex3f(w/h,2,0);
    //top left
    glTexCoord2f(xMin,yMin);
    glVertex3f(-w/h, 2,0);

   glEnd();
    //reset lighting to default
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void _parallax::drawParallaxRoad(float w, float h)
{
    glColor3f(1.0, 1.0, 1.0);
    texture->bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    float texScale = 4.0f; // This must match 'textureRepeats'
    float tMin = yMin * texScale;
    float tMax = yMax * texScale;
    glBegin(GL_QUADS);

    // Bottom-Left
    glTexCoord2f(xMin, yMin);
    glVertex3f(-85.0f, 10.0f, -50.0f);

    // Bottom-Right
    glTexCoord2f(xMax, yMin);
    glVertex3f(85.0f, 10.0f, -50.0f);

    // Top-Right
    glTexCoord2f(xMax, yMax);
    glVertex3f(85.0f, 0.0f, 80.0f);

    // Top-Left
    glTexCoord2f(xMin, yMax);
    glVertex3f(-85.0f, 0.0f, 80.0f);

    glEnd();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
