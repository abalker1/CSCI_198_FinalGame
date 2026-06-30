#ifndef _UIPAGE_H
#define _UIPAGE_H

#include <_common.h>
#include <_uiObject.h>
#include <_uiButton.h>
#include <_uiText.h>

class _uiPage
{
    public:
        _uiPage();
        virtual ~_uiPage();

        vector<_uiObject*> objects;

        void clear();               // Wipes object vector and deletes all UI objects within
        void add(_uiObject* obj);   // Adds a UI object to the objects vector

        void setVisible(bool visible);          // Toggle visibility of all elements of the UI page
        void draw(float width, float height);   // Draw the UI

        vector<_uiButton*> getHitButtons(float x, float y); // Get all buttons that hit with these coordinates
        vector<_uiButton*> getHitButtons(WPARAM wParam); // Get all buttons hit with this hotkey

        bool overlay = false;   // Should the page be drawn with a semi-transparent backdrop
        bool active = false;    // Should the page be considered active for purposes of detecting interaction
        bool visible = false;   // Whether or not the page itself should be visible


        // These two are just to cut down on a TON of repetitive code lol
        void addObject(const char* objTexture); // Add a new object to the page
        void addButton(const char* btnTexture, function<void()> action, char hotkey); // Add a new button to the page


    protected:

    private:
};

#endif // _UIPAGE_H
