#ifndef NIVEL2H
#define NIVEL2_H

#include "Nivel.h"
#include "TextureW.h"

class Nivel2: public Nivel{
    public:
        Nivel2();
        void cargarNivel();
        void cerrar();
        void renderBackground();

    //private:
        /*TextureW gBGTexture;
        TextureW gCiudadTexture;
        TextureW gPlanetaTexture;*/

};

#endif

