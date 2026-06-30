#include "_uiPage.h"

_uiPage::_uiPage()
{
    //ctor
}

_uiPage::~_uiPage()
{
    clear();
}

void _uiPage::clear()
{
    for (auto *obj : objects) {
        if (obj != nullptr) {
            delete obj;
        }
    }
    objects.clear();
}

// Add general UI object to objects vector
void _uiPage::add(_uiObject* obj)
{
    if (obj != nullptr) {
        objects.push_back(obj);
    }
}

// Sets the visibility of all elements in the manager
void _uiPage::setVisible(bool val)
{
    visible = val;
    for(auto *obj : objects) {
        if (obj != nullptr) {
            obj->visible = val;
        }
    }

}

void _uiPage::draw(float width, float height)
{
    // Don't draw an invisible UI manager
    if (!visible) {
        return;
    }

    // Begin use of orthographic rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, static_cast<GLdouble>(width), static_cast<GLdouble>(height), 0.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    if (overlay) {
    // Draw transparent overlay
    glDisable(GL_TEXTURE_2D);
    glColor4f(0.0, 0.0, 0.0, 0.55);
    glBegin(GL_QUADS);
        glVertex2f(0.0, 0.0);
        glVertex2f(width, 0.0);
        glVertex2f(width, height);
        glVertex2f(0.0, height);
    glEnd();
    glEnable(GL_TEXTURE_2D);
    }

    // Draw all the elements in the UI objects vector
    for(auto *obj : objects) {
        if (obj != nullptr) {
            obj->draw();
        }
    }

    // End use of orthographic rendering
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

vector<_uiButton*> _uiPage::getHitButtons(float x, float y)
{
    vector<_uiButton*> out;

    for (auto *obj : objects) {
        // Don't try anything with nullptrs
        if (obj == nullptr) {
            continue;
        }

        // Yay, polymorphism!
        _uiButton* button = dynamic_cast<_uiButton*>(obj);

        // If the cast succeeds we can safely call hitTest or check hotkey chars
        if (button && button->hitTest(x, y)) {
            out.push_back(button);
        }
    }

    return out;
}

vector<_uiButton*> _uiPage::getHitButtons(WPARAM wParam)
{
    vector<_uiButton*> out;

    for (auto *obj : objects) {
        // Don't try anything with nullptrs
        if (obj == nullptr) {
            continue;
        }

        // Yay, polymorphism!
        _uiButton* button = dynamic_cast<_uiButton*>(obj);
        int keyDown = toupper(static_cast<unsigned char>(wParam));

        // If the cast succeeds we can safely call hitTest or check hotkey chars
        if (button && keyDown == button->hotkeyChar) {
            out.push_back(button);
        }
    }

    return out;
}

void _uiPage::addObject(const char* objTexture)
{
    auto newObject = new _uiObject(); // Create a new object
    newObject->init(objTexture); // Initialize the object with the specified texture
    add(newObject);
}

void _uiPage::addButton(const char* btnTexture, function<void()> action, char hotkey)
{
    auto newButton = new _uiButton(); // Create a new button
    newButton->init(btnTexture); // Initialize the button with the specified texture
    newButton->setButton(action, toupper(hotkey)); // Assign button specific behavior
    add(newButton);
}
