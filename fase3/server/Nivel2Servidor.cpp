#include "Nivel2Servidor.h"
#include "Server.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Temporizador.h"
#include <sys/socket.h>

class Server;

Nivel2Servidor::Nivel2Servidor(int cant_jugadores){
    cant_enemigos = json.get_cantidad_enemigo("nivel2");
}

void Nivel2Servidor::cargarNivel(Server* server, int cant_enemigos, int cant_jugadores){

    scrollingOffsetPlaneta1 = 300;
    scrollingOffsetPlaneta2 = 1000;

    scrollingOffsetAsteroides1 = 0;
    scrollingOffsetAsteroides2 = 0;
    scrollingOffsetAsteroides3 = 0;

    anchoBG = 800;
    anchoAst = 872;

    setNaves(server, cant_enemigos, cant_jugadores);
}

void Nivel2Servidor::iniciar_reconexion(int id, Server* server, int socket_id){
    logger.info("Iniciando reconexiones");
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->posX = (int)scrollingOffsetPlaneta1;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetPlaneta2;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetAsteroides1;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetAsteroides2;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetAsteroides3;
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

void Nivel2Servidor::parallax(){
    scrollingOffsetPlaneta2 -= 0.25;

    scrollingOffsetPlaneta1 -= 0.20;

	scrollingOffsetAsteroides1 -= 10;
    if( scrollingOffsetAsteroides1 < -anchoBG ) scrollingOffsetAsteroides1 = 0;

    scrollingOffsetAsteroides2 -= 5;
    if( scrollingOffsetAsteroides2 < -anchoAst ) scrollingOffsetAsteroides2 = 0;

    scrollingOffsetAsteroides3 -= 2.5;
    if( scrollingOffsetAsteroides3 < -anchoAst ) scrollingOffsetAsteroides3 = 0;
}
