#ifndef _PLAYER_H
#define _PLAYER_H

#include <_model.h>

// This is honestly just for future use (for the final probably) but I wanted to prototype things here. So this really isn't anything other than an alias right now.


class _player : public _model
{
    public:
        _player(float x, float y, float z);
        virtual ~_player();

        vec3 playerHalfDimensions{3.0f, 5.0f, 3.0f}; // Vector used for calculating player collisions

        bool grounded = true;               // Is the player on the ground
        bool jumpHeld = false;              // Is the jump key being held
        float vy = 0.0f;                    // Player's upward velocity
        float groundY = 0.0f;
        float jumpSpeed = 27.0f;            // Player's jump 'speed' (or power);
        float gravity = 30.f;               // Default gravity
        float gravityHeldMult = 0.75f;      // Gravity multiplier when jump key is held
        float gravityCutMult = 1.8f;        // Gravity multiplier when jump key is released
        float gravityFallMult = 1.35f;      // Gravity multiplier during fall
        float terminalVelocity = 30.0f;     // Max player fall speed
        bool jump();                        // Function to initiate the jump
        void step(float dt);                // Function to update the player's position through the jump

        float speed = 27.0f;                // Player's movement speed
        float sprintMultiplier = 2.0f;      // Multiplier for speed when sprinting
        float facingAngle = 0.0f;           // Direction the player is facing

        bool playingOneShot = false;                            // Whether or not a one shot animation is currently playing
        Action activeOneShot = ACT_STAND;                       // What one shot animation is currently playing
        Action resumeAction = ACT_STAND;                        // What action the player should go back to when the one shot animation ends
        void playAnim(Action anim, Action resume = ACT_STAND);  // Not fully implemented stuff, but should allow for the player to act out a single action one time

        // Modified animation and rendering functions for better animation frame interpolation and custom action dependent behavior
        virtual void renderFrameItpWithGLCmds (int n, float interp, const struct md2_model_t *mdl) override;
        virtual void animate(int start, int end, int& frame, float& interp) override;

    protected:

    private:
};

#endif // _PLAYER_H
