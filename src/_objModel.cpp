#include "_objModel.h"
#include <objLoader.h>

_objModel::_objModel()
{
    //ctor
    rotateX = 0;
    rotateY = 0;
    rotateZ = 0.0;

    posX = 1;
    posY = -2;
    posZ = -8.0;

    scale = 5;

    horizontalPos = 0.0f;
    targetHorizontalPos = 0.0f;
    movementSpeed = 0.005f; // tweak these values
}

_objModel::~_objModel()
{
    //dtor
}

void _objModel::loadMyOBJ(const char* filename) {
    if (!myObj.load(filename)) {
        std::cerr << "Failed to load OBJ file" << std::endl;
    }
}

void _objModel::drawModel()
{
    glPushMatrix();          // start group

    glTranslated(posX,posY,posZ); //translation
    glRotated(rotateX,1,0,0);     //rotate around X-Axis
    glRotated(rotateY,0,1,0);     //rotate around Y-Axis
    glRotated(rotateZ,0,0,1);     //rotate around Z-Axis

    glScaled(scale,scale,scale);  // Scale your model
    myObj.draw();
    glPopMatrix();                 //End group
}

void _objModel::update()
{
    // Each frame, move the horizontalPos a little bit closer to the target.
    horizontalPos += (targetHorizontalPos - horizontalPos) * movementSpeed;
    posX = horizontalPos; // Apply the smooth position to the model's actual posX
}

