#ifndef NIVEL1SERVIDOR_H_
#define NIVEL1SERVIDOR_H_

#include "Nave.h"
#include "NivelServidor.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Server.h"

class Server;

class Nivel1Servidor: public NivelServidor {

    public:

        Nivel1Servidor();
        void cargarNivel(Server* server, int cantidad_enemigos, int cant_jugadores);
        void iniciar_reconexion(int id, Server* server, int socket_id);
        void parallax();

    private:
        double scrollingOffsetBG;

        double scrollingOffsetCity;
        double tierraInicial;
        double scrollingOffsetNube1;
        double scrollingOffsetNube2;
};

#endif
