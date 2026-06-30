#include "_level0.h"

_level0::_level0()
{
    shouldLose = false;
    shouldWin = false;
}

_level0::~_level0()
{
    delete light;
    delete cam;
    delete collision;
    delete parallax;
    delete skybox;
    delete myModel;
    delete myTexture;
    delete parkedTexture1;
    delete parkedTexture2;
    delete myPrlx;
    delete roadPrlx;
    delete backgroundPrlx;
    delete timerShow;
    delete myInput;
    delete gameTimer;
}

void _level0::init()
{
    // We do this here instead of the constructor because this all requires a valid OpenGL context.
    glShadeModel(GL_SMOOTH);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClearDepth(2.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Save current matrix state and reset to identity
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Initialize children
    this->light = new _light(GL_LIGHT0);
    this->cam = new _camera();
    this->collision = new _collision();
    this->parallax = new _parallax();
    this->skybox = new _skybox();
    this->myModel = new _objModel();
    this->myTexture = new _texture();
    this->parkedTexture1 = new _texture();
    this->parkedTexture2 = new _texture();
    this->myPrlx = new _parallax();  // Clouds
    this->roadPrlx = new _parallax();
    this->backgroundPrlx = new _parallax();  // Background fence
    this->sky = new _parallax();
    this->timerShow = new _parallax();
    this->myInput = new _inputs();
    this->gameTimer = new _timer();

    // Restore matrix state
    glPopMatrix();

    myTexture->load("images/carLevel/car.png");
    parkedTexture1->load("images/carLevel/car1.png");
    parkedTexture2->load("images/carLevel/car2.png");

    // Load OBJ model
    myModel->loadMyOBJ("images/carLevel/car.obj");

    myPrlx->init("images/carLevel/letssee.png");  // Clouds
    roadPrlx->init("images/carLevel/road2.png");
    backgroundPrlx->init("images/carLevel/background-fence.png");  // Background sky parallax
    sky->init("images/carLevel/background-sky.png");
    timerShow->init("images/carLevel/10.png");

    // Start the game timer
    gameTimer->reset();
    currentGameState = PLAYING;

    parkingSpotModel.load("images/carLevel/car.obj");

    // Initialize visual parked cars
    float carSpacing = zRange / 6.0f; // Space cars evenly across the range

    // Left side cars
    for(int i = 0; i < 6; ++i) {
        visualParkedCar car;
        car.z_position = zStart + (i * carSpacing);
        car.x_offset = -5.0f; // Left side
        car.textureIndex = rand() % 2; // Random texture
        car.isVisible = true;
        visualCars.push_back(car);
    }

    // Right side cars
    for(int i = 0; i < 6; ++i) {
        visualParkedCar car;
        car.z_position = zStart + (i * carSpacing) + (carSpacing * 0.5f); // Offset by half spacing
        car.x_offset = 6.0f; // Right side
        car.textureIndex = rand() % 2; // Random texture
        car.isVisible = true;
        visualCars.push_back(car);
    }

    int randInt1 = (rand()%12);
    visualCars[randInt1].isVisible = false;

    // Further cars - Left side
    for(int i = 0; i < 6; ++i) {
        visualParkedCar car;
        car.z_position = zStart + (i * carSpacing);
        car.x_offset = -12.0f; // Left side
        car.textureIndex = rand() % 2; // Random texture
        car.isVisible = true;
        visualCars.push_back(car);
    }

    // Further cars - Right side
    for(int i = 0; i < 6; ++i) {
        visualParkedCar car;
        car.z_position = zStart + (i * carSpacing) + (carSpacing * 0.5f); // Offset by half spacing
        car.x_offset = 13.0f; // Right side
        car.textureIndex = rand() % 2; // Random texture
        car.isVisible = true;
        visualCars.push_back(car);
    }

    cam->camInitFPS();

    /*
    skybox->skyboxInit();
    skybox->tex[0] = skybox->texture->load("images/skybox/nz.png"); //front
    skybox->tex[1] = skybox->texture->load("images/skybox/pz.png"); //back
    skybox->tex[2] = skybox->texture->load("images/skybox/py.png"); //top
    skybox->tex[3] = skybox->texture->load("images/skybox/ny.png"); //bottom
    skybox->tex[4] = skybox->texture->load("images/skybox/px.png"); //right
    skybox->tex[5] = skybox->texture->load("images/skybox/nx.png"); //left
    */

    initialized = true;
}

void _level0::reset()
{
    // Reset game state
    currentGameState = PLAYING;
    isParked = false;
    isParkingAnimationActive = false;
    shouldRenderPlayerCar = true;
    parkingAnimStage = 0;
    parkingAnimTimer = 0.0f;

    // Reset player model
    myModel->posX = 1.0f;
    myModel->posY = -2.0f;
    myModel->posZ = -8.0f;
    myModel->rotateZ = 0.0f;
    myModel->horizontalPos = 0.0f;
    myModel->targetHorizontalPos = 0.0f;

    // Reset input
    myInput->currentSpeed = 0.0f;
    myInput->isUpPressed = false;
    myInput->isDownPressed = false;
    myInput->isLeftPressed = false;
    myInput->isRightPressed = false;
    myInput->totalRoadOffsetZ = 0.0f;

    // Reset camera
    isFirstPersonView = false;
    cam->camInit();

    // Reset timer
    gameTimer->reset();

    // Reset visual cars - make one random spot available again
    for (auto& car : visualCars) {
        car.isVisible = true;
    }
    int randInt1 = (rand() % 12);
    visualCars[randInt1].isVisible = false;

    shouldWin = false;
    shouldLose = false;
    backgroundZoom = 0.4f;
}

void _level0::update(float dt, bool paused)
{
    if (paused) return;

    // Check timer and game state (only in Level0)
    if (currentGameState == PLAYING && !isParkingAnimationActive) {
        checkTimer();
    }

    // Update input and model
    myInput->update(myModel, roadPrlx, myPrlx);
    myModel->update();

    // Update background zoom based on speed (zoom in effect while driving)
    float speedRatio = (myInput->currentSpeed / myInput->maxSpeed);
    backgroundZoom += (speedRatio * (maxZoom - 0.5f)) * 0.0005;  // Zoom from 1.0 to maxZoom

    // Clouds parallax (myPrlx) movement is handled in _inputs::update()

    checkParking();
    updateParkingAnimation();

    // Set win/lose flags based on game state (let UI manager handle display)
    if (currentGameState == WON) {
        shouldWin = true;
    } else if (currentGameState == LOST) {
        shouldLose = true;
    }
}

void _level0::draw(float dt, bool paused)
{
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up camera based on view mode
    if (isFirstPersonView) {
        updateFirstPersonCamera();
        cam->setUpCamera();
    } else {
        // Default 2D view - use standard camera
        cam->camInit();
        cam->setUpCamera();
    }

    // Background sky parallax
    glPushMatrix();
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glTranslatef(0,5,-100);
        glScalef(50,50,5);
        sky->drawParallax(w, h);
        //glDisable(GL_BLEND);
    glPopMatrix();

    // Background fence parallax
    glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glTranslatef(0, 8, -50);
        //glScalef(55.0f * backgroundZoom, 15.0f * backgroundZoom, 0.0);  // Zoom effect
        glScalef(55.0f * backgroundZoom, 25.0f * (backgroundZoom/2), 0.0);  // Zoom effect
        glTranslatef(0,backgroundZoom/3, 0);
        backgroundPrlx->drawParallax(w, h);
        glDisable(GL_BLEND);
    glPopMatrix();

    // Moving clouds parallax (letssee) - moves on top of background
    glPushMatrix();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glTranslatef(0, 10, -45);  // Slightly closer than background
        glScalef(45, 2, 0.0);
        myPrlx->drawParallax(w, h);
        glDisable(GL_BLEND);
    glPopMatrix();

    // the road
    glPushMatrix();
        glTranslatef(0.0, 37.0, -5.0);
        glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
        glScalef(0.8, -8, 0.8);
        roadPrlx->drawParallaxRoad(w, h);
    glPopMatrix();

    // Draw visual parked cars that loop with the parallax
    drawVisualParkedCars();

    // the car
    if (shouldRenderPlayerCar) {
        glPushMatrix();
            myTexture->bind(); // Bind the texture first
            glEnable(GL_LIGHTING);
            glRotatef(15, 0.5, 0.75, 0.0);
            glRotatef(89, -1, 0.0, 0.0);
            glRotatef(-10, 0, 0, 1);
            glTranslatef(1.0,5.0,-0.5);
            glScalef(0.2,0.2,0.2);
            myModel->drawModel();
        glPopMatrix();
    }

    // Timer display
    glPushMatrix();
        glTranslatef(0, 1.35, 0);
        glScalef(0.075, 0.075, 0.05);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        timerShow->drawParallax(w, h);
    glPopMatrix();
}

void _level0::onKeyDown(WPARAM wParam)
{
    myInput->wParam = wParam;

    // Handle "P" key for first-person view toggle (only when playing)
    if (currentGameState == PLAYING && (wParam == 'P' || wParam == 'p')) {
        toggleFirstPersonView();
    }

    // Only process game input when playing
    if (currentGameState == PLAYING) {
        if(!isParkingAnimationActive) {
            myInput->keyPressed(myModel);
        }
        myInput->keyPressed(roadPrlx);
    }
}

void _level0::onKeyUp(WPARAM wParam)
{
    if (currentGameState == PLAYING) {
        myInput->wParam = wParam;
        myInput->keyUp();
    }
}

void _level0::onLMouseDown(float x, float y)
{
}

void _level0::onRMouseDown(float x, float y)
{
}

void _level0::onMMouseDown(float x, float y)
{
}

void _level0::onLMouseUp(float x, float y)
{
}

void _level0::onRMouseUp(float x, float y)
{
}

void _level0::onMMouseUp(float x, float y)
{
}

void _level0::onMouseMove(float dx, float dy)
{
}

void _level0::onMouseWheel(float delta)
{
}

void _level0::drawVisualParkedCars()
{
    float yStart = 1.5f;     // Y at Z=-20 (far, higher)
    float yEnd = -5.0f;      // Y at Z=10 (close, lower)
    float speedMultiplier = 0.3f;

    float movement = myInput->totalRoadOffsetZ * speedMultiplier;       // Movement from input, scaled to match player car speed

    for (auto& car : visualCars)
    {
        if (!car.isVisible) continue; // Skip if not visible

        glPushMatrix();
            float currentZ = car.z_position + movement;

            while (currentZ > zEnd) {
                currentZ -= zRange;
            }
            while (currentZ < zStart) {
                currentZ += zRange;
            }

            // Linear interpolation: Y = yStart + (currentZ - zStart) * (yEnd - yStart) / (zEnd - zStart)
            car.y_position = yStart + (currentZ - zStart) * (yEnd - yStart) / zRange;

            glTranslatef(car.x_offset, car.y_position, currentZ);

            // Draw the model
            glEnable(GL_LIGHTING);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            glRotatef(-10,0.0f,1.0f,0.0f);
            glScalef(1.75,1.75,1.75);

            // Use the car's assigned texture
            if (car.textureIndex == 0) parkedTexture1->bind();
            else if (car.textureIndex == 1) parkedTexture2->bind();
            else myTexture->bind();

            parkingSpotModel.draw();
            glDisable(GL_LIGHTING);
        glPopMatrix();
    }
}

void _level0::checkParking() {
    if (!myInput->isSpacePressed) return;
    if (isParkingAnimationActive || isParked) {
        myInput->isSpacePressed = false;
        return;
    }
    myInput->isSpacePressed = false;

    // --- Collision Parameters ---

    // Player Car Position and Half-Dimensions (P1, H1)
    vec3 carPos = {static_cast<float>(myModel->posX), 0.5f, -10.0f};
    vec3 carHalfDimensions = {1.0f, 0.5f, 2.0f};

    // Parking Spot Half-Dimensions (H2)
    vec3 spotHalfDimensions = {2.0f, 0.5f, 3.0f};

    // --- Looping and Movement Parameters (Must match drawVisualParkedCars) ---
    const float speedMultiplier = 0.3f;      // Keep movement in sync with visuals
    const float movement = myInput->totalRoadOffsetZ * speedMultiplier;

    // Determine which side the player is attempting to park on: right (>=0) or left (<0)
    const float rightSpotX = 6.0f;
    const float leftSpotX = -5.0f;
    const float xTolerance = 0.2f;           // Allow a little drift in the stored offsets
    const float zMatchTolerance = 2.0f;      // How close in Z the car must be to count as "next to" the player

    bool checkingRight = (myModel->posX >= 0.0f);
    float targetXOffset = checkingRight ? rightSpotX : leftSpotX;

    visualParkedCar* closestCar = nullptr;
    float closestDeltaZ = 1e9f;
    float closestCarZ = 0.0f;

    for (auto& car : visualCars) {
        if (fabs(car.x_offset - targetXOffset) > xTolerance) {
            continue;
        }

        float currentZ = car.z_position + movement;
        while (currentZ > zEnd) { currentZ -= zRange; }
        while (currentZ < zStart) { currentZ += zRange; }

        float deltaZ = fabs(currentZ - carPos.z);
        if (deltaZ < closestDeltaZ) {
            closestDeltaZ = deltaZ;
            closestCar = &car;
            closestCarZ = currentZ;
        }
    }

    if (!closestCar || closestDeltaZ > zMatchTolerance) {
        cout << "PARKING MISSED! No car aligned with your side when you pressed Space." << endl;
        return;
    }

    if (!closestCar->isVisible) {
        cout << "SUCCESS! Parking animation starting for spot at Z=" << closestCarZ << endl;

        myInput->currentSpeed = 0.0f;
        myInput->isUpPressed = false;
        myInput->isDownPressed = false;
        myInput->isLeftPressed = false;
        myInput->isRightPressed = false;

        startParkingAnimation(closestCar, closestCarZ);
        return;
    }

    cout << "FAILURE! Spot is occupied at Z=" << closestCarZ << endl;
    return;
}

void _level0::startParkingAnimation(visualParkedCar* car, float targetZ)
{
    // Check if parked within time limit
    float elapsedSeconds = (float)gameTimer->getTicks() / CLOCKS_PER_SEC;
    if (elapsedSeconds <= timeLimit) {
        cout << "WIN! Parked in " << elapsedSeconds << " seconds (within " << timeLimit << " second limit)!" << endl;
    } else {
        currentGameState = LOST;
        cout << "LOSE! Took " << elapsedSeconds << " seconds (exceeded " << timeLimit << " second limit)" << endl;
    }
    if (!car) {
        return;
    }

    isParkingAnimationActive = true;
    shouldRenderPlayerCar = true;
    parkingAnimTimer = 0.0f;
    parkingAnimStage = 0;
    parkingAnimTargetCar = car;
    parkingAnimTargetZ = targetZ;

    parkingAnimStartPos = {static_cast<float>(myModel->posX + 1.0f), 5.0f, -0.5f};
    parkingAnimStartRotationZ = static_cast<float>(myModel->rotateZ);
    parkingAnimTargetRotation = 90.0f;

    const float RIGHT_X_MULT = 2.5f;
    const float LEFT_X_MULT = 1.0f;
    float xMult = (car->x_offset < 0) ? LEFT_X_MULT : RIGHT_X_MULT;

    float targetX = car->x_offset * xMult;

    if (targetX < 0) {
        parkingAnimEndPos = {targetX-13, 5, -8};
    }
    else if (targetX > 0) {
        parkingAnimEndPos = {targetX, 5, -8};
    }
}

void _level0::updateParkingAnimation()
{
    if (!isParkingAnimationActive) {
        return;
    }

    const float frameDelta = 0.008f; // Approximate 60 FPS
    parkingAnimTimer += frameDelta;

    auto moveTowards = [](double& current, float target, float step) {
        float delta = target - current;
        if (fabs(delta) <= step) {
            current = target;
            return true;
        }
        current += (delta > 0.0f ? step : -step);
        return false;
    };

    bool stageComplete = false;

    switch (parkingAnimStage) {
        case 0: // Align along Z axis
            stageComplete = moveTowards(myModel->posZ, parkingAnimEndPos.z, parkingAnimPositionStep);
            if (stageComplete) {
                parkingAnimStage = 1;
            }
            break;

        case 1: // Align along Y axis
            stageComplete = moveTowards(myModel->posY, parkingAnimEndPos.y, parkingAnimPositionStep);
            if (stageComplete) {
                parkingAnimStage = 2;
            }
            break;

        case 2: // Rotate the car to face the spot
            stageComplete = moveTowards(myModel->rotateZ, parkingAnimTargetRotation, parkingAnimRotationStep);
            if (stageComplete) {
                parkingAnimStage = 3;
            }
            break;

        case 3: // Align along X axis
            stageComplete = moveTowards(myModel->posX, parkingAnimEndPos.x, parkingAnimPositionStep);
            myModel->targetHorizontalPos = static_cast<float>(myModel->posX); // prevent drift from input smoothing
            if (stageComplete) {
                parkingAnimStage = 4;
            }
            break;

        case 4: // Finalize the parking sequence
            isParked = true;
            cout << "SUCCESS! Parked in open spot at Z=" << parkingAnimTargetZ << endl;
            currentGameState = WON;
            parkingAnimStage = 5;
            break;

        default:
            break;
    }
}

void _level0::toggleFirstPersonView()
{
    isFirstPersonView = !isFirstPersonView;

    if (isFirstPersonView) {
        // Initialize FPS camera when entering first-person mode
        cam->camInitFPS();

        // Set camera bounds to match car movement limits to prevent strafing beyond car's range
        float maxHorizontalPosLeft = 2.0f;
        float maxHorizontalPosRight = 1.0f;
        float padding = 0.5f; // Small padding to prevent edge cases

        cam->minX = -maxHorizontalPosLeft - padding;
        cam->maxX = maxHorizontalPosRight + padding;
        // Z bounds can be wider since the car doesn't move in Z (road scrolls instead)
        cam->minZ = -2.0f;
        cam->maxZ = 2.0f;

        cout << "First Person View: ON" << endl;
    } else {
        // Reset to default camera when exiting first-person mode
        cam->camInit();
        cout << "First Person View: OFF" << endl;
    }
}

void _level0::updateFirstPersonCamera()
{
    if (isParkingAnimationActive) {
        isFirstPersonView = false;
        return;
    }
    // Position camera at the windshield of the player car model
    float carX = static_cast<float>(myModel->posX);
    float carY = static_cast<float>(myModel->posY);
    float carZ = static_cast<float>(myModel->posZ);

    // Car movement bounds (must match the bounds in _inputs::update)
    float maxHorizontalPosLeft = 2.5f;
    float maxHorizontalPosRight = 1.5f;

    // Clamp carX to the car's movement bounds
    if (carX < -maxHorizontalPosLeft) {
        carX = -maxHorizontalPosLeft;
    } else if (carX > maxHorizontalPosRight) {
        carX = maxHorizontalPosRight;
    }

    // During parking animation, use the actual parked position
    if (isParkingAnimationActive || isParked) {
        carY = 0.5f;
    }

    // Account for the car's Z rotation (parking rotation)
    float carRotationZ = static_cast<float>(myModel->rotateZ);
    float rotRadZ = carRotationZ * PI / -180.0f;

    // Windshield position in car's local space
    float localWindshieldX = 0.0f;
    float localWindshieldY = 0.0f;
    float localWindshieldZ = -0.2f;

    // Apply car's Z rotation to windshield position
    float rotatedWindshieldX = localWindshieldX * cos(rotRadZ) - localWindshieldZ * sin(rotRadZ);
    float rotatedWindshieldZ = localWindshieldX * sin(rotRadZ) + localWindshieldZ * cos(rotRadZ);

    // Set camera position
    float cameraY;
    if (!isParkingAnimationActive && !isParked) {
        cameraY = carY + 2.0f + 0.1f;
    } else {
        cameraY = carY + 0.1f;
    }

    // Camera position = car position + rendering offset + rotated windshield offset
    cam->eye.x = carX + 1.0f + rotatedWindshieldX;
    cam->eye.y = cameraY;
    cam->eye.z = carZ - 0.5f + rotatedWindshieldZ;

    // Calculate forward direction based on car's rotation
    float forwardX = sin(rotRadZ);
    float forwardZ = -cos(rotRadZ);

    float lookAheadDistance = 20.0f;

    // Set camera look target
    cam->des.x = cam->eye.x + forwardX * lookAheadDistance;
    cam->des.y = cam->eye.y;
    cam->des.z = cam->eye.z + forwardZ * lookAheadDistance;

    // Update rotation angles
    vec3 lookDir;
    lookDir.x = cam->des.x - cam->eye.x;
    lookDir.y = cam->des.y - cam->eye.y;
    lookDir.z = cam->des.z - cam->eye.z;

    // Calculate horizontal angle (yaw)
    float yaw = atan2(lookDir.x, lookDir.z) * 180.0f / PI;
    cam->rotAngle.x = yaw;
    cam->rotAngle.y = -3.0f;  // Slight downward angle to see the road
}

void _level0::checkTimer()
{
    if (currentGameState != PLAYING && initialized) {
        return;
    }

    // Get elapsed time in seconds
    float elapsedSeconds = (float)gameTimer->getTicks() / CLOCKS_PER_SEC;
    int simpleTime = static_cast<int>(floor(elapsedSeconds));

    switch(simpleTime) {
        case 0:
            timerShow->init("images/carLevel/10.png");
            break;
        case 1:
            timerShow->init("images/carLevel/9.png");
            break;
        case 2:
            timerShow->init("images/carLevel/8.png");
            break;
        case 3:
            timerShow->init("images/carLevel/7.png");
            break;
        case 4:
            timerShow->init("images/carLevel/6.png");
            break;
        case 5:
            timerShow->init("images/carLevel/5.png");
            break;
        case 6:
            timerShow->init("images/carLevel/4.png");
            break;
        case 7:
            timerShow->init("images/carLevel/3.png");
            break;
        case 8:
            timerShow->init("images/carLevel/2.png");
            break;
        case 9:
            timerShow->init("images/carLevel/1.png");
            break;
    }

    // Check if time limit exceeded
    if (elapsedSeconds >= timeLimit) {
        currentGameState = LOST;
        cout << "TIME'S UP! You took " << elapsedSeconds << " seconds." << endl;
    }
}

void _level0::restartLevel()
{
    reset();
}

