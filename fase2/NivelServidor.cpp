#include "NivelServidor.h"
#include "Temporizador.h"
#include "Server.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Nave.h"
#include <sys/socket.h>

class Server;

NivelServidor::NivelServidor()
{
    tiempo_transcurrido = 0;
}

void NivelServidor::cargarNivel(Server* server, int cant_enemigos, int cant_jugadores){}

void NivelServidor::parallax(){}

void NivelServidor::iniciar_reconexion(int id, Server* server, int socket_id){}

void NivelServidor::iniciarNivel(int cantidad_enemigos, Server* server, int t_niv)
{
    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
    Temporizador temporizador;
    temporizador.iniciar();

    float tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cant_enemigos;
    int renderizados = 1;

    while( tiempo_transcurrido < TIEMPO_NIVEL_SEGS ) {
        //if(server->cola_esta_vacia()) std::cout<<"ESTA VACIA LA COLA\n";
        while(! server->cola_esta_vacia()){
            void* dato = server->desencolar();
            posiciones_t* pos = procesar((velocidades_t*)dato);
            //std::cout<<"ID: "<< pos->id<<"X: "<<pos->posX<<"Y: "<<pos->posY;
            server->send_all(pos);
            free(pos);
        }
        //printf("encola enemigo\n");
        tiempo_transcurrido = temporizador.transcurridoEnSegundos();
        if(tiempo_transcurrido/renderizados > tiempo_por_enemigos && renderizados<cantidad_enemigos) renderizados++;
        velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
        v->id = renderizados+4;
        server->encolar(v);
    }
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->id = -1;
    server->send_all(pos);
    free(pos);

}

posiciones_t* NivelServidor::procesar(velocidades_t* v)
{
    int id = v->id;
    int vx = v->VelX;
    int vy = v->VelY;
    //printf("SERVER proceso un dato, ID: %d\n", id);
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->id = id;
    pos->posX = 0;
    pos->posY = 0;
    if(id>3){
        for(int i = 0; i < id - 4; i++){
            enemigos[i]->mover(jugadores[0]);
        }
        parallax();
    }
    else if(strcmp(v->descrip, "off") != 0)
    {
        jugadores[id]->setVelX(vx);
        jugadores[id]->setVelY(vy);
        jugadores[id]->mover(enemigos);
        pos->posX = jugadores[id]->getPosX();
        pos->posY = jugadores[id]->getPosY();

    }
    pos->descrip[0] = 0;
    strncat(pos->descrip, v->descrip, 5);
    free(v);
    return pos;
}

bool NivelServidor::esValidoReconectar()
{
    return TIEMPO_NIVEL_SEGS - tiempo_transcurrido > 5;
}

