#ifndef _CAMERA_H
#define _CAMERA_H

#include <_common.h>

class _camera
{
    public:
        _camera();
        virtual ~_camera();

        vec3 eye;  // Camera's eye position
        vec3 des;  // Position at which the camera is looking
        vec3 up;   // Camera's head orientation

        float step;                 // Camera's movement speed (when not attached to some other object)
        float sensitivity = 0.25;   // Rotational sensitivity of the camera
        vec2 rotAngle;              // rotation of the camera [left/right/up/down]
        float distance;             // Distance from camera's eye position to the point at which it is looking
        void reset();               // Reset the camera to original settings
        void rotate();              // Rotate the camera in an orbital fashion
        void moveFB(int dir);       // Move the camera forward and backward
        void moveLR(int dir);       // Move the camera left and right
        void setUpCamera();         // Set the camera's view matrix using gluLookat

        // Stuff below this is for the custom player control in third-person levels
        float defaultDistance = 45.0;   // Default distance camera should be at from target
        void lookAt(vec3 pos);          // Point camera at position (I'm not too sure this even does anything)

        // Car level compatibility methods
        void camInit();     // Initialize third-person camera
        void camInitFPS();  // Initialize first-person camera
        float minX, maxX;   // X-axis bounds for first-person camera
        float minZ, maxZ;   // Y-axis bounds for first-person camera

    protected:

    private:
};

#endif // _CAMERA_H
