#ifndef NIVEL1_H
#define NIVEL1_H

#include "Nivel.h"
#include "TextureW.h"

class Nivel1: public Nivel{

    public:
        Nivel1();
        void cargarNivel();
        void cerrar();
        void renderBackground();

        void parallax();

    private:
        TextureW gBGTexture;
        TextureW gCiudadTexture;
        TextureW gPlanetaTexture;
        TextureW gNube1;
        TextureW gNube2;

        //TextureW gFinNivel;

        SDL_Rect dataCiudad;
        SDL_Rect dataBG;
        SDL_Rect dataNube1;
        SDL_Rect dataNube2;

        double scrollingOffsetBG;

        double scrollingOffsetCity;
        double tierraInicial;
        double scrollingOffsetNube1;
        double scrollingOffsetNube2;
};

#endif

