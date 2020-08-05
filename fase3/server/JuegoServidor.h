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

        JuegoServidor(int cant_jugadores, Server* server);
        void iniciarJuego(Server* server, int t_niv);
        void iniciar_reconexion(int id, Server* server, int socket_id);
        bool esValidoReconectar();
        int get_nivel_actual();
        void desconectarUser(int id);

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
