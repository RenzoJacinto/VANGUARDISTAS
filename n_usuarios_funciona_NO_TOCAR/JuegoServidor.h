#ifndef JUEGOSERVIDOR_H_
#define JUEGOSERVIDOR_H_

#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Server.h"

class Server;

class JuegoServidor{

    public:

        JuegoServidor(int cant_enemigos, int cant_jugadores, Server* server);
        void iniciarNivel(int cantidad_enemigos, Server* server, int t_niv);
        posiciones_t* procesar(velocidades_t* v);

    private:
        vector<NaveEnemiga*> enemigos;
        vector<NaveJugador*> jugadores;
};

#endif
