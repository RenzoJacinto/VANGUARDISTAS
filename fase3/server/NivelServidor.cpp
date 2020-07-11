#include "NivelServidor.h"
#include "Temporizador.h"
#include "Server.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Nave.h"
#include <sys/socket.h>

class Server;

NivelServidor::NivelServidor(){
    tiempo_transcurrido = 0;
}

void NivelServidor::cargarNivel(Server* server, int cant_enemigos, int cant_jugadores){}

void NivelServidor::parallax(){}

void NivelServidor::iniciar_reconexion(int id, Server* server, int socket_id){}

void NivelServidor::iniciarNivel(int cantidad_enemigos, Server* server, int t_niv){
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

posiciones_t* NivelServidor::procesar(velocidades_t* v){
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
    } else if(strcmp(v->descrip, "off") != 0){
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

bool NivelServidor::esValidoReconectar(){
    return TIEMPO_NIVEL_SEGS - tiempo_transcurrido > 0;
}

void NivelServidor::setNaves(Server* server, int cant_enemigos, int cant_jugadores){
    for(int i = 0; i<cant_jugadores; i++){
        NaveJugador* nave = new NaveJugador(200, 100*(i+1), i);
        jugadores.push_back(nave);

        posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
        pos->id = i;
        pos->posX = 200;
        pos->posY = 100*(i+1);
        if(server->desconecto(i)) {
            printf("nave %d desconectada\n", i);
            strcpy(pos->descrip, "off");
        }
        else strcpy(pos->descrip, "on");
        server->send_all(pos);
        //printf("SERVER: se crea nave jugador, id: %d\n", pos->id);
        free(pos);
    }

    for(int i = 0; i<cant_enemigos; i++){
        std::string sprite = "enemigo";
        int enemigo_random = 1 + rand() % 4;
        sprite += std::to_string(enemigo_random);

        // Obtencion de la posicion pos = inf + rand()%(sup+1-inf)
        int y = 50 + rand() % (SCREEN_HEIGHT + 1);
        // SUPONGO EL BORDE DE RESPAWN COMO +/-100
        // CASO ENEMIGOS 1 y 2: sup = 800 + 100
        int x = SCREEN_WIDTH + rand() % 51;

        // CASO ENEMIGOS 3 y 4: inf = -100
        if(enemigo_random == 4 || enemigo_random == 3) x = -50 + rand() % 51;

        NaveEnemiga* enemigo = new NaveEnemiga(x, y, sprite.c_str());

        enemigos.push_back(enemigo);

        posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
        pos->posX = x;
        pos->posY = y;
        pos->descrip[0] = 0;
        strncat(pos->descrip, sprite.c_str(), 14);
        pos->id = i+4;
        server->send_all(pos);
        free(pos);
        //printf("se crea nave enemiga\n");
    }
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->id = -1;
    server->send_all(pos);
    free(pos);
}

