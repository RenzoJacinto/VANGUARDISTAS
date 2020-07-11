#ifndef NIVEL_H
#define NIVEL_H

#include <list>
#include "NaveEnemiga.h"
#include "Escenario.h"
#include "TextureW.h"
#include "Client.h"

class Client;

class Nivel: public Escenario {

    public:

        Nivel();
        bool iniciarNivel(Client* client);
        void procesar(posiciones_t* pos);
        void finalizar();
        void renderizar();
        void aumentarRenderizados(int i);
        void setNaves(Client* client);

        virtual void cerrar();
        virtual void cargarNivel(Client* client);
        virtual void reconectar(Client* client);
        virtual void renderBackground();
        virtual void parallax();


        //static const int TIEMPO_NIVEL_SEGS = 240;

    protected:
        int renderizados;
        vector<NaveEnemiga*> enemigos;
        vector<NaveJugador*> jugadores;
        int cantidad_enemigos;
        TextureW gFinNivel;
        SDL_Rect dataFinNivel;

        Mix_Music *gMusic;
};

#endif
