#include "_scene.h"

_scene::_scene()
{
    initialized = false;    // Scenes start uninitialized
    shouldWin = false;      // Make sure the game doesn't instantly decide it needs to win
    shouldLose = false;     // Make sure the game doesn't instantly decide it needs to lose
}

_scene::~_scene()
{
    //dtor
}

void _scene::init()
{
    // We do this here instead of the constructor because this all requires a valid OpenGL context. That means we can't do it in the constructor (which is invoked right at the start of main.cpp).
    // We could move the invocation of the constructor to allow it to all be kept in the constructor, but this is a more robust solution.
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Save current matrix state and reset to identity to ensure
    // lights and other objects are initialized consistently
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // We can initialize any children here (light, for example), since initGL will only be invoked post OpenGL context creation. This should mean they can have their initializations in their constructors.
    // However, we also don't want to initialize them in the header anymore. Define them as unbound variables and bind them here.
    // ==========================

    // This is where you'd initialize stuff like _light or _camera or whatever

    // ==========================
    // Restore matrix state
    glPopMatrix();

    // Set initialized to true at the end so the game knows the level is good to go
    initialized = true;
}

void _scene::reset()
{
    // Any and all member variables that are updated throughout your scene should be reset to their default values here
    // This includes any member variables of the scene's children like the camera or player as well
    return;
}

void _scene::update(float dt, bool paused)
{
    // Implement whatever updating logic you want here (movement and things like that)
    return;
}

void _scene::draw(float dt, bool paused)
{
    // Implement whatever draw logic you want
    // Technically you could do all of the updating logic here too, but update will be called by the game before draw is called
    return;
}

void _scene::onKeyDown(WPARAM wParam)
{
    // Add implementation if needed
    return;
}

void _scene::onKeyUp(WPARAM wParam)
{
    // Add implementation if needed
    return;
}

void _scene::onLMouseDown(float x, float y)
{
    // Add implementation if needed
    return;
}

void _scene::onRMouseDown(float x, float y)
{
    // Add implementation if needed
    return;
}

void _scene::onMMouseDown(float x, float y)
{
    // Add implementation if needed
    return;
}

void _scene::onLMouseUp(float x, float y)
{
    // Add implementation if needed
    return;
}

void _scene::onRMouseUp(float x, float y)
{
    // Add implementation if needed
    return;
}

void _scene::onMMouseUp(float x, float y)
{
    // Add implementation if needed
    return;
}

void _scene::onMouseMove(float dx, float dy)
{
    // Add implementation if needed
    return;
}

void _scene::onMouseWheel(float delta)
{
    // Add implementation if needed
    return;
}

// Sets up the callback function for retrieving the game's keyState
void _scene::setKeyGetter(function<array<bool, 256>()> getter)
{
    getKeys = getter;
}

// Sets up the callback functions to allow the scene to play sound through the game's sound engine
void _scene::setSoundFunctions(function<void(const char*)>playM, function<void(const char*)>playS, function<void(const char*)>playD, function<void(const char*)>stopS)
{
    playMusic = playM;
    playSound = playS;
    playDupeS = playD;
    stopSound = stopS;
}

void _scene::renderBitmapCharacters(float x, float y, col4 rgba, void *font, const string& word)
{
    // Begin use of orthographic rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, static_cast<GLdouble>(w), static_cast<GLdouble>(h), 0.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // Render all the characters in the string to the screen in the specified color
    glColor4f(rgba.r, rgba.g, rgba.b, rgba.a);
    glRasterPos2f(x, y);
    for (auto& c : word) {
        glutBitmapCharacter(font, c);
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

void _scene::renderBox(float x, float y, float bw, float bh, col4 rgba)
{
    // Begin use of orthographic rendering
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, static_cast<GLdouble>(w), static_cast<GLdouble>(h), 0.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // Draw a box centered at the specified coordinates with the given dimensions and color
    glDisable(GL_TEXTURE_2D);
    glColor4f(rgba.r, rgba.g, rgba.b, rgba.a);
    glBegin(GL_QUADS);
        glVertex2f(x - 0.5f * bw, y - 0.5f * bh);  // Top Left
        glVertex2f(x + 0.5f * bw, y - 0.5f * bh);  // Top Right
        glVertex2f(x + 0.5f * bw, y + 0.5f * bh);  // Bottom Right
        glVertex2f(x - 0.5f * bw, y + 0.5f * bh);  // Bottom Left
    glEnd();
    glEnable(GL_TEXTURE_2D);

    // End use of orthographic rendering
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
