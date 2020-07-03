#ifndef NIVEL_H
#define NIVEL_H

#include <list>
#include "NaveEnemiga.h"
#include "Escenario.h"
#include "TextureW.h"
#include "Server.h"

class Nivel: public Escenario {

    public:

        Nivel();
        vector<NaveEnemiga*> crear_enemigos(Server* sv);
        bool procesar_servidor();
        bool procesar_cliente();
        void finalizar();
        virtual void cerrar();
        virtual void cargarNivel();
        virtual void renderBackground();

        static const int TIEMPO_NIVEL_SEGS = 240;
    protected:
        int cantidad_enemigos;
        TextureW gFinNivel;
        SDL_Rect dataFinNivel;
};

#endif
