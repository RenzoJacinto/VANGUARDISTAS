#ifndef JUEGOSERVIDOR_H_
#define JUEGOSERVIDOR_H_

#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Server.h"
#include "NivelServidor.h"

class Server;
class NivelServidor;

class JuegoServidor{

    public:

        JuegoServidor(int cant_enemigos, int cant_jugadores, Server* server);
        void iniciarJuego(int cantidad_enemigos, Server* server, int t_niv);
        void iniciar_reconexion(int id, Server* server, int socket_id);
        bool esValidoReconectar();
        int get_nivel_actual();

    private:

        double scrollingOffsetBG;
        double scrollingOffsetCity;
        double tierraInicial;
        double scrollingOffsetNube1;
        double scrollingOffsetNube2;

        vector<NivelServidor*> niveles;

        int nivel_actual;
};

#endif
