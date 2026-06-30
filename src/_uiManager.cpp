#include "_uiManager.h"

_uiManager::_uiManager()
{
    this->enabled = true;

    this->initialized = false;
    this->open = false;
    this->helpIndex = 0;

    this->resetRequested = false;
    this->advanceRequested = false;

    this->helpPages = {
        "images/ui/CSS_HelpPage1.png",
        "images/ui/CSS_HelpPage2.png",
        "images/ui/CSS_HelpPage3.png"
    };
}

_uiManager::~_uiManager()
{
    uiStack.clear();
    delete landingPage;
    delete menuPage;
    delete helpPage;
    delete pausePage;
    delete winPage;
    delete losePage;
    delete creditsPage;
}

void _uiManager::setCursorCallbacks(function<void()> lock, function<void()> unlock)
{
    requestLockCursor = move(lock);
    requestUnlockCursor = move(unlock);
}

void _uiManager::resetFirstLevelHack(function<void()>reset)
{
    resetFirstLevel = move(reset);
}


void _uiManager::init(float w, float h)
{
    // Don't need to initialize the UI if it is already initialized
    if(initialized) {
        return;
    }

    // Set up landing page
    landingPage->clear();
    landingPage->addObject("images/ui/CSS_Landing.png"); // 0
    landingPage->addButton("images/ui/CSS_Start.png", [&]{this->openMenu();}, 'S');  // 1

    // Set up menu page
    menuPage->clear();
    menuPage->addObject("images/ui/CSS_Landing.png"); // 0
    menuPage->addButton("images/ui/CSS_Play.png", [&]{this->startGame();}, 'N'); // 1
    menuPage->addButton("images/ui/CSS_Help.png", [&]{this->openHelp();}, 'H'); // 2
    menuPage->addButton("images/ui/CSS_Credits.png", [&]{this->openCredits();}, 'C'); // 3
    menuPage->addButton("images/ui/CSS_Quit.png", [&]{this->quitGame();}, 'Q'); // 4

    // Set up help page
    helpPage->clear();
    helpPage->overlay = true;
    helpPage->addObject(helpPages[helpIndex]); // 0
    helpPage->addButton("images/ui/CSS_Right.png", [&]{this->nextHelp();}, 'R'); // 1
    helpPage->addButton("images/ui/CSS_Left.png", [&]{this->prevHelp();}, 'L'); // 2

    // Set up pause page
    pausePage->clear();
    pausePage->overlay = true;
    pausePage->addObject("images/ui/CSS_PauseConfirm.png"); // 0
    pausePage->addButton("images/ui/CSS_Yes.png", [&]{this->openMenu();}, 'Y'); // 1
    pausePage->addButton("images/ui/CSS_No.png", [&]{this->resumeGame();}, 'N'); // 2

    // Set up win page
    winPage->clear();
    winPage->overlay = true;
    winPage->addObject("images/ui/CSS_WinPage.png"); // 0
    winPage->addButton("images/ui/CSS_Menu.png", [&]{this->resetGame(); this->openMenu();}, 'M'); // 1
    winPage->addButton("images/ui/CSS_NextLevel.png", [&]{this->advanceRequested = true;}, 'N'); // 2

    // Set up lose page
    losePage->clear();
    losePage->overlay = true;
    losePage->addObject("images/ui/CSS_LosePage.png"); // 0
    losePage->addButton("images/ui/CSS_Menu.png", [&]{this->resetGame(); this->openMenu();}, 'M'); // 1
    losePage->addButton("images/ui/CSS_Restart.png", [&]{this->resetGame(); this->resumeGame();}, 'R'); // 2

    // Set up credits page
    creditsPage->clear();
    creditsPage->addObject("images/ui/CSS_CreditsPage.png"); // 1

    // Finalize UI initialization
    initialized = true;
    layout(w, h);
}

void _uiManager::layout(float w, float h)
{
    // Don't try to lay out an uninitialized UI
    if (!initialized) {
        return;
    }

    float cx = (w * 0.5f);
    float cy = (h * 0.5f);

    // Lay out landing page
    landingPage->objects[0]->setFrame(cx, cy, w, h);

    float landingPlayButtonAR = landingPage->objects[1]->getAspectRatio();
    float landingPlayButtonWidth = 0.40f * w;
    float landingPlayButtonHeight = landingPlayButtonWidth * landingPlayButtonAR;
    float landingPlayButtonY = h * 0.70f;
    landingPage->objects[1]->setFrame(cx, landingPlayButtonY, landingPlayButtonWidth, landingPlayButtonHeight);

    // Lay out menu page
    menuPage->objects[0]->setFrame(cx, cy, w, h);

    float menuButtonAR = menuPage->objects[1]->getAspectRatio();
    float menuButtonWidth = 0.25f * w;
    float menuButtonHeight = menuButtonWidth * menuButtonAR;

    float menuCenterButtonY = cy + 0.17f * h;
    float menuButtonOffset = 1.2f * menuButtonHeight;

    float menuHelpButtonY = menuCenterButtonY - menuButtonOffset;
    float menuStartButtonY = menuHelpButtonY - menuButtonOffset;
    float menuCreditButtonY = menuHelpButtonY + menuButtonOffset;
    float menuExitButtonY = menuCreditButtonY + menuButtonOffset;

    menuPage->objects[1]->setFrame(cx, menuStartButtonY, menuButtonWidth, menuButtonHeight);
    menuPage->objects[2]->setFrame(cx, menuHelpButtonY, menuButtonWidth, menuButtonHeight);
    menuPage->objects[3]->setFrame(cx, menuCreditButtonY, menuButtonWidth, menuButtonHeight);
    menuPage->objects[4]->setFrame(cx, menuExitButtonY, menuButtonWidth, menuButtonHeight);


    // Lay out help page
    float helpPageAr = helpPage->objects[0]->getAspectRatio();
    float helpPageWidth = 0.50f * w;
    float helpPageHeight = helpPageWidth * helpPageAr;
    helpPage->objects[0]->setFrame(cx, cy, helpPageWidth, helpPageHeight);

    float scrollButtonAR = helpPage->objects[1]->getAspectRatio();
    float scrollButtonWidth = 0.15f * helpPageWidth;
    float scrollButtonHeight = scrollButtonWidth * scrollButtonAR;

    float rightX = cx + 0.5f * helpPageWidth + 0.75f * scrollButtonWidth;
    float leftX = cx - 0.5f * helpPageWidth - 0.75f * scrollButtonWidth;

    helpPage->objects[1]->setFrame(rightX, cy, scrollButtonWidth, scrollButtonHeight);
    helpPage->objects[2]->setFrame(leftX, cy, scrollButtonWidth, scrollButtonHeight);

    // Lay out credits page
    creditsPage->objects[0]->setFrame(cx, cy, w, h);

    // Lay out pause page
    float pauseConfirmAR = pausePage->objects[0]->getAspectRatio();
    float pauseConfirmWidth = 0.50f * w;
    float pauseConfirmHeight = pauseConfirmWidth * pauseConfirmAR;
    float pauseConfirmY = pauseConfirmHeight;
    pausePage->objects[0]->setFrame(cx, pauseConfirmY, pauseConfirmWidth, pauseConfirmHeight);

    float pauseButtonAR = pausePage->objects[1]->getAspectRatio();
    float pauseButtonWidth = 0.40f * pauseConfirmWidth;
    float pauseButtonHeight = pauseButtonWidth * pauseButtonAR;
    float pauseButtonY = pauseConfirmY + 0.15f * pauseConfirmHeight;
    float pauseButtonYesX = cx - 0.50f * pauseConfirmWidth + 0.65f * pauseButtonWidth;
    float pauseButtonNoX = cx + 0.50f * pauseConfirmWidth - 0.65f * pauseButtonWidth;

    pausePage->objects[1]->setFrame(pauseButtonYesX, pauseButtonY, pauseButtonWidth, pauseButtonHeight);
    pausePage->objects[2]->setFrame(pauseButtonNoX, pauseButtonY, pauseButtonWidth, pauseButtonHeight);

    // Lay out win page
    float winPanelAR = winPage->objects[0]->getAspectRatio();
    float winPanelWidth = 0.5f * w;
    float winPanelHeight = winPanelWidth * winPanelAR;
    float winPanelY = pauseConfirmY;

    winPage->objects[0]->setFrame(cx, winPanelY, winPanelWidth, winPanelHeight);
    winPage->objects[1]->setFrame(pauseButtonYesX, pauseButtonY, pauseButtonWidth, pauseButtonHeight);
    winPage->objects[2]->setFrame(pauseButtonNoX, pauseButtonY, pauseButtonWidth, pauseButtonHeight);

    // Lay out lose page (it's the same as the win screen with a different panel so we can just do this)
    losePage->objects[0]->setFrame(cx, winPanelY, winPanelWidth, winPanelHeight);
    losePage->objects[1]->setFrame(pauseButtonYesX, pauseButtonY, pauseButtonWidth, pauseButtonHeight);
    losePage->objects[2]->setFrame(pauseButtonNoX, pauseButtonY, pauseButtonWidth, pauseButtonHeight);
}

void _uiManager::draw(float w, float h)
{
    if (!uiStack.empty()) {
        open = true;
        for (auto *page : uiStack) {
            page->draw(w, h);
        }
    }
}

void _uiManager::reset() {

    // Guard against calling any UI functions when the UI is not enabled
    if (!enabled) {
        return;
    }

    landingPage->setVisible(false);
    menuPage->setVisible(false);
    helpPage->setVisible(false);
    pausePage->setVisible(false);
    winPage->setVisible(false);
    losePage->setVisible(false);

    landingPage->active = false;
    menuPage->active = false;
    helpPage->active = false;
    pausePage->active = false;
    winPage->active = false;
    losePage->active = false;
}

void _uiManager::openLanding()
{
    if (!enabled) {
        return;
    }
    if (requestUnlockCursor) {
        requestUnlockCursor();
    }
    open = true;
    reset();
    landingPage->setVisible(true);
    landingPage->active = true;
    if (uiStack.empty() || uiStack.back() != landingPage) {
        uiStack.push_back(landingPage);
    }
}

void _uiManager::openMenu()
{
    if (!enabled) {
        return;
    }
    if (requestUnlockCursor) {
        requestUnlockCursor();
    }
    open = true;
    if (!uiStack.empty()) {
        uiStack.pop_back();
    }
    reset();
    menuPage->setVisible(true);
    menuPage->active = true;
    if (uiStack.empty() || uiStack.back() != menuPage) {
        uiStack.push_back(menuPage);
    }
}

void _uiManager::openHelp()
{
    if (!enabled) {
        return;
    }
    if (requestUnlockCursor) {
        requestUnlockCursor();
    }
    open = true;
    reset();
    menuPage->setVisible(true);
    helpPage->setVisible(true);
    helpPage->active = true;
    if (uiStack.empty() || uiStack.back() != helpPage) {
        uiStack.push_back(helpPage);
    }
}

void _uiManager::nextHelp()
{
    helpIndex = (helpIndex + 1) % helpPages.size();
    helpPage->objects[0]->init(helpPages[helpIndex]);
}

void _uiManager::prevHelp()
{
    helpIndex = (helpIndex - 1) % helpPages.size();
    helpPage->objects[0]->init(helpPages[helpIndex]);
}


void _uiManager::openCredits()
{
    if (!enabled) {
        return;
    }
    if (requestUnlockCursor) {
        requestUnlockCursor();
    }
    open = true;
    if (!uiStack.empty()) {
        uiStack.pop_back();
    }
    reset();
    creditsPage->setVisible(true);
    creditsPage->active = true;
    if (uiStack.empty() || uiStack.back() != creditsPage) {
        uiStack.push_back(creditsPage);
    }
}

void _uiManager::startGame()
{
    if (!enabled) {
        return;
    }
    if (!uiStack.empty()) {
        uiStack.pop_back();
    }
    if (requestLockCursor) {
        requestLockCursor();
    }
    resetFirstLevel();
    open = false;
    reset();
}

void _uiManager::pauseGame()
{
    if (!enabled) {
        return;
    }
    if (requestUnlockCursor) {
        requestUnlockCursor();
    }
    open = true;
    reset();
    pausePage->setVisible(true);
    pausePage->active = true;
    if (uiStack.empty() || uiStack.back() != pausePage) {
        uiStack.push_back(pausePage);
    }
}

void _uiManager::resumeGame()
{
    if (!enabled) {
        return;
    }
    if (!uiStack.empty()) {
        uiStack.pop_back();
    }
    if (requestLockCursor) {
        requestLockCursor();
    }
    open = false;
    reset();
}

void _uiManager::resetGame()
{
    if (!enabled) {
        return;
    }
    if (!uiStack.empty()) {
        uiStack.pop_back();
    }

    reset();
    uiStack.clear();
    resetRequested = true;
}

void _uiManager::winGame()
{
    if (!enabled) {
        return;
    }
    if (requestUnlockCursor) {
        requestUnlockCursor();
    }
    open = true;
    reset();
    winPage->setVisible(true);
    winPage->active = true;
    if (uiStack.empty() || uiStack.back() != winPage) {
        uiStack.push_back(winPage);
    }
}

void _uiManager::loseGame()
{
    if (!enabled) {
        return;
    }
    if (requestUnlockCursor) {
        requestUnlockCursor();
    }
    open = true;
    reset();
    losePage->setVisible(true);
    losePage->active = true;
    if (uiStack.empty() || uiStack.back() != losePage) {
        uiStack.push_back(losePage);
    }
}

void _uiManager::quitGame()
{
    if (!enabled) {
        return;
    }
    if (requestLockCursor) {
        requestLockCursor();
    }
    PostQuitMessage(0); // For new button functionality, buttons want a std::function<void()> so PostQuitMessage can't be called directly, ultimately this is still less bloat that the enum-based behavior, so oh well
}

void _uiManager::processButtons(WPARAM w)
{
    if (!enabled) {
        return;
    }
    for (_uiPage *pg : uiStack) {
        for (auto *btn : pg->getHitButtons(w)) {
            btn->buttonFunc();
        }
    }
}

void _uiManager::processButtons(float x, float y)
{
    if (!enabled) {
        return;
    }
    for (_uiPage *pg : uiStack) {
        for (auto *btn : pg->getHitButtons(x, y)) {
            btn->buttonFunc();
        }
    }
}
