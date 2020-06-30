#ifndef NIVEL_H
#define NIVEL_H

#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Server.h"
#include "Client.h"
#include "Estado.h"
#include "Temporizador.h"
#include "global.h"
#include "Escenario.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

class Nivel: public Escenario {

    public:

        Nivel();
        vector<NaveEnemiga*> crear_enemigos();
        virtual bool procesar();
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
