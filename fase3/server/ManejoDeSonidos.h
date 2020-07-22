#ifndef MANEJODESONIDOS_H
#define MANEJODESONIDOS_H

#include "ManejoDeSDL.h"


class ManejoDeSonidos{

    public:
        ManejoDeSonidos();

        Mix_Music* loadMusic(const char* file);
        void playMusic(Mix_Music* music);
        void pauseMusic(Mix_Music* music);
        void haltMusic();
        void freeMusic(Mix_Music* music);
        void stopMusic();

        Mix_Chunk* loadEffect(const char* file);
        void playEffect(Mix_Chunk* effect);
        void freeEffect(Mix_Chunk* effect);
        void pauseEffects();

        static const int PAUSED = 1;
        static const int NO_PLAYING = 0;
    private:
        bool effectsOff;
        bool musicOff;

};

#endif
