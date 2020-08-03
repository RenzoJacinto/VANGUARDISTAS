#ifndef NIVELSERVIDOR_H_
#define NIVELSERVIDOR_H_

#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Server.h"
#include "Misil.h"

class Server;

class NivelServidor{

    public:

        NivelServidor();
        void iniciarNivel(Server* server, int t_niv);
        void procesar(Server* server, velocidades_t* v);
        bool esValidoReconectar();
        virtual void cargarNivel(Server* server, int cant_jugadores);
        virtual void iniciar_reconexion(int id, Server* server, int socket_id);
        virtual void parallax();
        void cerrar();

        void setNaves(Server* server, int cant_jugadores);

        int obtenerNaveSeguidaRandom(int cant_naves);
        int obtenerNaveSeguidaPonderada();

        static const int TIEMPO_NIVEL_SEGS = 240;

        // FUNCIONES PARA EL ENVIO DE DATA Y RECIBIMIENTO
        velocidades_t* create_velocidad(int id,  char const* descrip, int x, int y);
        posiciones_t* create_posicion(int id,  char const* descrip, int x, int y);
        posicionesR_t* create_posicionR(int id,  char const* descrip, int x, int y, int puntaje, int vida, int energia);

        // FUNCIONES PARA EL CORTE DE NIVEL
        bool jugadoresMuertos();
        bool enemigosMuertos();

        // FUNCIONES PARA EL RECIBIMIENTO DE DATA
        void recibeNone(Server* server, velocidades_t* v);
        bool recibeModoTest(Server* server, velocidades_t* v);
        bool recibePrimerDisparo(Server* server, velocidades_t* v);
        bool recibeNaveEnemiga(Server* server, velocidades_t* v);
        bool recibeNaveJugador(Server* server, velocidades_t* v);
        void recibeJugadorDesconectado(Server* server, velocidades_t* v);

        // FUNCIONES AUXILIARES DE LAS MISMAS
        void moverEnemigos(Server* server, velocidades_t* v);
        void moverMisiles(Server* server, velocidades_t* v);

    protected:
        vector<NaveEnemiga*> enemigos;
        vector<NaveJugador*> jugadores;
        vector<posiciones_t*> pos_iniciales;
        list<Misil*> misiles;
        int cant_enemigos;
        int tiempo_transcurrido;
        int renderizados;
};

#endif
