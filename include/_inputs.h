#ifndef _INPUTS_H
#define _INPUTS_H

#include <_common.h>
#include <_parallax.h>

// Forward declaration
class _objModel;

class _inputs
{
    public:
        _inputs();
        virtual ~_inputs();

        //flag variables
        bool isUpPressed;
        bool isDownPressed;
        bool isLeftPressed;
        bool isRightPressed;
        bool isSpacePressed;

        //speed variables
        float currentSpeed;
        float maxSpeed;
        float acceleration;
        float deceleration;

        void update( _objModel* mdl, _parallax* prlx, _parallax* prlx1);

        void keyPressed(_objModel *); // key pressed on keyboard
        void keyPressed(_parallax *); // key pressed on keyboard
        void keyUp();              // key released on keyboard

        WPARAM wParam;
        LPARAM lParam;

        float totalRoadOffsetZ;
    protected:

    private:
};

#endif // _INPUTS_H

