#include "Nivel2Servidor.h"
#include "Server.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Temporizador.h"
#include <sys/socket.h>

class Server;

Nivel2Servidor::Nivel2Servidor()
{
    cant_enemigos = json.get_cantidad_enemigo("nivel2");
}

void Nivel2Servidor::cargarNivel(Server* server, int cant_enemigos, int cant_jugadores)
{
    scrollingOffsetPlaneta1 = 300;
    scrollingOffsetPlaneta2 = 1000;

    scrollingOffsetAsteroides1 = 0;
    scrollingOffsetAsteroides2 = 0;
    scrollingOffsetAsteroides3 = 0;

    for(int i = 0; i<cant_jugadores; i++)
    {
        NaveJugador* nave = new NaveJugador(200, 150*(i+1), i);
        jugadores.push_back(nave);

        posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
        pos->id = i;
        pos->posX = 200;
        pos->posY = 150*(i+1);
        if(server->desconecto(i)) strncat(pos->descrip, "off", 5);
        else strncat(pos->descrip, "on", 5);
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

void Nivel2Servidor::iniciar_reconexion(int id, Server* server, int socket_id)
{
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

void Nivel2Servidor::parallax()
{
    scrollingOffsetPlaneta2 -= 0.25;

    scrollingOffsetPlaneta1 -= 0.20;

	scrollingOffsetAsteroides1 -= 10;
    if( scrollingOffsetAsteroides1 < -800 ) scrollingOffsetAsteroides1 = 0;

    scrollingOffsetAsteroides2 -= 5;
    if( scrollingOffsetAsteroides2 < -872 ) scrollingOffsetAsteroides2 = 0;

    scrollingOffsetAsteroides3 -= 2.5;
    if( scrollingOffsetAsteroides3 < -872 ) scrollingOffsetAsteroides3 = 0;
}
