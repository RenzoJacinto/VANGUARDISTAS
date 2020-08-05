#include "Nivel2Servidor.h"
#include "Server.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Temporizador.h"
#include <sys/socket.h>

class Server;

Nivel2Servidor::Nivel2Servidor(){
    cant_enemigos = json.get_cantidad_enemigo("nivel2");
}

void Nivel2Servidor::cargarNivel(Server* server, int cant_jugadores){

    scrollingOffsetPlaneta1 = 300;
    scrollingOffsetPlaneta2 = 1000;

    scrollingOffsetAsteroides1 = 0;
    scrollingOffsetAsteroides2 = 0;
    scrollingOffsetAsteroides3 = 0;

    anchoBG = 800;
    anchoAst = 872;

    setNaves(server, cant_jugadores);

    // fin set
    posiciones_t* pos_f = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos_f->id = -1;
    server->send_all(pos_f);
    free(pos_f);
}

void Nivel2Servidor::iniciar_reconexion(int id, Server* server, int socket_id){
    logger.info("Iniciando reconexiones");
    posicionesR_t* pos = (posicionesR_t*)malloc(sizeof(posicionesR_t));
    pos->posX = (int)scrollingOffsetPlaneta1;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetPlaneta2;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetAsteroides1;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetAsteroides2;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetAsteroides3;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);

    int i = 0;
    vector<NaveJugador*>::iterator posJ;
    for(posJ = jugadores.begin(); posJ != jugadores.end(); posJ++){
        pos->posX = (*posJ)->getPosX();
        pos->posY = (*posJ)->getPosY();
        pos->id = i;
        if(server->desconecto(i) && i != id) strcpy(pos->descrip, "off");
        else if ((*posJ)->inModeTest()) strcpy(pos->descrip, "test");
        else strcpy(pos->descrip, "on");
        pos->score = (*posJ)->getScore();
        pos->vidas = (*posJ)->getVidas();
        pos->energiaActual = (*posJ)->getVidaActual();
        send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
        i++;
    }

    i = 4;
    vector<NaveEnemiga*>::iterator posE;
    for(posE = enemigos.begin(); posE != enemigos.end(); posE++){
        pos->posX = (*posE)->getPosX();
        pos->posY = (*posE)->getPosY();
        pos->id = i;
        pos->descrip[0] = 0;
        strcpy(pos->descrip, (*posE)->getClave());
        pos->vidas = (*posE)->getVidas();
        pos->energiaActual = (*posE)->getVidaActual();
        send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
        i++;
    }
    pos->id = -1;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    free(pos);
}

void Nivel2Servidor::parallax(){
    scrollingOffsetPlaneta2 -= 0.25;

    scrollingOffsetPlaneta1 -= 0.20;

	scrollingOffsetAsteroides1 -= 5;
    if( scrollingOffsetAsteroides1 < -anchoBG ) scrollingOffsetAsteroides1 = 0;

    scrollingOffsetAsteroides2 -= 3;
    if( scrollingOffsetAsteroides2 < -anchoAst ) scrollingOffsetAsteroides2 = 0;

    scrollingOffsetAsteroides3 -= 2;
    if( scrollingOffsetAsteroides3 < -anchoAst ) scrollingOffsetAsteroides3 = 0;
}
