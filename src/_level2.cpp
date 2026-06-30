#include "_level2.h"

_level2::_level2()
{
    //ctor
}

_level2::~_level2()
{
    delete light;
    delete cam;
    delete collision;
    delete skybox;
    delete ply;
    delete assignmentMgr;
    delete platformTexture;
    for (_platform* p : platforms) {
        delete p;
    }
    delete particles;
}

void _level2::init()
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
    this->assignmentMgr = new _fallAssignmentMgr();
    this->platformTexture = new _texture();
    this->particles = new _particles();


    // ==========================
    // Restore matrix state
    glPopMatrix();

    // Initialize the general skybox, maybe we can have smaller ones inside or something? I'm not too sure how well that would work.
    // Might rewrite this because it looks ugly and I'm sure theres a better way to do this lmfao
    skybox->skyboxInit();
    skybox->tex[0] = skybox->texture->load("images/assignmentLevel/skybox/class/negz.png");
    skybox->tex[1] = skybox->texture->load("images/assignmentLevel/skybox/class/posz.png");
    skybox->tex[2] = skybox->texture->load("images/assignmentLevel/skybox/class/posy.png");
    skybox->tex[3] = skybox->texture->load("images/assignmentLevel/skybox/class/negy.png");
    skybox->tex[4] = skybox->texture->load("images/assignmentLevel/skybox/class/posx.png");
    skybox->tex[5] = skybox->texture->load("images/assignmentLevel/skybox/class/negx.png");

    // Initialize the player
    ply->initModel("models/billgates/tris.md2", "models/billgates/billywork.jpeg");

    // Add all the platforms
    platformTexture->load("images/assignmentLevel/textures/ground/laminate.jpg");
    platforms.clear();
    platforms.push_back(new _platform(0.0f, -7.0f, 0.0f, 800.0f, 5.0f, 800.0f, 10.0f, 10.0f)); // The last 2 numbers are for texture repeating, tweak them if the texture looks too stretched

    // Set camera distance from player to default
    cam->distance = cam->defaultDistance;

    // Load texture for falling assignments? Might be ugly
    assignmentMgr->loadTexture("images/assignmentLevel/textures/CSS_Assignment.png");

    // After everything necessary to the scene has been initialized, make sure to set initialized to true
    initialized = true;
}

void _level2::reset()
{
    // A reset function will look something like this
    spawned = 0;
    done = false;
    grade = 100.0f;
    timeRemaining = 30.0f;
    ply->pos = playerStartPos;
    assignmentMgr->nextSpawn = 3.0f;
    shouldWin = false;
    shouldLose = false;
    assignments.clear();
    cam->reset();
    particles->clear();
}

void _level2::update(float dt, bool paused)
{
    // Grab the key states from game
    array<bool, 256> keyStates = getKeys();

    // Player stuff
    if (!paused)
    {
        if (spawned >= 1) {
            timeRemaining -= dt;
        }

        if (timeRemaining <= 0.0f && grade >= passing) {
            shouldWin = true;
        }
        else if (timeRemaining <= 0.0f && grade < passing) {
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
        updateAssignments(dt);
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

void _level2::draw(float dt, bool paused)
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
        assignmentMgr->drawAssignments(assignments);
    glPopMatrix(); // END MATRIX ==============================================

    glPushMatrix(); // START MATRIX ===========================================
        // If it's easier to work with I can modify it to use corner-based coordinates IN the box function but this will work too
        // We draw with the corner normalized because the bitmap text's x coordinate is to the corner
        // We could also offset the rendering of the text based on the length since the bitmap font should be monospace
        float boxW = 0.20f * w;     // Desired width
        float boxH = 0.15f * h;     // Desired height
        float boxCX = 0.5f * boxW;  // This will make the box just draw as a little corner overlay
        float boxCY = 0.5f * boxH;  // Same as above,
        renderBox(boxCX, boxCY, boxW, boxH, col4{0.0f, 0.0f, 0.0f, 0.55f});

        col4 gradeColor = {1.0f, 1.0f, 1.0f, 1.0f};
        if (grade >= 90.0f) {
            //A
            gradeColor = {0.0f, 1.0f, 0.0f, 1.0f};
        }
        else if (grade >= 80.0f) {
            //B
            gradeColor = {1.0f, 1.0f, 0.0f, 1.0f};
        }
        else if (grade >= 70.0f) {
            //C
            gradeColor = {1.0f, 0.5f, 0.0f, 1.0f};
        }
        else {
            //D and below
            gradeColor = {1.0f, 0.0f, 0.0f, 1.0f};
        }
        renderBitmapCharacters(0.025f * w, 0.07f * h, gradeColor, GLUT_BITMAP_9_BY_15, "Grade: " + to_string(grade));
        if (timeRemaining < 0.0f) {
            timeRemaining = 0.0f;
        }
        renderBitmapCharacters(0.025f * w, 0.10f * h, col4{1.0f, 1.0f, 1.0f, 1.0f}, GLUT_BITMAP_9_BY_15, "Time: " + to_string(timeRemaining));
    glPopMatrix(); // END MATRIX ==============================================

    glPushMatrix(); // START MATRIX ===========================================
    particles->draw();
    glPopMatrix(); // END MATRIX ==============================================
}

void _level2::onKeyDown(WPARAM wParam)
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

void _level2::onKeyUp(WPARAM wParam)
{
    unsigned char key = static_cast<unsigned char>(wParam);

    switch(key)
    {
        default:
            break;
    }
}

void _level2::onLMouseDown(float x, float y)
{

}

void _level2::onRMouseDown(float x, float y)
{

}

void _level2::onMMouseDown(float x, float y)
{

}

void _level2::onLMouseUp(float x, float y)
{

}

void _level2::onRMouseUp(float x, float y)
{

}

void _level2::onMMouseUp(float x, float y)
{

}

void _level2::onMouseMove(float dx, float dy)
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

void _level2::onMouseWheel(float delta)
{

}

void _level2::updateAssignments(float dt)
{
    // Decrement spawn timer and spawn a new assignment if the spawn timer reaches 0
    assignmentMgr->nextSpawn -= dt;
    if (spawned < maxSpawned && assignmentMgr->nextSpawn <= 0.0f) {
        assignmentMgr->spawnAssignment(assignments, ply->pos);
        spawned++;
    }

    // We have to iterate like this to cleanly delete collided assignments
    for (auto it = assignments.begin(); it != assignments.end();) {
        // Dereference iterator to access object
        auto& a = *it;
        a.update(dt);

        // Check for collisions
        vec3 assignmentHalfDimensions{0.5f*a.w, 0.5f*a.h, 0.5f*a.d};
        if (collision->isCubicCol(ply->pos, vec3{a.x, a.y, a.z}, ply->playerHalfDimensions, assignmentHalfDimensions)) {
            if (!a.graded) {
                a.graded = true;
                particles->createParticles(EXPLOSION, vec3{a.x, a.y, a.z});
                playDupeS("sounds/win.mp3");
                if (spawned >= maxSpawned) {
                    done = true;
                }
            }
            it = assignments.erase(it);
            continue;
        }
        else if (a.y - (0.5f * a.h) < -5.0f/*This is kind of hacky but more efficient than checking collision*/) {
            grade = clamp(grade - frand(penMin, penMax), 0.0f, 100.0f);
            particles->createParticles(EXPLOSION, vec3{a.x, a.y, a.z});
            if (spawned >= maxSpawned) {
                done = true;
            }
        }

        // Move to next assignment
        it++;
    }

    // Remove assignments that are past the despawn point
    assignments.erase(remove_if(assignments.begin(), assignments.end(), [&](const _fallAssignment& a) {return (a.y - (0.5f * a.h)) < -5.0f;}), assignments.end());
}

bool _level2::resolvePlayerPlatformContact(float snapTo)
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
