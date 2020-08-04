#include "Nivel1Servidor.h"
#include "Server.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Temporizador.h"
#include <sys/socket.h>

class Server;

Nivel1Servidor::Nivel1Servidor(){
    cant_enemigos = json.get_cantidad_enemigo("nivel1");
}

void Nivel1Servidor::cargarNivel(Server* server, int cant_jugadores){

    scrollingOffsetBG = 0;
    scrollingOffsetCity = 0;
    tierraInicial = 700;
    scrollingOffsetNube1 = 60;
    scrollingOffsetNube2 = 0;

    anchoBG = 2048;
    anchoNube = 800;

    setNaves(server, cant_jugadores);

    // fin set
    posiciones_t* pos_f = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos_f->id = -1;
    server->send_all(pos_f);
    free(pos_f);
}
void Nivel1Servidor::iniciar_reconexion(int id, Server* server, int socket_id){
    logger.info("Iniciando reconexiones");
    printf("a\n");
    posicionesR_t* pos = (posicionesR_t*)malloc(sizeof(posicionesR_t));
    pos->posX = (int)scrollingOffsetBG;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetCity;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    pos->posX = (int)tierraInicial;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetNube1;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetNube2;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    printf("b\n");
    int i = 0;
    vector<NaveJugador*>::iterator posJ;
    for(posJ = jugadores.begin(); posJ != jugadores.end(); posJ++){
        pos->posX = (*posJ)->getPosX();
        pos->posY = (*posJ)->getPosY();
        pos->id = i;
        if(server->desconecto(i) && i != id) strcpy(pos->descrip, "off");
        else strcpy(pos->descrip, "on");
        printf("score = %d\n", (*posJ)->getScore());
        pos->score = (*posJ)->getScore();
        printf("vidas = %d\n", (*posJ)->getVidas());
        pos->vidas = (*posJ)->getVidas();
        printf("energia Actual = %d\n", (*posJ)->getVidaActual());
        pos->energiaActual = (*posJ)->getVidaActual();
        send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
        i++;
    }
    printf("c\n");
    i = 4;
    vector<NaveEnemiga*>::iterator posE;
    for(posE = enemigos.begin(); posE != enemigos.end(); posE++){
        pos->posX = (*posE)->getPosX();
        pos->posY = (*posE)->getPosY();
        pos->id = i;
        strcpy(pos->descrip, (*posE)->getClave());
        pos->vidas = (*posE)->getVidas();
        pos->energiaActual = (*posE)->getVidaActual();
        send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
        i++;
    }
    printf("d\n");
    pos->id = -1;
    send(server->get_socket(socket_id), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    free(pos);
}

void Nivel1Servidor::parallax(){
   tierraInicial -= 0.2;

    scrollingOffsetBG -= 0.075;
    if( scrollingOffsetBG < -anchoBG ) scrollingOffsetBG = 0;

	scrollingOffsetCity -= 3;
    if( scrollingOffsetCity < -anchoBG ) scrollingOffsetCity = 0;

    scrollingOffsetNube1 -= 5;
    if( scrollingOffsetNube1 < -anchoNube ) scrollingOffsetNube1 = 0;

    scrollingOffsetNube2 -= 7;
    if( scrollingOffsetNube2 < -anchoNube ) scrollingOffsetNube2 = 0;
}
