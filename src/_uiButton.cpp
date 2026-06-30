#include "_uiButton.h"

_uiButton::_uiButton()
{
    //ctor
    hotkeyChar = 0;
}

_uiButton::~_uiButton()
{
    //dtor
}

void _uiButton::setButton(function<void()> action, char hotkey)
{
    buttonFunc = action;
    hotkeyChar = hotkey;
}

bool _uiButton::selectTest(float msX, float msY)
{
    //TODO
}

bool _uiButton::hitTest(float msX, float msY)
{
    // If the button is not visible, don't report hits
    if (!visible) {
        return false;
    }

    // Calculate half lengths for checking coordinate overlap between mouse and the button
    float halfW = w * 0.5f;
    float halfH = h * 0.5f;

    // Return true if the mouse is within the button's frame
    return (msX >= (cx - halfW) && msX <= (cx + halfW) &&
            msY >= (cy - halfH) && msY <= (cy + halfH));
}
