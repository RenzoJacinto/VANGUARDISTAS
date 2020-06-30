#ifndef NIVEL_H
#define NIVEL_H

#include <list>
#include "NaveEnemiga.h"
#include "Escenario.h"
#include "TextureW.h"
#include "ColaMultihilo.h"
#include "typesmsj.h"

class Nivel: public Escenario {

    public:

        Nivel();
        vector<NaveEnemiga*> crear_enemigos(ColaMultihilo* cola);

        void finalizar();
        virtual void cerrar();
        virtual void cargarNivel();
        virtual void renderBackground();

        bool procesarClient(position_t* pos);
        bool procesarServer(ColaMultihilo* cola, int max_users);

        static const int TIEMPO_NIVEL_SEGS = 240;
    protected:
        int cantidad_enemigos;
        TextureW gFinNivel;
        SDL_Rect dataFinNivel;
};

#endif
