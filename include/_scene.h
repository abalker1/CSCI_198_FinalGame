#ifndef _SCENE_H
#define _SCENE_H

#include <_common.h>

#include <_light.h>
#include <_texture.h>
#include <_skybox.h>
#include <_camera.h>
#include <_model.h>
#include <_player.h>
#include <_fallAssignmentMgr.h>
#include <_collision.h>
#include <_parallax.h>
#include <_platform.h>
#include <_particles.h>

class _scene
{
    public:
        _scene();
        virtual ~_scene();

        /*+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=*
         *                                                                                                            *
         * If you want your scene to work in the game it needs to implement behavior the first four virtual functions *
         * If you want your game to have controls, implement behavior for any of the virtual input handling functions *
         *                                                                                                            *
         *+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=*
        */

        // Put children of the scene here (or don't, I guess, I'm not your mom)
        // Note: You can also initialize them in your scene's init function, but you still have to declare them here
        // ==========================

        // Available modules: _camera, _collision, _light, _model, _parallax, _platform, _player, _skybox, _texture, _particles

        // ==========================

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

        void setSoundFunctions(function<void(const char*)> playM,
                               function<void(const char*)> playS,
                               function<void(const char*)> playD,
                               function<void(const char*)> stopS);  // Set the scene's sound functions so that it can play sounds through the game's sound engine
        function<void(const char*)> playMusic;                      // Function to play music
        function<void(const char*)> playSound;                      // Function to play sound
        function<void(const char*)> playDupeS;
        function<void(const char*)> stopSound;                      // Function to stop sound
        void setKeyGetter(function<array<bool, 256>()> getter);     // Set the function to retrieve the keyState
        function<array<bool, 256>()> getKeys;                       // Function to retrieve the global keyState from the game

        void renderBitmapCharacters(float x, float y, col4 rgba, void* font, const string& word);   // Render a string of bitmap characters to the screen
        void renderBox(float x, float y, float bw, float bh, col4 rgba);                            // Render a flat 2D box to the screen

        float w;            // Width of the scene (fed from game)
        float h;            // Height of the scene (fed from game)
        bool shouldWin;     // Flag to track whether the scene should enter a win state
        bool shouldLose;    // Flag to track whether the scene should enter a lose state
        bool initialized;   // Flag to track the initialization of the scene

        // You can add whatever level-specific functions you want or need for your level too

    protected:

    private:
};

#endif // _SCENE_H
