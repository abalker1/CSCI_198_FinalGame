#include "_game.h"



_game::_game()
{
    // Game must start uninitialized
    this->initialized = false;

    // Set initial values for most variables here
    this->width = GetSystemMetrics(SM_CXSCREEN);
    this->height = GetSystemMetrics(SM_CYSCREEN);
    this->aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    this->windowHandle = nullptr;
    this->cursorLocked = false;

    for (int i = 0; i < keyStates.size(); i++) {
        this->keyStates[i] = false; // Make sure we do not have garbage values in the keyStates array
    }

    this->ui = new _uiManager(); // We have to construct the UI here or we get super awesome access violation crashes later, very cool
    this->ui->setCursorCallbacks([this]() { lockCursor(windowHandle); }, [this]() { unlockCursor(); }); // So the UI can lock/unlock the cursor as needed
    this->ui->resetFirstLevelHack([this] { startFirstLevel(); });   // Oshiete oshiete yo sono shikumi wo... https://www.youtube.com/watch?v=7aMOurgDB-o
    this->paused = ui->enabled; // By default the game will start paused if the UI is enabled and unpaused if the UI is not enabled, but you could set them separately if you want

    this->sound = new _sound();
    // Music by jumpingbunny from Pixabay
    // Jump Sound Effect by EdR from Pixabay

    this->level = 0; // Levels are just scenes stored in a vector, so they're 0 indexed - Technically I guess we could put a dummy nullptr at scenes[0] to force the actual levels to be "1 indexed" (not really, but you get the point)
    this->playedSound = false;
    this->firstLevelStarted = false;

    this->debug = false;
}

_game::~_game()
{

    delete ui;
    delete sound;
    for (_scene* s : scenes) {
        delete s;
    }
    scenes.clear();
}

void _game::init()
{
    // Don't allow re-initialization - it breaks things
    if (initialized) {
        return;
    }

    // ADD ALL LEVELS HERE
    if (scenes.empty()) {
        scenes.push_back(new _level0());
        scenes.push_back(new _level1());
        scenes.push_back(new _level2());
    }

    this->activeScene = scenes[level];
    initActiveScene();

    if (ui->enabled) {
        ui->init(width, height);
        ui->openLanding();
    }

    sound->init();
    sound->playMusic("sounds/background.mp3");

    initialized = true;
}

void _game::initActiveScene()
{
    if (activeScene && !activeScene->initialized) {
        activeScene->init();
    }

    // Init first then set members since we might overwrite something that init sets (that it shouldn't touch)
    if (activeScene) {
        activeScene->w = this->width;
        activeScene->h = this->height;
        activeScene->setKeyGetter([this]{ return keyStates; });
        activeScene->setSoundFunctions([this](const char* path) { sound->playMusic(path); },
                                       [this](const char* path) { sound->playSoundDupe(path); },
                                       [this](const char* path) { sound->playSound(path); },
                                       [this](const char* path) { sound->stopSound(path); });
    }
}

void _game::lockCursor(HWND hWnd)
{
    if (cursorLocked) {
        return;
    }
    windowHandle = hWnd;
    cursorLocked = true;

    POINT pos;
    GetCursorPos(&pos);
    ScreenToClient(windowHandle, &pos);
    prev_msX = pos.x;
    prev_msY = pos.y;

    ShowCursor(FALSE);
    updateCursorClip();
    centerCursor();
}

void _game::unlockCursor()
{
    if (!cursorLocked) {
        return;
    }
    ClipCursor(nullptr);
    cursorLocked = false;
    ShowCursor(TRUE);
}

void _game::updateCursorClip()
{
    if (!windowHandle) {
        return;
    }

    RECT clientRect;
    GetClientRect(windowHandle, &clientRect);

    POINT lt{clientRect.left, clientRect.top};
    POINT rb{clientRect.right, clientRect.bottom};

    ClientToScreen(windowHandle, &lt);
    ClientToScreen(windowHandle, &rb);

    RECT clipRect{lt.x, lt.y, rb.x, rb.y};
    ClipCursor(&clipRect);
}

void _game::centerCursor()
{
    if (!cursorLocked || !windowHandle || !activeScene) {
        return;
    }

    POINT clientCenter{static_cast<long>(width / 2), static_cast<long>(height / 2)};
    POINT screenCenter = clientCenter;

    ClientToScreen(windowHandle, &screenCenter);
    SetCursorPos(screenCenter.x, screenCenter.y);

    prev_msX = clientCenter.x;
    prev_msY = clientCenter.y;
}

void _game::render()
{
    paused = ui->open;

    static auto last = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    double dt = std::chrono::duration<double>(now - last).count();

    last = now;
    clamp(dt, 0.0, 0.05);
    if (ui->resetRequested) {
        ui->resetRequested = false;
        playedSound = false;
        if (level == 0) {
            firstLevelStarted = false;
        }
        activeScene->reset();
    }

    if (!paused) {
        activeScene->update(dt, paused);

    }

    activeScene->draw(dt, paused);
    ui->draw(width, height);

    if (activeScene->shouldWin) {
        ui->winGame();
        if (!playedSound) {
            sound->playSoundDupe("sounds/win.mp3");
            playedSound = true;
        }
    }
    else if (activeScene->shouldLose) {
        if (!playedSound && !ui->open) {
        sound->playSoundDupe("sounds/lose.mp3");
        playedSound = true;
        }
        ui->loseGame();
    }

    if (ui->advanceRequested) {
        if (level < scenes.size() - 1) {
            ui->advanceRequested = false;
            playedSound = false;
            centerCursor();
            advanceLevel();
            ui->resumeGame();
        }
        else {
            ui->advanceRequested = false;
            playedSound = false;
            centerCursor();
            activeScene->reset();
            level = 0;
            activeScene = scenes[0];
            ui->openCredits();
            ui->winPage->objects[2]->init("images/ui/CSS_NextLevel.png");
        }
    }
}

void _game::resize(int w, int h)
{
    aspectRatio = static_cast<float>(w)/static_cast<float>(h);// keep track of the ratio
    glViewport(0,0,w,h); // adjust my viewport

    glMatrixMode(GL_PROJECTION);  // To setup projection
    glLoadIdentity();             // calling identity matrix
    gluPerspective(45, aspectRatio,0.1,1000.0); // setting perspective projection

    this->width = w;
    this->height= h;
    if (activeScene && activeScene->initialized) { // Don't remove this guard or the game will crash
        activeScene->w = w;
        activeScene->h = h;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             // calling identity matrix

    // Recalculate UI layout dynamically on resize to ensure things don't look terrible
    if (ui->enabled) {
        ui->layout(width, height);
    }

    // Update the rectangle the cursor's movement is clipped to
    if (cursorLocked) {
        updateCursorClip();
    }
}

void _game::mouseMapping(int x, int y)
{
    GLint viewport[4];
    GLdouble ModelViewMatrix[16];
    GLdouble projectionMatrix[16];
    GLfloat winX, winY, winZ = 1.0f;

    glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = static_cast<GLfloat>(x);
    winY = static_cast<GLfloat>(viewport[3] - y - 1); // Fix Y coordinate (origin is meant to be bottom-left)

    glReadPixels(x, static_cast<int>(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    gluUnProject(winX, winY, winZ, ModelViewMatrix, projectionMatrix, viewport, &msX, &msY, &msZ);
}

// Forward input to the active scene so that it can decide how it wants to handle things
int _game::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {

        case WM_SETFOCUS:
            if (!ui->open) {
                lockCursor(hWnd);
            } else {
                unlockCursor();
            }
            break;

        case WM_KILLFOCUS:
            unlockCursor();
            break;

        case WM_ACTIVATEAPP:
            if (wParam) {
                if (!ui->open) {
                    lockCursor(hWnd);
                } else {
                    unlockCursor();
                }
            } else {
                unlockCursor();
            }
            break;

        case WM_KEYDOWN:
            if (debug) dumpDebug("handleKeyDown", wParam);

            // If we lock the cursor and have the ui disabled, we need a way to quit easily
            if (!ui->enabled && wParam == VK_ESCAPE) {
                PostQuitMessage(0);
            }

            // BEGIN UI KEYPRESS HANDLING (Don't wire any game functions between here)
            ui->processButtons(wParam);

            // Example of menu-specific key press handling:
            if ((ui->helpPage->active || ui->creditsPage->active) && wParam == VK_ESCAPE) {
                ui->openMenu();
            }

            // Example of key press handling for gameplay:
            if (!paused) {
                // Game is running
                if (!ui->pausePage->active && wParam == VK_ESCAPE ) {
                    ui->pauseGame();
                }

            }
            else {
                // Game is paused (pause screen /should/ be open, hence the pausePage.active guard
                if (ui->pausePage->active && wParam == VK_ESCAPE) {
                    ui->resumeGame();
                }
                if (ui->pausePage->active && wParam == VK_RETURN) {
                    ui->openMenu();
                }
            }
            // END UI KEYPRESS HANDLING (... and here)

            keyStates[wParam] = true;

            // Forward key down message to scene so scene can define its own input handling
            if (!paused) {
                activeScene->onKeyDown(wParam);
            }
            break;

        case WM_KEYUP:

            keyStates[wParam] = false;


            if (!paused) {
                activeScene->onKeyUp(wParam);
            }
            break;

        case WM_LBUTTONDOWN:
            ui->processButtons(LOWORD(lParam), HIWORD(lParam));

            if (!paused) {
                activeScene->onLMouseDown(LOWORD(lParam), HIWORD(lParam));
            }
            break;

        case WM_RBUTTONDOWN:
            if(!paused) {
                activeScene->onRMouseDown(LOWORD(lParam), HIWORD(lParam));
            }
            break;

         case WM_MBUTTONDOWN:
            if(!paused) {
                activeScene->onMMouseDown(LOWORD(lParam), HIWORD(lParam));
            }
            break;

        case WM_LBUTTONUP:
            if(!paused) {
                activeScene->onLMouseUp(LOWORD(lParam), HIWORD(lParam));
            }
            break;

        case WM_RBUTTONUP:
            if(!paused) {
                activeScene->onRMouseUp(LOWORD(lParam), HIWORD(lParam));
            }
            break;

        case WM_MBUTTONUP:
            if(!paused) {
                activeScene->onLMouseUp(LOWORD(lParam), HIWORD(lParam));
            }
            break;

        case WM_MOUSEMOVE:
            if(!paused) {
                if (!cursorLocked) {
                    prev_msX = LOWORD(lParam);
                    prev_msY = HIWORD(lParam);
                    break;
                }

                float dx = prev_msX - LOWORD(lParam);
                float dy = HIWORD(lParam) - prev_msY;

                activeScene->onMouseMove(dx, dy);
                centerCursor();
            }
            break;

        case WM_MOUSEWHEEL:
            if(!paused) {
                activeScene->onMouseWheel(20);
            }
            break;

        default:
            break;
    }
}

void _game::advanceLevel()
{
    // Advance the level and initialize the new scene if we aren't yet on the final level
    if (level < scenes.size() - 1) {
        activeScene->reset();
        level += 1;
        activeScene = scenes[level];
        initActiveScene();
        playedSound = false;
    }

    // This is so there's no "next" button on the win screen of the final level
    if (level >= scenes.size() - 1) {
        ui->winPage->objects[2]->init("images/ui/CSS_WinCredits.png");
    }
}

// This should print virtually all important information for figuring out what state the game is currently in for the sake of figuring out what is happening and why
void _game::dumpDebug(string trigger, WPARAM wParam)
{
    cout << "\n**************** DEBUG ********************\n";

    cout << "Trigger: " << trigger << endl;
    cout << "WPARAM: " << wParam << endl;
    cout << "Current level: " << level << " (" << level + 1 << ")\n";

    string pause = paused ? "Yes\n" : "No\n";
    cout << "Is the game paused: " << pause;

    string uiEmpty = ui->uiStack.empty() ? "Yes\n" : "No\n";
    cout << "Is the UI stack empty: " << uiEmpty;

    string lv = ui->landingPage->visible ? "Yes" : "No";
    string mv = ui->menuPage->visible ? "Yes" : "No";
    string hv = ui->helpPage->visible ? "Yes" : "No";
    string pv = ui->pausePage->visible ? "Yes" : "No";
    string wv = ui->winPage->visible ? "Yes" : "No";
    string lov = ui->losePage->visible ? "Yes" : "No";
    string la = ui->landingPage->active ? "Yes\n" : "No\n";
    string ma = ui->menuPage->active ? "Yes\n" : "No\n";
    string ha = ui->helpPage->active ? "Yes\n" : "No\n";
    string pa = ui->pausePage->active ? "Yes\n" : "No\n";
    string wa = ui->winPage->active ? "Yes\n" : "No\n";
    string loa = ui->losePage->active ? "Yes\n" : "No\n";

    cout << "Is landing page visible / active: \t" << lv << " / " << la;
    cout << "Is menu page visible / active: \t\t" << mv << " / " << ma;
    cout << "Is help page visible / active: \t\t" << hv << " / " << ha;
    cout << "Is pause page visible / active: \t" << pv << " / " << pa;
    cout << "Is win page visible / active: \t\t" << wv << " / " << wa;
    cout << "Is lose page visible / active: \t\t" << lov << " / " << loa;


    cout << endl;
}

void _game::startFirstLevel()
{
    if (firstLevelStarted) {
        return;
    }

    firstLevelStarted = true;
    if (level == 0) {
        activeScene->reset();
    }
}


