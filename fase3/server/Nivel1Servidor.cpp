#include "Nivel1Servidor.h"
#include "Server.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Temporizador.h"
#include <sys/socket.h>

class Server;

Nivel1Servidor::Nivel1Servidor(int cant_jugadores){
    cant_enemigos = json.get_cantidad_enemigo("nivel1");
}

void Nivel1Servidor::cargarNivel(Server* server, int cantidad_enemigos, int cant_jugadores){

    scrollingOffsetBG = 0;
    scrollingOffsetCity = 0;
    tierraInicial = 700;
    scrollingOffsetNube1 = 120;
    scrollingOffsetNube2 = 0;

    anchoBG = 2048;
    anchoNube = 800;

    setNaves(server, cant_enemigos, cant_jugadores);
}
void Nivel1Servidor::iniciar_reconexion(int id, Server* server, int socket_id){
    logger.info("Iniciando reconexiones");
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->posX = (int)scrollingOffsetBG;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetCity;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)tierraInicial;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetNube1;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetNube2;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);

    int i = 0;
    vector<NaveJugador*>::iterator posJ;
    for(posJ = jugadores.begin(); posJ != jugadores.end(); posJ++){
        pos->posX = (*posJ)->getPosX();
        pos->posY = (*posJ)->getPosY();
        pos->id = i;
        pos->descrip[0] = 0;
        if(server->desconecto(i) && i != id) strncat(pos->descrip, "off", 5);
        else strncat(pos->descrip, "on", 5);

        send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
        i++;
    }

    i = 4;
    vector<NaveEnemiga*>::iterator posE;
    for(posE = enemigos.begin(); posE != enemigos.end(); posE++){
        pos->posX = (*posE)->getPosX();
        pos->posY = (*posE)->getPosY();
        pos->id = i;
        pos->descrip[0] = 0;
        strncat(pos->descrip, (*posE)->getClave(), 15);

        send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
        i++;
    }
    pos->id = -1;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    free(pos);
}

void Nivel1Servidor::parallax(){
   tierraInicial -= 0.2;

    scrollingOffsetBG -= 0.5;
    if( scrollingOffsetBG < -anchoBG ) scrollingOffsetBG = 0;

	scrollingOffsetCity -= 5;
    if( scrollingOffsetCity < -anchoBG ) scrollingOffsetCity = 0;

    scrollingOffsetNube1 -= 8;
    if( scrollingOffsetNube1 < -anchoNube ) scrollingOffsetNube1 = 0;

    scrollingOffsetNube2 -= 10;
    if( scrollingOffsetNube2 < -anchoNube ) scrollingOffsetNube2 = 0;
}
