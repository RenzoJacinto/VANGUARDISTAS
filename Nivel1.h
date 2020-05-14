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

    private:
        TextureW gBGTexture;
        TextureW gCiudadTexture;
        TextureW gPlanetaTexture;
        TextureW gNube;

        SDL_Rect dataCiudad;
        SDL_Rect dataBG;
        SDL_Rect dataNube;

        double scrollingOffsetBG;
        double scrollingOffsetCity;
        double tierraInicial;
        double scrollingOffsetNube;

        const char* sMapaBG;
        const char* sPlaneta;
        const char* sCiudad;
        const char* sNube;
};

#endif

