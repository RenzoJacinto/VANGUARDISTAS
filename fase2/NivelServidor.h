#ifndef NIVELSERVIDOR_H_
#define NIVELSERVIDOR_H_

#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Server.h"

class Server;

class NivelServidor{

    public:

        NivelServidor();
        void iniciarNivel(int cantidad_enemigos, Server* server, int t_niv);
        posiciones_t* procesar(velocidades_t* v);
        bool esValidoReconectar();
        virtual void cargarNivel(Server* server, int cant_enemigos, int cant_jugadores);
        virtual void iniciar_reconexion(int id, Server* server, int socket_id);
        virtual void parallax();

        static const int TIEMPO_NIVEL_SEGS = 240;
    protected:
        vector<NaveEnemiga*> enemigos;
        vector<NaveJugador*> jugadores;
        int cant_enemigos;
        int tiempo_transcurrido;
};

#endif
