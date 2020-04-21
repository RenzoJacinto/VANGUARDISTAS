#ifndef BOTON_INICIAR_H_
#define BOTON_INICIAR_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

class BotonIniciar
{
    public:
        BotonIniciar();

        void handleEvent( SDL_Event& e );

        //void render();

};

    #endif // BOTON_INICIAR_H_
