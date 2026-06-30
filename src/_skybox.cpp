#include "_skybox.h"

_skybox::_skybox()
{
    //ctor
}

_skybox::~_skybox()
{
    delete texture;
}

void _skybox::skyboxInit()
{
    xMin = yMin =0;
    xMax = yMax =1;

    rotation.x= rotation.y=rotation.z = 0;
    pos.x = pos.y =0;
    pos.z = 0.0;

    boxSize.x = boxSize.z = 30.0;
    boxSize.y = 5.0;

}

void _skybox::draw()
{
    glDisable(GL_LIGHTING); // if you are using room comment this line
    glEnable(GL_TEXTURE_2D);

    glPushMatrix();

    glColor3f(1.0, 1.0, 1.0);// set environment to white
    glScalef(boxSize.x,boxSize.y,boxSize.z);

    glRotatef(rotation.x, 1,0,0);
    glRotatef(rotation.y, 0,1,0);
    glRotatef(rotation.z, 0,0,1);

    //front Wall

    glBindTexture(GL_TEXTURE_2D,tex[0]);
    //glNormal3f()  use this only if you are setting room with lighting
    glBegin(GL_QUADS);
    glTexCoord2f(xMin,yMin); glVertex3f(-1.0,1.0,1.0);
    glTexCoord2f(xMax,yMin); glVertex3f(1.0,1.0,1.0);
    glTexCoord2f(xMax,yMax); glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(xMin,yMax); glVertex3f(-1.0,-1.0,1.0);
    glEnd();

    //Back Wall

    glBindTexture(GL_TEXTURE_2D,tex[1]);
    //glNormal3f()  use this only if you are setting room with lighting
    glBegin(GL_QUADS);
    glTexCoord2f(xMin,yMax); glVertex3f(1.0,-1.0,-1.0);
    glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,-1.0);
    glTexCoord2f(xMax,yMin); glVertex3f(-1.0, 1.0,-1.0);
    glTexCoord2f(xMin,yMin); glVertex3f(1.0,1.0,-1.0);
    glEnd();

    //Top Wall

    glBindTexture(GL_TEXTURE_2D,tex[2]);
    //glNormal3f()  use this only if you are setting room with lighting
    glBegin(GL_QUADS);
    glTexCoord2f(xMin,yMax); glVertex3f(1.0,1.0,-1.0);
    glTexCoord2f(xMin,yMin); glVertex3f(1.0,1.0,1.0);
    glTexCoord2f(xMax,yMin); glVertex3f(-1.0,1.0,1.0);
    glTexCoord2f(xMax,yMax); glVertex3f(-1.0,1.0,-1.0);
    glEnd();

    //Bottom Wall

    glBindTexture(GL_TEXTURE_2D,tex[3]);
    //glNormal3f()  use this only if you are setting room with lighting
    glBegin(GL_QUADS);
    glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,1.0);
    glTexCoord2f(xMin,yMax); glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(xMin,yMin); glVertex3f(1.0,-1.0,-1.0);
    glTexCoord2f(xMax,yMin); glVertex3f(-1.0,-1.0,-1.0);
    glEnd();

    //Right Wall

    glBindTexture(GL_TEXTURE_2D,tex[5]);
    //glNormal3f()  use this only if you are setting room with lighting
    glBegin(GL_QUADS);
    glTexCoord2f(xMax,yMin); glVertex3f(1.0,1.0,-1.0);
    glTexCoord2f(xMax,yMax); glVertex3f(1.0,-1.0,-1.0);
    glTexCoord2f(xMin,yMax); glVertex3f(1.0,-1.0,1.0);
    glTexCoord2f(xMin,yMin); glVertex3f(1.0,1.0,1.0);
    glEnd();

    //Left Wall

    glBindTexture(GL_TEXTURE_2D,tex[4]);
    //glNormal3f()  use this only if you are setting room with lighting
    glBegin(GL_QUADS);
    glTexCoord2f(xMin,yMin); glVertex3f(-1.0,1.0,-1.0);
    glTexCoord2f(xMax,yMin); glVertex3f(-1.0,1.0,1.0);
    glTexCoord2f(xMax,yMax); glVertex3f(-1.0,-1.0,1.0);
    glTexCoord2f(xMin,yMax); glVertex3f(-1.0,-1.0,-1.0);
    glEnd();

    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, 0); // Clean up texture bindings so other things don't wear the skybox's skin
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
}
