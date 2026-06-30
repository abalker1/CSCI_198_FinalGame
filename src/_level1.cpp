#include "_level1.h"

_level1::_level1()
{
    //ctor
}

_level1::~_level1()
{
    delete light;
    delete cam;
    delete collision;
    delete skybox;
    delete ply;
    delete platformTexture;
    delete boxTex;
    for (_platform* p : platforms) {
        delete p;
    }
    delete particles;
}

void _level1::init()
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
    // lights and other objects are initialized consistently (in Eye Space)
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // We can initialize any children here (light, for example), since initGL will only be invoked post OpenGL context creation. This should mean they can have their initializations in their constructors.
    // However, we also don't want to initialize them in the header anymore. Define them as unbound variables and bind them here.
    // ==========================

    this->light     = new _light(GL_LIGHT0);
    this->cam       = new _camera();
    this->collision = new _collision();
    this->skybox    = new _skybox();
    this->ply       = new _player(playerStartPos.x, playerStartPos.y, playerStartPos.z);           // Main player model, probably shouldn't remove this but you do you, I guess.
    this->platformTexture = new _texture();
    this->particles = new _particles();
    this->boxTex    = new _texture();


    // ==========================
    // Restore matrix state
    glPopMatrix();

    // Initialize the general skybox
    skybox->skyboxInit();
    skybox->tex[0] = skybox->texture->load("images/platformLevel/skybox/negz.png");
    skybox->tex[1] = skybox->texture->load("images/platformLevel/skybox/posz.png");
    skybox->tex[2] = skybox->texture->load("images/platformLevel/skybox/posy.png");
    skybox->tex[3] = skybox->texture->load("images/platformLevel/skybox/negy.png");
    skybox->tex[4] = skybox->texture->load("images/platformLevel/skybox/posx.png");
    skybox->tex[5] = skybox->texture->load("images/platformLevel/skybox/negx.png");

    // Initialize the player
    ply->initModel("models/billgates/tris.md2", "models/billgates/billywork.jpeg");
    ply->playerHalfDimensions = {2.0f, 5.0f, 1.0f};

    // Load the box texture
    boxTex->load("images/platformLevel/COFFEE.png");

    // Add all the platforms
    platformTexture->load("images/assignmentLevel/textures/ground/laminate.jpg");
    platforms.clear();

    auto makeSPlatform = [](float x, float y, float z) { return new _platform(x, y, z, 20.0f, 3.0f, 20.0f, 2.0f, 2.0f); }; // The last 2 numbers are for texture repeating, tweak them if the texture looks too stretched
    auto makeMPlatform = [](float x, float y, float z) { return new _platform(x, y, z, 40.0f, 3.0f, 40.0f, 4.0f, 4.0f); };
    auto makeLPlatform = [](float x, float y, float z) { return new _platform(x, y, z, 80.0f, 3.0f, 80.0f, 8.0f, 8.0f); };

    platforms.push_back(makeLPlatform(0.0f, 0.0f, 0.0f));
    platforms.push_back(makeMPlatform(-100.0f, 10.0f, 0.0f));
    platforms.push_back(makeSPlatform(-100.0f, 15.0f, 75.0f));
    platforms.push_back(makeMPlatform(-100.0f, 20.0f, 150.0f));
    platforms.push_back(makeSPlatform(-10.0f, 20.0f, 150.0f));
    platforms.push_back(makeSPlatform(70.0f, 20.0f, 170.0f));
    platforms.push_back(makeSPlatform(150.0f, 20.0f, 150.0f));
    platforms.push_back(makeMPlatform(150.0f, -10.0f, 25.0f));
    platforms.push_back(makeSPlatform(160.0f, 0.0f, -25.0f));
    platforms.push_back(makeSPlatform(140.0f, 10.0f, -75.0f));
    platforms.push_back(makeSPlatform(160.0f, 20.0f, -125.0f));
    platforms.push_back(makeSPlatform(140.0f, 30.0f, -175.0f));
    platforms.push_back(makeMPlatform(150.0f, 40.0f, -245.0f));
    platforms.push_back(makeSPlatform(100.0f, 40.0f, -275.0f));
    platforms.push_back(makeSPlatform(60.0f, 50.0f, -295.0f));
    platforms.push_back(makeSPlatform(30.0f, 60.0f, -255.0f));
    platforms.push_back(makeSPlatform(0.0f, 70.0f, -215.0f));
    platforms.push_back(makeSPlatform(-30.0f, 60.0f, -175.0f));
    platforms.push_back(makeMPlatform(-60.0f, 70.0f, -105.0f));


    // Set camera distance from player to default
    cam->distance = cam->defaultDistance;

    // After everything necessary to the scene has been initialized, make sure to set initialized to true
    initialized = true;
}

void _level1::reset()
{
    // A reset function will look something like this

    timeRemaining = 90.0f;
    ply->pos = playerStartPos;
    shouldWin = false;
    shouldLose = false;
    goalReached = false;
    cam->reset();
    particles->clear();
}

void _level1::update(float dt, bool paused)
{
    // Grab the key states from game
    array<bool, 256> keyStates = getKeys();

    // Player stuff
    if (!paused)
    {
        timeRemaining -= dt;

        if (timeRemaining >= 0.0f && goalReached) {
            shouldWin = true;
        }
        else if (timeRemaining <= 0.0f || ply->pos.y < -50) {
            shouldLose = true;
        }

        // This is to fix the player sometimes falling through platforms lol
        const float maxStep = 1.0f / 120.0f;
        float remaining = dt;
        while (remaining > 0.0f) {
            float stepDt = min(remaining, maxStep);
            ply->step(stepDt);
            resolvePlayerPlatformContact(0.25f);
            remaining -= stepDt;
        }

        if (keyStates[' ']) {
            if (ply->grounded) {
                playSound("sounds/jump.mp3");
            }
            ply->jump();
        }
        ply->jumpHeld = keyStates[' '];

        // Movement logic
        if (keyStates['W'] || keyStates['A'] || keyStates['S'] || keyStates['D']) {

            // Get player angle in radians, initialize movement magnitudes
            float yawRad = (ply->facingAngle + 180.0f) * PI / 180.0f;

            float forwardX = -sin(yawRad);
            float forwardZ = -cos(yawRad);
            float rightX = cos(yawRad);
            float rightZ = -sin(yawRad);

            float moveX = 0.0f;
            float moveZ = 0.0f;

            if(keyStates['W']) {
                moveX += forwardX;
                moveZ += forwardZ;
            }

            if(keyStates['S']) {
                moveX -= forwardX;
                moveZ -= forwardZ;
            }

            if(keyStates['A']) {
                moveX -= rightX;
                moveZ -= rightZ;
            }

            if(keyStates['D']) {
                moveX += rightX;
                moveZ += rightZ;
            }

            // Normalize movement vector
            float len = sqrt (moveX * moveX + moveZ * moveZ);
            if (len > 0) {
                moveX /= len;
                moveZ /= len;

                // Allow the player to sprint
                float trueSpeed = ply->speed;
                if (keyStates[VK_SHIFT]) {
                    trueSpeed *= ply->sprintMultiplier;
                }

                ply->pos.x += moveX * trueSpeed * dt;
                ply->pos.z += moveZ * trueSpeed * dt;

                // Confine player position so they can't go out of bounds (this doesn't work super well because it's a third person game so the camera can actually still just clip out of bounds)
                ply->pos.x = clamp(ply->pos.x, -maxX, maxX);
                ply->pos.z = clamp(ply->pos.z, -maxZ, maxZ);

            }
            if (!ply->playingOneShot) {
                ply->actionTrigger = ply->ACT_RUN;
            }
        } else {
            if (!ply->playingOneShot) {
                ply->actionTrigger = ply->ACT_STAND;
            }
        }

        vec3 obsPos = {-60.0f, 75.0f, -105.0f};
        vec3 obsHlf = {2.5f, 2.5f, 2.5f};
        if (!goalReached && collision->isCubicCol(obsPos, ply->pos, obsHlf, ply->playerHalfDimensions)) {
            goalReached = true;
        }

        particles->update(dt);
    }

    // Camera stuff
    // Modify camera view, we want to do this last so that the camera view matrix doesn't "lag" behind the actual transforms of the scene
    float cameraYaw = ply->facingAngle + 180.0f;
    if (cameraYaw >= 360.0f) {
        cameraYaw -= 360.0f;
    }

    cam->rotAngle.x = cameraYaw;
    cam->lookAt({ply->pos.x, ply->pos.y + 5.5, ply->pos.z});
    cam->rotate();
}

void _level1::draw(float dt, bool paused)
{
    // Clear buffers before trying to draw anything
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Update the camera's view matrix
    cam->setUpCamera();

    glPushMatrix(); // START MATRIX ===========================================
        // Draw the skybox
        glTranslatef(cam->eye.x, cam->eye.y + 20.0f, cam->eye.z);
        glScalef(10.0f, 20.0f, 10.0f);
        skybox->draw();
    glPopMatrix(); // END MATRIX ==============================================

    glPushMatrix(); // START MATRIX ===========================================
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        platformTexture->bind();
        for (_platform* p : platforms) {
            p->draw();
        }
    glPopMatrix(); // END MATRIX ==============================================

    glPushMatrix(); // START MATRIX ===========================================
        glTranslatef(ply->pos.x, ply->pos.y, ply->pos.z);
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glScalef(0.2f, 0.2f, 0.2f);
        glRotatef(ply->facingAngle, 0.0f, 0.0f, 1.0f);

        ply->actions();
        ply->draw(dt, paused);
    glPopMatrix(); // END MATRIX ==============================================

    glPushMatrix(); // START MATRIX ===========================================
        if (!goalReached) {
                boxTex->bind();
            drawBox(-60.0f, 75.0f, -105.0f);
        }
    glPopMatrix(); // END MATRIX ==============================================

    glPushMatrix(); // START MATRIX ===========================================

        // If it's easier to work with I can modify it to use corner-based coordinates IN the box function but this will work too
        float boxW = 0.20f * w;     // Desired width
        float boxH = 0.15f * h;     // Desired height
        float boxCX = 0.5f * boxW;  // This will make the box just draw as a little corner overlay
        float boxCY = 0.5f * boxH;  // Same as above,
        renderBox(boxCX, boxCY, boxW, boxH, col4{0.0f, 0.0f, 0.0f, 0.55f});
        if (timeRemaining < 0.0f) {
            timeRemaining = 0.0f;
        }
        renderBitmapCharacters(0.025f * w, 0.10f * h, col4{1.0f, 1.0f, 1.0f, 1.0f}, GLUT_BITMAP_9_BY_15, "Time: " + to_string(timeRemaining));
    glPopMatrix(); // END MATRIX ==============================================

    glPushMatrix(); // START MATRIX ===========================================

    particles->draw();

    glPopMatrix(); // END MATRIX ==============================================
}

void _level1::onKeyDown(WPARAM wParam)
{
    unsigned char key = static_cast<unsigned char>(wParam); // You can check what key was pressed by testing against uppercase char's e.g. 'W', 'A', 'S', 'D', no need to find ascii codes or whatever

    switch(key)
    {
        case 'Q':
            //ply->playAnim(ply->ACT_ATTACK);
            break;

        default:
            break;
    }
}

void _level1::onKeyUp(WPARAM wParam)
{
    unsigned char key = static_cast<unsigned char>(wParam);

    switch(key)
    {
        default:
            break;
    }
}

void _level1::onLMouseDown(float x, float y)
{

}

void _level1::onRMouseDown(float x, float y)
{

}

void _level1::onMMouseDown(float x, float y)
{

}

void _level1::onLMouseUp(float x, float y)
{

}

void _level1::onRMouseUp(float x, float y)
{

}

void _level1::onMMouseUp(float x, float y)
{

}

void _level1::onMouseMove(float dx, float dy)
{
    // Alter the player's direction based on the x-axis movement of the mouse
    ply->facingAngle += static_cast<float>(dx * cam->sensitivity);
    if (ply->facingAngle > 360.0f) {
        ply->facingAngle -= 360.0f;
    }
    else if (ply->facingAngle < -360.0f) {
        ply->facingAngle = 360.0f;
    }

    // Tie camera's x rotation to the player's facing angle (third person style camera)
    cam->rotAngle.x = ply->facingAngle +180.0f;

    // Alter the camera's pitch based on the y-axis movement of the mouse
    cam->rotAngle.y += dy * cam->sensitivity;
    cam->rotAngle.y = static_cast<float>(clamp(cam->rotAngle.y, -5.0f, 89.0f));
}

void _level1::onMouseWheel(float delta)
{

}

bool _level1::resolvePlayerPlatformContact(float snapTo)
{
    const vec3 playerHalf = ply->playerHalfDimensions;
    ply->grounded = false;
    bool landed = false;

    for (auto* p : platforms) {
        vec3 platformPos{p->x, p->y, p->z};
        vec3 platformHalf = p->halfExtents;

        // Calculate values for determining collision between player and cols
        vec3 delta{ply->pos.x - platformPos.x, ply->pos.y - platformPos.y, ply->pos.z - platformPos.z};
        vec3 absDelta{fabsf(delta.x), fabs(delta.y), fabs(delta.z)};
        vec3 allowance{playerHalf.x + platformHalf.x, playerHalf.y + platformHalf.y, playerHalf.z +platformHalf.z};

        if (absDelta.x > allowance.x || absDelta.y > allowance.y || absDelta.z > allowance.z) {
            continue;
        }

        float penX = allowance.x - absDelta.x;
        float penY = allowance.y - absDelta.y;
        float penZ = allowance.z - absDelta.z;

        if (penY <= penX && penY <= penZ) {
            float platformBottom = platformPos.y - platformHalf.y;
            float playerBottom = ply->pos.y - playerHalf.y;

            if (delta.y >= 0.0f && ply->vy <= 0.0f && playerBottom >= p->topY - snapTo) {
                float snapY = p->topY + playerHalf.y;
                ply->pos.y = snapY;
                ply->groundY = snapY;
                ply->vy = 0.0f;
                ply->grounded = true;
                landed = true;
                continue;
            }

            if (delta.y < 0.0f && ply->vy > 0.0f) {
                ply->pos.y = platformBottom -playerHalf.y - snapTo;
                ply->vy = 0.0f;
                ply->grounded = false;
                continue;
            }
        }
        else if (penX < penY && penX <= penZ) {
            float push = allowance.x - penX + snapTo;
            ply->pos.x = platformPos.x + (delta.x > 0 ? push : -push);
        }
        else {
            float push = allowance.z - penZ + snapTo;
            ply->pos.z = platformPos.z + (delta.z > 0 ? push : -push);
        }
    }
    return landed;
}

void _level1::drawBox(float x, float y, float z)
{
        glPushMatrix();

        glTranslatef(x, y, z);
        glEnable(GL_TEXTURE_2D);

        float dim = 5.0f;

        const float hx = 0.5f * dim, hy = 0.5f * dim, hz = 0.5f * dim;

        glBegin(GL_QUADS);
        // Right
        glNormal3f(1, 0, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( hx,-hy,-hz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( hx,-hy, hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( hx, hy, hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( hx, hy,-hz);
        // Left
        glNormal3f(-1, 0, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-hx,-hy,-hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, hy,-hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Top
        glNormal3f(0, 1, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-hx, hy,-hz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( hx, hy,-hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( hx, hy, hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Bottom
        glNormal3f(0,-1, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( hx,-hy, hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( hx,-hy,-hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-hx,-hy,-hz);
        // Front
        glNormal3f(0, 0, 1);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-hx,-hy, hz);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( hx,-hy, hz);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( hx, hy, hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-hx, hy, hz);
        // Back
        glNormal3f(0, 0, -1);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-hx,-hy,-hz);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( hx,-hy,-hz);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( hx, hy,-hz);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-hx, hy,-hz);
        glEnd();

        glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}
