#ifndef _UIMANAGER_H
#define _UIMANAGER_H

// I had to make this one to solve an issue with C++ being terrible and awful and whiny
// If I didn't love C++ so much I would have just done something even more stupid than this and ran with it, but here we are

#include <_common.h>
#include <_uiPage.h>

class _uiManager
{
    public:
        _uiManager();
        virtual ~_uiManager();

        void setCursorCallbacks(function<void()> lock, function<void()> unlock);    // Set the functions used to give cursor control to the ui
        function<void()> requestLockCursor;     // Function to let the ui lock the cursor
        function<void()> requestUnlockCursor;   // Function to let the ui unlock the cursor

        void resetFirstLevelHack(function<void()> reset);   // Hacky solution to a bug, if this is still here it means we didn't figure out a good way to fix the bug
        function<void()> resetFirstLevel;                   // Yeah <3

        array<const char*, 3> helpPages;// Filepaths to help page textures, this is a kind of hacky work around but it's probably the simplest way to achieve the desired behavior
        int helpIndex;                  // Current index for the help screen texture

        vector<_uiPage*> uiStack;       // Collection to hold all managers
        bool enabled;                   // Whether or not the UI is enabled
        bool initialized;               // Track initialization of UI
        bool open;                      // Whether or not the UI is open
        bool resetRequested;            // Whether or not the user has requested a reset through the UI
        bool advanceRequested;

        _uiPage *landingPage = new _uiPage();            // Store UI objects for the landing page
        _uiPage *menuPage = new _uiPage();               // Store UI objects for the menu page
        _uiPage *helpPage = new _uiPage();               // Store UI objects for the help page
        _uiPage *pausePage = new _uiPage();              // Store UI objects for the pause page
        _uiPage *winPage = new _uiPage();                // Store UI objects for the win page
        _uiPage *losePage = new _uiPage();               // Store UI objects for the lose page
        _uiPage *creditsPage = new _uiPage();            // Store UI objects for the credits page

        void init(float w, float h);    // Initialization of all UI objects
        void layout(float w, float h);  // Set the layout of UI objects
        void reset();                   // Reset state of all UI components
        void draw(float w, float h);    // Draw the UI

        void processButtons(WPARAM w);          // Process buttons corresponding to wParam (key press)
        void processButtons(float x, float y);  // Process buttons corresponding to x and y (mouse click)
        void openLanding();     // Open the landing menu
        void openMenu();        // Open the main menu
        void openHelp();        // Open the help page
        void nextHelp();        // Move to the next help page
        void prevHelp();        // Move to the previous help page
        void openCredits();     // Open the credits page
        void startGame();       // Start the game
        void pauseGame();       // Pause the game
        void resumeGame();      // Resume the game
        void quitGame();        // Quit the game
        void resetGame();       // Reset the game
        void winGame();         // Open the win panel
        void loseGame();        // Open the lose panel

    protected:

    private:
};

#endif // _UIMANAGER_H
