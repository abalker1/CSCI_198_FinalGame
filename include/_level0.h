#ifndef _LEVEL0_H
#define _LEVEL0_H

#include <_common.h>
#include <_scene.h>
#include <_inputs.h>
#include <_timer.h>
#include <objLoader.h>
#include <_objModel.h>

class _level0 : public _scene
{
    public:
        _level0();
        virtual ~_level0();

        _light *light = nullptr;
        _camera *cam = nullptr;
        _collision *collision = nullptr;
        _parallax *parallax = nullptr;
        _skybox *skybox = nullptr;
        _objModel *myModel = nullptr;
        _texture *myTexture = nullptr;
        _texture *parkedTexture1 = nullptr;
        _texture *parkedTexture2 = nullptr;
        _parallax *myPrlx = nullptr;  // Clouds parallax (letssee)
        _parallax *roadPrlx = nullptr;
        _parallax *backgroundPrlx = nullptr;  // Background fence parallax (replaces skybox)
        _parallax *sky = nullptr;  // Background sky parallax
        _parallax *timerShow = nullptr;
        _inputs *myInput = nullptr;
        _timer *gameTimer = nullptr;

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

        void checkParking();                      // check if player can park in a spot
        void drawVisualParkedCars();  // Draw the looping visual parked cars
        void updateParkingAnimation();
        void startParkingAnimation(visualParkedCar* car, float targetZ);

        OBJModel parkingSpotModel;
        std::vector<visualParkedCar> visualCars;  // Visual parked cars (for display, loops with parallax)
        bool isParked = false;  // track if player has successfully parked

        bool isParkingAnimationActive = false;
        bool shouldRenderPlayerCar = true;
        int parkingAnimStage = 0;
        float parkingAnimTimer = 0.0f;
        float parkingAnimDuration = 1.25f;
        float parkingAnimPositionStep = 2.0f;
        float parkingAnimRotationStep = 3.0f;
        float parkingAnimTargetRotation = -90.0f;
        float parkingAnimStartRotationZ = 0.0f;
        vec3 parkingAnimStartPos {0.0f, 0.0f, 0.0f};
        vec3 parkingAnimEndPos {0.0f, 0.0f, 0.0f};
        visualParkedCar* parkingAnimTargetCar = nullptr;
        float parkingAnimTargetZ = 0.0f;

        // --------------For Parked Cars----------------
        float zStart = -30.0f;  // Far Z position
        float zEnd = 10.0f;      // Close Z position
        float zRange = zEnd - zStart; // Range = 30

        // --------------For First Person View----------------
        bool isFirstPersonView = false;
        void toggleFirstPersonView();
        void updateFirstPersonCamera();

        // --------------For Game Timer and Win/Lose States----------------
        enum GameState { IDLE, PLAYING, WON, LOST };
        GameState currentGameState = IDLE;
        float timeLimit = 10.0f; // 10 seconds to park
        void checkTimer();
        void restartLevel();

        // Background parallax zoom effect
        float backgroundZoom = 0.4f;
        float maxZoom = 1.6f;  // Maximum zoom when at full speed


    protected:

    private:
};

#endif // _LEVEL0_H

