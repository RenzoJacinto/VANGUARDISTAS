#ifndef BOTON_INICIAR_H_
#define BOTON_INICIAR_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "TextureW.h"

class BotonIniciar{
    public:
        BotonIniciar();

        bool handleEvent( SDL_Event& e );
        int mouseEvent(SDL_Event& e);
        bool clickOnBoxName(int x, int y);
        bool clickOnBoxPass(int x, int y);

        std::string get_ID();
        std::string get_Pass();
        void inicializar_credenciales();
        void cargarImagenes();

        // Seleccion de boxs
        static const int SELECT_NAME = 0;
        static const int SELECT_PASS = 1;
        static const int NONE_SELECT = -1;

        // Posiciones de las box
        static const int INIT_X_TEXT_NAME = 73;
        static const int FIN_X_TEXT_NAME = 367;

        static const int INIT_X_TEXT_PASS = 428;
        static const int FIN_X_TEXT_PASS = 723;

        static const int INIT_Y_TEXT = 357;
        static const int FIN_Y_TEXT = 388;
        static const int Y_MEDIO_TEXT = 365;

    private:

        std::string id;
        std::string pass;
        TextureW gId;
        TextureW gPass;

        std::string inputText;

        TextureW gSelectName;
        TextureW gSelectPass;

        int box;
};

    #endif // BOTON_INICIAR_H_
