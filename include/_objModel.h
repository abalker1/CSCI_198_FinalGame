#ifndef _OBJMODEL_H
#define _OBJMODEL_H

#include <_common.h>
#include <objLoader.h>

class _objModel
{
    public:
        _objModel();
        virtual ~_objModel();

        float targetTilt;              // The Z-angle we want to tilt towards.
        float currentTilt;             // The Z-angle for the current frame.
        float horizontalPos;           // The current X position on the road.
        float targetHorizontalPos;     // The X position we want to move towards.
        float movementSpeed;           // How fast the bike moves left and right.
        float tiltSpeed;               // How fast the bike tilts.

        void update();                 // This will run every frame to smooth animations.

        void loadMyOBJ(const char*);
        double rotateX;
        double rotateY;
        double rotateZ;

        double posX;
        double posY;
        double posZ;

        double scale;

        void drawModel();

    protected:

    private:
        OBJModel myObj;
};

#endif // _OBJMODEL_H

