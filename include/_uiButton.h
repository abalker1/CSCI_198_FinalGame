#ifndef _UIBUTTON_H
#define _UIBUTTON_H

#include <_uiObject.h>
#include <_common.h>


class _uiButton : public _uiObject
{
    public:
        _uiButton();
        virtual ~_uiButton();

        virtual bool isButton() override {return true;} // This IS a button

        // Initialization of a button requires more than just a texture path, we also need its action and the hotkey for it so we'll have to do something else
        void setButton(function<void()> action, char hotkey);
        function<void()> buttonFunc;
        char hotkeyChar;

        // Check if the button is being hovered over or if it has been pressed
        bool selectTest(float msX, float msY);
        bool hitTest(float msX, float msY);

    protected:

    private:
};

#endif // _UIBUTTON_H
