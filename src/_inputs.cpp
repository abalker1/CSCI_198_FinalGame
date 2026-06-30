#include "_inputs.h"
#include "_objModel.h"

_inputs::_inputs()
{
    //ctor
    isUpPressed = false;
    isDownPressed = false;
    isLeftPressed = false;
    isRightPressed = false;

    isSpacePressed = false;
    totalRoadOffsetZ = 0.0f;

    currentSpeed = 0.0f;
    maxSpeed = 0.03f;       // original scroll speed
    acceleration = 0.005f;  // How fast to speed up (tweak this)
    deceleration = 0.0016f;  // How fast to slow down (tweak this)
}

_inputs::~_inputs()
{
    //dtor
}



void _inputs::keyPressed(_objModel* mdl)
{
     switch(wParam)
     {
        case VK_UP:    isUpPressed = true; break;
        case 87:    isUpPressed = true; break;
        case VK_DOWN:  isDownPressed = true; break;
        case 83:  isDownPressed = true; break;
        case VK_LEFT:  isLeftPressed = true; break;
        case 65:  isLeftPressed = true; break;
        case VK_RIGHT: isRightPressed = true; break;
        case 68: isRightPressed = true; break;
        case VK_SPACE: isSpacePressed = true; break;
     }
}

void _inputs::keyPressed(_parallax* prlx)
{
  // implemented elsewhere
}

void _inputs::keyUp()
{
    switch(wParam)
    {
        case VK_UP:    isUpPressed = false; break;
        case 87:    isUpPressed = false; break;
        case VK_DOWN:  isDownPressed = false; break;
        case 83:  isDownPressed = false; break;
        case VK_LEFT:  isLeftPressed = false; break;
        case 65:  isLeftPressed = false; break;
        case VK_RIGHT: isRightPressed = false; break;
        case 68: isRightPressed = false; break;
        case VK_SPACE: isSpacePressed = false; break;
    }
}

// This function checks the flags and tells the model what to do.
void _inputs::update(_objModel* mdl, _parallax* prlx, _parallax* prlx2)
{
    float maxHorizontalPosLeft = 8; // How far left from the center the bike can go.
    float maxHorizontalPosRight = 4; // How far right from the center the bike can go.

    if(isUpPressed) {
        currentSpeed+=acceleration;
        if (currentSpeed>maxSpeed) {
            currentSpeed = maxSpeed;
        }
    }
    else {
        currentSpeed-=deceleration;
        if(currentSpeed<0.0) {
            currentSpeed=0;
        }
    }

    if (currentSpeed>0.0) {
        prlx->scroll("up", currentSpeed, 0.03); //prlx->scroll("up", currentSpeed*0.2);
        prlx2->scroll("down", currentSpeed, 0.03); //prlx2->scroll("down", currentSpeed*0.2);
        totalRoadOffsetZ += currentSpeed * 10.0f;
    }
    else if (currentSpeed < 0.0) { // Should not happen with current logic, but for reverse
         prlx->scroll("down", -currentSpeed, 0.03); //prlx->scroll("down", -currentSpeed*0.2);
         prlx2->scroll("up", -currentSpeed, 0.03); //prlx2->scroll("up", -currentSpeed*0.2);
         totalRoadOffsetZ += currentSpeed * 10.0f;
    }

    // Smooth horizontal movement - allow gradual movement instead of snapping to extremes
    float horizontalMoveSpeed = 0.15f; // How fast to move horizontally per frame

    if (isLeftPressed) {
        // Gradually move left
        mdl->targetHorizontalPos -= horizontalMoveSpeed;
        // Clamp to maximum left position
        if (mdl->targetHorizontalPos < -maxHorizontalPosLeft) {
            mdl->targetHorizontalPos = -maxHorizontalPosLeft;
        }
    } else if (isRightPressed) {
        // Gradually move right
        mdl->targetHorizontalPos += horizontalMoveSpeed;
        // Clamp to maximum right position
        if (mdl->targetHorizontalPos > maxHorizontalPosRight) {
            mdl->targetHorizontalPos = maxHorizontalPosRight;
        }
    }
    // No else block - position stays where it is when keys are released
}


