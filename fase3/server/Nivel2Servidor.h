#ifndef NIVEL2SERVIDOR_H_
#define NIVEL2SERVIDOR_H_

#include "Nave.h"
#include "NivelServidor.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Server.h"

class Server;

class Nivel2Servidor: public NivelServidor {

    public:

        Nivel2Servidor();

        void cargarNivel(Server* server, int cant_jugadores);
        void iniciar_reconexion(int id, Server* server, int socket_id);
        void parallax();

    private:
        double scrollingOffsetPlaneta1;
        double scrollingOffsetPlaneta2;
        double scrollingOffsetAsteroides1;
        double scrollingOffsetAsteroides2;
        double scrollingOffsetAsteroides3;

        double anchoBG;
        double anchoAst;
};

#endif
