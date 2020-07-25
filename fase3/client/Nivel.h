#ifndef NIVEL_H
#define NIVEL_H

#include <list>
#include "NaveEnemiga.h"
#include "Escenario.h"
#include "TextureW.h"
#include "Client.h"
#include "Misil.h"

class Client;

class Nivel: public Escenario {

    public:

        Nivel();
        bool iniciarNivel(Client* client);
        void procesar(posiciones_t* pos);
        void finalizar();
        void renderizar(int id_nave);
        void aumentarRenderizados(int i);
        void setNaves(Client* client);
        void renderEnemigos();
        void renderBooms();

        virtual void cerrar();
        virtual void cargarNivel(Client* client);
        virtual void reconectar(Client* client);
        virtual void renderBackground();
        virtual void parallax();


        void renderPuntajes();
        //static const int TIEMPO_NIVEL_SEGS = 240;

    protected:

        int renderizados;

        vector<NaveEnemiga*> enemigos;
        vector<NaveJugador*> jugadores;
        list<Misil*> misiles;

        int cantidad_enemigos;
        TextureW gFinNivel;
        SDL_Rect dataFinNivel;

        TextureW puntajesBoxTexture;
        TextureW lifeTexture;

        Mix_Music *gMusic;
        Mix_Chunk *explosion;
        Mix_Chunk *shotFX;
        Mix_Chunk *hitReceiveFX;
        Mix_Chunk *lifeDownFX;

};

#endif
