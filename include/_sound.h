#ifndef _SOUND_H
#define _SOUND_H

#include <_common.h>
#include <SNDS/irrKlang.h>

using namespace irrklang;

class _sound
{
    public:
        _sound();
        virtual ~_sound();

        ISoundEngine *engine = createIrrKlangDevice();

        void playMusic(const char* filepath);
        void playSound(const char* filepath);
        void playSoundDupe(const char* filepath);
        void stopSound(const char* filepath);
        void init();

    protected:

    private:
};

#endif // _SOUND_H
