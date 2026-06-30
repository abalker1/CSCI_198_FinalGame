#ifndef _LEVEL1_H
#define _LEVEL1_H

#include <_common.h>
#include <_scene.h>

class _level1 : public _scene
{
    public:
        _level1();
        virtual ~_level1();

        _light *light = nullptr;
        _camera *cam = nullptr;
        _collision *collision = nullptr;
        _skybox *skybox = nullptr;
        _player *ply = nullptr;           // Main player model, probably shouldn't remove this but you do you, I guess.
        _particles *particles = nullptr;

        virtual void init();                        // Initialize the scene
        virtual void reset();                       // Reset the scene to its default state
        virtual void update(float dt, bool paused); // Update the scene (handle things like movement or physics)
        virtual void draw(float dt, bool paused);   // Draw the scene and everything within it

        virtual void onKeyDown(WPARAM wParam);          // Handle key presses
        virtual void onKeyUp(WPARAM wParam);            // Handle key releases
        virtual void onLMouseDown(float x, float y);    // Handle left mouse presses
        virtual void onRMouseDown(float x, float y);    // Handle right mouse presses
        virtual void onMMouseDown(float x, float y);    // Handle middle mouse presses
        virtual void onLMouseUp(float x, float y);      // Handle left mouse releases
        virtual void onRMouseUp(float x, float y);      // Handle right mouse releases
        virtual void onMMouseUp(float x, float y);      // Handle middle mouse releases
        virtual void onMouseMove(float dx, float dy);   // Handle mouse movements
        virtual void onMouseWheel(float delta);         // Handle mouse wheel

        float timeRemaining = 90.0f;
        float penMin = 3.0f;
        float penMax = 5.0f;
        vec3 playerStartPos = {0.0f, 8.0f, 0.0f}; // 0 8 0
        float maxX = 500;
        float maxY = 500;
        float maxZ = 500;

        bool goalReached = false;
        void drawBox(float x, float y, float z);
        _texture *boxTex = nullptr;

        _texture *platformTexture = nullptr;
        vector<_platform*> platforms; // Static platforms (or boxes, I should have just called them boxes) that can act like collision objects
        bool resolvePlayerPlatformContact(float snapTo = 0.25f);


    protected:

    private:
};

#endif // _LEVEL1_H
