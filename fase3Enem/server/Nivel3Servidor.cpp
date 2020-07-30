#include "Nivel3Servidor.h"
#include "Server.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Temporizador.h"
#include <sys/socket.h>

class Server;

Nivel3Servidor::Nivel3Servidor(){
    cant_enemigos = json.get_cantidad_enemigo("nivel3");
}

void Nivel3Servidor::cargarNivel(Server* server, int cant_jugadores){

    scrollingOffsetBG = 0;
    scrollingOffsetFondo1 = 0;
    scrollingOffsetFondo2 = 0;
    scrollingOffsetFondo3 = 0;
    scrollingOffsetFondo4 = 0;
    scrollingOffsetFondo5 = 0;
    scrollingOffsetFondo6 = 0;

    anchoBG = 800;

    setNaves(server, cant_jugadores);
}

void Nivel3Servidor::iniciar_reconexion(int id, Server* server, int socket_id){
    logger.info("Iniciando reconexiones");
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->posX = (int)scrollingOffsetBG;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetFondo1;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetFondo2;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetFondo3;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetFondo4;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetFondo5;
    send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
    pos->posX = (int)scrollingOffsetFondo6;
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

        int bytes_written = 0;
        int total_bytes_written = 0;
        int total_size = sizeof(posiciones_t);
        while(total_size>total_bytes_written){
            bytes_written = send(server->get_socket(socket_id), pos, sizeof(posiciones_t), MSG_NOSIGNAL);
            if(bytes_written<=0) return;
            total_bytes_written+=bytes_written;
        }

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

void Nivel3Servidor::parallax(){
    scrollingOffsetBG -= 0.1;
    if( scrollingOffsetBG < -anchoBG ) scrollingOffsetBG = 250;

    scrollingOffsetFondo1 -= 15;
    if( scrollingOffsetFondo1 < -anchoBG ) scrollingOffsetFondo1 = 0;

    scrollingOffsetFondo2 -= 13.5;
    if( scrollingOffsetFondo2 < -anchoBG ) scrollingOffsetFondo2 = 0;

    scrollingOffsetFondo3 -= 12;
    if( scrollingOffsetFondo3 < -anchoBG ) scrollingOffsetFondo3 = 0;

    scrollingOffsetFondo4 -= 10.5;
    if( scrollingOffsetFondo4 < -anchoBG ) scrollingOffsetFondo4 = 0;

    scrollingOffsetFondo5 -= 9;
    if( scrollingOffsetFondo5 < -anchoBG ) scrollingOffsetFondo5 = 0;

    scrollingOffsetFondo6 -= 7.5;
    if( scrollingOffsetFondo6 < -anchoBG ) scrollingOffsetFondo6 = 0;
}