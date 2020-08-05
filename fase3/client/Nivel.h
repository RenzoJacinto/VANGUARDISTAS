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
        void borrarNaves();
        bool iniciarNivel(Client* client);
        void procesar(posiciones_t* pos);
        void finalizar();
        void renderizar(int id_nave);
        void aumentarRenderizados(int i);
        bool setNaves(Client* client);
        void renderEnemigos();
        void renderBooms();
        void renderGameOver();

        void freeSounds();

        virtual void cerrar();
        virtual bool cargarNivel(Client* client);
        virtual void reconectar(Client* client);
        virtual void renderBackground();
        virtual void parallax();

        velocidades_t* create_velocidad(int id, char const* descrip, int x, int y);

        void renderPuntajes();
        //static const int TIEMPO_NIVEL_SEGS = 240;

        // FUNCIONES PARA PROCESAR
        bool recibeColision(posiciones_t* pos);
        bool recibeModoTest(posiciones_t* pos);
        bool recibeDisparo(posiciones_t* pos);
        bool recibeHit(posiciones_t* pos);
        bool recibeParallax(posiciones_t* pos);
        void moverNaves(posiciones_t* pos);

        // FUNCIONES AUX ENVIAR TECLEO
        void enviarTecleo(int tecleo, NaveJugador* jugador, Client* client, int id_nave);

        void recibeScoresDelNivel(Client* client);

        // tecleos
        static const int MODO_TEST = -1;
        static const int MISIL = 0;
        static const int NONE = 1;

    protected:
        int renderGO;
        int renderizados;

        vector<int> score_nivel;

        vector<NaveEnemiga*> enemigos;
        vector<NaveJugador*> jugadores;
        list<Misil*> misiles;

        int cantidad_enemigos;
        TextureW gFinNivel;
        SDL_Rect dataFinNivel;

        TextureW puntajesBoxTexture;
        TextureW lifeTexture;
        TextureW deathUserTexture;

        Mix_Music *gMusic;

        Mix_Chunk *explosion;
        Mix_Chunk *shotFX;
        Mix_Chunk *hitReceiveFX;
        Mix_Chunk *lifeDownFX;
        Mix_Chunk *gameOverFX;
        Mix_Chunk *levelUpFX;

};

#endif
