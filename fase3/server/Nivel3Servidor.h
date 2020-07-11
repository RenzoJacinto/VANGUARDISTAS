#ifndef NIVEL3SERVIDOR_H_
#define NIVEL3SERVIDOR_H_

#include "Nave.h"
#include "NivelServidor.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Server.h"

class Server;

class Nivel3Servidor: public NivelServidor {

    public:

        Nivel3Servidor(int cant_jugadores);
        void cargarNivel(Server* server, int cantidad_enemigos, int cant_jugadores);
        void iniciar_reconexion(int id, Server* server, int socket_id);
        void parallax();

    private:
        double scrollingOffsetBG;

        double scrollingOffsetFondo1;
        double scrollingOffsetFondo2;
        double scrollingOffsetFondo3;
        double scrollingOffsetFondo4;
        double scrollingOffsetFondo5;
        double scrollingOffsetFondo6;

        double anchoBG;
};

#endif
