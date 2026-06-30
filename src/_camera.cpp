#include "_camera.h"

_camera::_camera()
{
    reset();
}

_camera::~_camera()
{
    //dtor
}

// Resets the camera to its default settings
void _camera::reset()
{
    // Reset the camera to default settings
    eye.x = 0; eye.y = 0; eye.z = defaultDistance; // Z here will act like the default distance from target
    des.x = 0; des.y = 0; des.z = 0;
    up.x  = 0; up.y  = 1; up.z  = 0;

    // The increment by which the camera will move
    step = 0.5;

    // The calculated distance between the camera's position (eye) and the point at which the camera is looking (des)
    distance = sqrt(pow(eye.x-des.x,2)+pow(eye.y-des.y,2)+pow(eye.z-des.z,2));

    // The camera's rotation angle
    rotAngle.x = 0;
    rotAngle.y = 0;
}

// Makes the camera look at the supplied vec3 position
void _camera::lookAt(vec3 pos)
{
    // Update the point at which the camera is looking to the passed vec3 position
    des.x = pos.x;
    des.y = pos.y;
    des.z = pos.z;
}

// Sets up the camera's view matrix
void _camera::setUpCamera()
{
    gluLookAt(eye.x,eye.y,eye.z,
              des.x,des.y,des.z,
              up.x, up.y, up.z);
}

// Modifies the camera's eye position based on the point the camera is looking at so the camera appears to "orbit" around the point it looks at
void _camera::rotate()
{
    eye.x = des.x + distance*cos(rotAngle.y*PI/180.0)*sin(rotAngle.x*PI/180.0);
    eye.y = des.y + distance*sin(rotAngle.y*PI/180.0);
    eye.z = des.z + distance*cos(rotAngle.y*PI/180.0)*cos(rotAngle.x*PI/180.0);
}

// Moves the camera forward or backward - Direction is determined by sign of `dir` argument (+: Forward, -:Backward)
void _camera::moveFB(int dir)
{
    // if forward dir =1 else dir =-1
    eye.z += step*dir;
    des.z += step*dir;
}

// Moves the camera left or right - Direction is determined by sign of `dir` argument (+: Right, -:Left)
void _camera::moveLR(int dir)
{
    // Sign of dir determines whether camera moves left or right.
    eye.x += step*dir;
    des.x += step*dir;
}

// Car Level compatibility methods
void _camera::camInit()
{
    eye.x = 0.0; eye.y = 1; eye.z = 2;
    des.x = 0.1; des.y = -3; des.z = -30;
    up.x  = 0; up.y  = 2; up.z  = 0;

    step = 0.5;

    distance = sqrt(pow(eye.x-des.x,2)+pow(eye.y-des.y,2)+pow(eye.z-des.z,2));

    rotAngle.x = 0;
    rotAngle.y = 0;
}

void _camera::camInitFPS()
{
    eye.x = 0; eye.y = 0; eye.z = 0;
    up.x  = 0; up.y  = 1; up.z  = 0;
    step = 0.05f; // Reduced movement speed for better control

    minX = -50.0f;
    maxX =  100.0f;
    minZ = -100.0f;
    maxZ =  100.0f;

    // Set default rotation angles
    rotAngle.x = 180.0; // Yaw (left/right). 180 looks down -Z axis.
    rotAngle.y = 0.0;   // Pitch (up/down). 0 is level.

    des.x = eye.x + cos(rotAngle.y * PI/180.0) * sin(rotAngle.x * PI/180.0);
    des.y = eye.y + sin(rotAngle.y * PI/180.0);
    des.z = eye.z + cos(rotAngle.y * PI/180.0) * cos(rotAngle.x * PI/180.0);
}
