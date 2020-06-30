#ifndef NIVEL_CLIENTE_H
#define NIVEL_CLIENTE_H

#include <list>
#include "NaveEnemiga.h"
#include "Nivel.h"
#include "TextureW.h"

class NivelCliente: public Nivel {

    public:

        bool procesar();
        void cerrar();
        void cargarNivel();
        void renderBackground();

};

#endif
