#ifndef NIVEL_H
#define NIVEL_H

#include "Escenario.h"
#include "TextureW.h"

class Nivel: public Escenario {

    public:

        Nivel();
        void procesar();
        void cerrar();
        void cargarNivel();
        void renderBackground();

};

#endif
