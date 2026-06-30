#ifndef _GAME_H
#define _GAME_H

#include <_common.h>
#include <_level0.h>
#include <_level1.h>
#include <_level2.h>
#include <_scene.h>
#include <_sound.h>
#include <_uiManager.h>


// This class will act as our scene manager and will handle rendering the game and the UI separately
// I started to get really sick of how EVERYTHING was controlled by scene, so this should help to keep scenes lighter weight and more focused

class _game
{
    public:
        _game();
        virtual ~_game();

        _uiManager *ui = nullptr;       // The UI manager
        _sound *sound = nullptr;        // The sound engine
        _scene* activeScene = nullptr;  // Which scene is currently active
        vector<_scene*> scenes;         // Container of all available scenes
        bool paused;                    // Whether or not the game is paused
        bool initialized;


        float width;            // Width of window
        float height;           // Height of window
        double msX;             // Mouse X pos
        double msY;             // Mouse Y pos
        double msZ;             // Mouse Z pos
        float aspectRatio;      // Window aspect ratio

        void init();            // Initialize the game
        void initActiveScene(); // Initialize the active scene

        HWND windowHandle;          // The current window handle
        bool cursorLocked;          // Flag to track whether or not the cursor is currently locked to the window
        void lockCursor(HWND hWnd); // Function to lock the cursor to the window
        void unlockCursor();        // Function to unlock the cursor
        void updateCursorClip();    // Function to update the rectangle the cursor is clipped to
        void centerCursor();        // Function to center the cursor within the current window

        void render();                                                  // Render the active scene
        void resize(int w, int h);                                      // Resize window
        void mouseMapping(int x, int y);                                // Map mouse coordinates
        int winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // Get kbm input

        // Handle input
        array<bool, 256> keyStates; // Tracks whether or not each key is held so we can have better input handling that does not rely on windows' key repeating behavior (provided I implement it, I might just not lol)
        float prev_msX;             // The mouse's previous X position
        float prev_msY;             // The mouse's previous Y position

        int level;                  // Integer value representing the current level
        void advanceLevel();        // Function to move on to the next level
        bool playedSound;           // Whether or not the level up sound has been played
        bool firstLevelStarted;     // Hacky solution to a bug with the first level
        void startFirstLevel();     // Same as above lol

        bool debug;                                         // Whether or not debug behavior should be observed
        void dumpDebug(string trigger, WPARAM wParam=0);    // Function to print debug information to console

    protected:

    private:
};

#endif // _GAME_H
