#ifndef NIVEL_H
#define NIVEL_H

#include "Escenario.h"
#include "TextureW.h"

class Nivel: public Escenario {

    public:

        Nivel();
        void procesar();
        virtual void cerrar();
        virtual void cargarNivel();
        virtual void renderBackground();

};

#endif
