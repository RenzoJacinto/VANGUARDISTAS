#ifndef MAPA_H
#define MAPA_H

#include "Escenario.h"
#include "TextureW.h"

class Mapa: public Escenario
{
    public:

        Mapa();
        void procesar();
        void cerrar();

    private:

        TextureW gBGTexture;
        TextureW gCiudadTexture;
        TextureW gPlanetaTexture;

};

#endif
