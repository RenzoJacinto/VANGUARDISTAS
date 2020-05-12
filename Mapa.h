#ifndef MAPA_H
#define MAPA_H

#include "Escenario.h"
#include "TextureW.h"


class Mapa: public Escenario{
    public:
        Mapa();
        void procesar();
        virtual void cargarNivel();
        virtual void cerrar();
        virtual void renderBackground();

};

#endif
