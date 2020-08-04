#ifndef NIVEL1_H
#define NIVEL1_H

#include "Nivel.h"
#include "TextureW.h"
#include "Client.h"

class Client;

class Nivel1: public Nivel{

    public:
        Nivel1();
        bool cargarNivel(Client* client);
        void cerrar();
        void renderBackground();
        void reconectar(Client* client);

        void parallax();

    private:

        TextureW gBGTexture;
        TextureW gCiudadTexture;
        TextureW gPlanetaTexture;
        TextureW gNube1;
        TextureW gNube2;

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

