#ifndef NIVEL1_H
#define NIVEL1_H

#include "Mapa.h"
#include "TextureW.h"

class Nivel1: public Mapa{

    public:

        Nivel1();
        void cargarNivel();
        void cerrar();
        void renderBackground();

    private:
        TextureW gBGTexture;
        TextureW gCiudadTexture;
        TextureW gPlanetaTexture;

        SDL_Rect dataCiudad;
        SDL_Rect dataBG;

        double scrollingOffsetBG;
        double scrollingOffsetCity;
        double tierraInicial;

        const char* sMapaBG;
        const char* sPlaneta;
        const char* sCiudad;
};

#endif

