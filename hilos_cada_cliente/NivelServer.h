#ifndef NIVEL_SERVER_H
#define NIVEL_SERVER_H

#include <list>
#include "NaveEnemiga.h"
#include "Nivel.h"
#include "TextureW.h"

class NivelServer: public Nivel {

    public:

        bool procesar();
        void cerrar();
        void cargarNivel();
        void renderBackground();

};

#endif
