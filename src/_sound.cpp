#include "_sound.h"

_sound::_sound()
{
    //ctor
}

_sound::~_sound()
{
    engine->drop();
}

void _sound::playMusic(const char* filepath)
{
    if (!engine) {
        return;
    }
    if (!engine->isCurrentlyPlaying(filepath)) {
        engine->play2D(filepath, true);
    }
}

void _sound::playSound(const char* filepath)
{
    if (!engine) {
        return;
    }
    if (!engine->isCurrentlyPlaying(filepath)) {
        engine->play2D(filepath, false, false);
    }
}

void _sound::playSoundDupe(const char* filepath)
{
    if (!engine) {
        return;
    }
    engine->play2D(filepath, false, false);
}

void _sound::stopSound(const char* filepath)
{
    if (!engine) {
        return;
    }
    if (engine->isCurrentlyPlaying(filepath)) {
        engine->play2D(filepath, true, false);
    }
}

void _sound::init()
{
    if (!engine) {
        cerr << "[ERROR] Sound engine did not load\n";
    }
}
