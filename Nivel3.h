#ifndef NIVEL3_H
#define NIVEL3_H

#include "Nivel.h"
#include "TextureW.h"

class Nivel3: public Nivel{

    public:
        Nivel3();
        void cargarNivel();
        void cerrar();
        bool renderBackground();

        bool parallax();

    private:
        TextureW gBGTexture;
        TextureW gFondo1Texture;
        TextureW gFondo2Texture;
        TextureW gFondo3Texture;
        TextureW gFondo4Texture;
        TextureW gFondo5Texture;
        TextureW gFondo6Texture;

        TextureW gFinNivel;

        SDL_Rect dataBG;
        SDL_Rect dataFondo1;
        SDL_Rect dataFondo2;
        SDL_Rect dataFondo3;
        SDL_Rect dataFondo4;
        SDL_Rect dataFondo5;
        SDL_Rect dataFondo6;

        double scrollingOffsetBG;

        double scrollingOffsetFondo1;
        double scrollingOffsetFondo2;
        double scrollingOffsetFondo3;
        double scrollingOffsetFondo4;
        double scrollingOffsetFondo5;
        double scrollingOffsetFondo6;

};

#endif
