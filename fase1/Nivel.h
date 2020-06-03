#ifndef NIVEL_H
#define NIVEL_H

#include <list>
#include "NaveEnemiga.h"
#include "Escenario.h"
#include "TextureW.h"

class Nivel: public Escenario {

    public:

        Nivel();
        vector<NaveEnemiga*> crear_enemigos();
        bool procesar();
        void finalizar();
        virtual void cerrar();
        virtual void cargarNivel();
        virtual void renderBackground();

        static const int TIEMPO_NIVEL_SEGS = 10;
    protected:
        int cantidad_enemigos;
        TextureW gFinNivel;
        SDL_Rect dataFinNivel;
};

#endif
