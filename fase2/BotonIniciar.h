#ifndef BOTON_INICIAR_H_
#define BOTON_INICIAR_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "TextureW.h"

class BotonIniciar
{
    public:
        BotonIniciar();

        void handleEvent( SDL_Event& e );
        int mouseEvent(SDL_Event& e);
        bool clickOnBoxName(int x, int y);
        bool clickOnBoxPass(int x, int y);

        std::string get_inputTxt();

    private:
        TextureW gInputTextIdTexture;
        TextureW gInputTextPassTexture;

        std::string inputStringId;
        std::string inputStringPass;

        std::string inputText;

        int box;

};

    #endif // BOTON_INICIAR_H_
