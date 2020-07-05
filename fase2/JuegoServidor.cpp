#include "JuegoServidor.h"
#include "Server.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Temporizador.h"
#include <sys/socket.h>

JuegoServidor::JuegoServidor(int cant_enemigos, int cant_jugadores, Server* server)
{

    scrollingOffsetBG = 0;
    scrollingOffsetCity = 0;
    tierraInicial = 700;
    scrollingOffsetNube1 = 120;
    scrollingOffsetNube2 = 0;

    for(int i = 0; i<cant_jugadores; i++)
    {
        NaveJugador* nave = new NaveJugador(150*(i+1), 200, i);
        jugadores.push_back(nave);

        posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
        pos->id = i;
        pos->posX = 0;
        pos->posY = 0;
        server->send_all(pos);
        printf("SERVER: se crea nave jugador, id: %d\n", pos->id);
        free(pos);
    }

    for(int i = 0; i<cant_enemigos; i++)
    {
        std::string sprite = "enemigo";
        int enemigo_random = 1 + rand() % 4;
        sprite += std::to_string(enemigo_random);

        // Obtencion de la posicion pos = inf + rand()%(sup+1-inf)
        int y = 50 + rand() % (sdl.getScreenHeight() + 1);
        // SUPONGO EL BORDE DE RESPAWN COMO +/-100
        // CASO ENEMIGOS 1 y 2: sup = 800 + 100
        int x = sdl.getScreenWidth() + rand() % 51;

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
        printf("se crea nave enemiga\n");
    }
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->id = -1;
    server->send_all(pos);
    free(pos);
}

void JuegoServidor::iniciarNivel(int cantidad_enemigos, Server* server, int t_niv){

    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
    Temporizador temporizador;
    temporizador.iniciar();

    float tiempo_por_enemigos = t_niv/cantidad_enemigos;
    double tiempo_nivel = 0;
    int renderizados = 1;

    while( tiempo_nivel < t_niv ) {
        //if(server->cola_esta_vacia()) std::cout<<"ESTA VACIA LA COLA\n";
        while(! server->cola_esta_vacia()){
            void* dato = server->desencolar();
            posiciones_t* pos = procesar((velocidades_t*)dato);
            //std::cout<<"ID: "<< pos->id<<"X: "<<pos->posX<<"Y: "<<pos->posY;
            server->send_all(pos);
            free(pos);
        }
        //printf("encola enemigo\n");
        tiempo_nivel = temporizador.transcurridoEnSegundos();
        if(tiempo_nivel/renderizados > tiempo_por_enemigos && renderizados<cantidad_enemigos) renderizados++;
        velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
        v->id = renderizados+4;
        server->encolar(v);
    }

}

posiciones_t* JuegoServidor::procesar(velocidades_t* v){
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
            enemigos[i]->mover(jugadores);
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

void JuegoServidor::iniciar_reconexion(int id, Server* server, int socket_id)
{
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
    for(posJ = jugadores.begin(); posJ != jugadores.end(); posJ++)
    {
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
    for(posE = enemigos.begin(); posE != enemigos.end(); posE++)
    {
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

void JuegoServidor::parallax()
{

    tierraInicial -= 0.2;

    scrollingOffsetBG -= 0.5;
    if( scrollingOffsetBG < -2048 ) scrollingOffsetBG = 0;

	scrollingOffsetCity -= 5;
    if( scrollingOffsetCity < -2048 ) scrollingOffsetCity = 0;

    scrollingOffsetNube1 -= 8;
    if( scrollingOffsetNube1 < -800 ) scrollingOffsetNube1 = 0;

    scrollingOffsetNube2 -= 10;
    if( scrollingOffsetNube2 < -800 ) scrollingOffsetNube2 = 0;
}
