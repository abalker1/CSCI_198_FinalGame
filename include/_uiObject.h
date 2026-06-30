#ifndef _UIOBJECT_H
#define _UIOBJECT_H

#include <_common.h>
#include <_texture.h>

/*
This is the generalized UI object class. Since UI objects can have a lot of different functionality (it could be a button or some other object with specialized functionality),
it makes sense to have a parent class like this that holds or prototypes much of the shared behavior that all UI elements will have in common.
Any specialized UI elements should inherit from this class, like buttons will, but it can be used standalone to render 'panels', which will just be functionless UI objects.
*/

class _uiObject
{
    public:
        _uiObject();
        virtual ~_uiObject();

        _texture *tex = nullptr;// Texture for UI object
        float cx, cy;           // Center coordinates for UI object
        float w, h;             // Width and height of UI object
        bool visible;           // Whether or not the UI object is currently visible

        virtual bool isButton() {return false;} // This is not a button

        void setFrame(float centerX, float centerY, float width, float height); // Set the 'frame' of the UI object
        float getAspectRatio();                                                 // Get the aspect ratio of the UI object for proper resizing
        void init(const char* texturePath);                                     // Initialize the UI object

        virtual void draw();                                                    // Draw the UI object

    protected:

    private:
};

#endif // _UIOBJECT_H
