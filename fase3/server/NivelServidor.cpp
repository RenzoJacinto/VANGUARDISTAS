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
    renderizados = 1;
}

void NivelServidor::cargarNivel(Server* server, int cant_jugadores){}

void NivelServidor::parallax(){}

void NivelServidor::iniciar_reconexion(int id, Server* server, int socket_id){}

void NivelServidor::iniciarNivel(Server* server, int t_niv){
    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
    Temporizador temporizador;
    temporizador.iniciar();

    float tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cant_enemigos;
    //int renderizados = 1;

    while( tiempo_transcurrido < TIEMPO_NIVEL_SEGS ) {
        //if(server->cola_esta_vacia()) std::cout<<"ESTA VACIA LA COLA\n";
        while(! server->cola_esta_vacia()){
            void* dato = server->desencolar();
            posiciones_t* pos = procesar(server, (velocidades_t*)dato);
            //std::cout<<"ID: "<< pos->id<<"X: "<<pos->posX<<"Y: "<<pos->posY;
            //server->send_all(pos);
            free(pos);
        }
        //printf("encola enemigo\n");
        tiempo_transcurrido = temporizador.transcurridoEnSegundos();
        if(tiempo_transcurrido/renderizados > tiempo_por_enemigos && renderizados<cant_enemigos) renderizados++;
        velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
        strcpy(v->descrip, "rend");
        v->id = renderizados+4;
        server->encolar(v);
    }
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->id = -1;
    server->send_all(pos);
    free(pos);

}

posiciones_t* NivelServidor::procesar(Server* server, velocidades_t* v){
    int id = v->id;
    int vx = v->VelX;
    int vy = v->VelY;
    //printf("SERVER proceso un dato, ID: %d\n", id);
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->id = id;
    strcpy(pos->descrip, v->descrip);
    pos->posX = 0;
    pos->posY = 0;

    if(strcmp(v->descrip, "test") == 0){
        jugadores[id]->set_modeTest();
        server->send_all(pos);
        return pos;
    }

    if(strcmp(v->descrip, "shot0") == 0){
        Misil* misil = new Misil(vx, vy, id);
        misiles.push_back(misil);
        pos->posX = vx;
        pos->posY = vy;
        pos->id = misil->get_id();
        server->send_all(pos);
    } else{
        if(id>3){
            for(int i = 0; i < id - 4; i++){
                enemigos[i]->setJugadores(jugadores);
                int naveSeguida = enemigos[i]->getNaveSeguida();
                enemigos[i]->procesarAccion(jugadores[naveSeguida]);
                bool disparo = enemigos[i]->seDisparo();
                if (disparo){
                    velocidades_t* vMisil = (velocidades_t*) malloc(sizeof(velocidades_t));
                    strcpy(vMisil->descrip, "shot0");
                    vMisil->VelX =enemigos[i]->getPosX()+enemigos[i]->getRadio();
                    vMisil->VelY =enemigos[i]->getPosY();
                    vMisil->id = i+4;
                    server->encolar(vMisil);
                    enemigos[i]->reiniciarDisparo();
                    //enemigos[i]->setNaveSeguida(obtenerNaveSeguidaPonderada());
                }
                pos->posX = enemigos[i]->getPosX();
                pos->posY = enemigos[i]->getPosY();
                pos->id = i+4;
                strcpy(pos->descrip, "ASD");
                server->send_all(pos);
            }
            parallax();
            list<Misil*>::iterator pos_m = misiles.begin();
            while(pos_m != misiles.end()){
                int ok = -1;
                if((*pos_m)->get_id() < 3){
                    ok = (*pos_m)->mover(enemigos, renderizados);
                } else{
                    ok = (*pos_m)->mover(jugadores);
                }

                if(ok == -1){
                    pos_m = misiles.erase(pos_m);
                    //printf("borra misil\n");
                } else if(ok == -2){
                    //printf("no impacto\n");
                    strcpy(pos->descrip, "shot1");
                    pos->id = (*pos_m)->get_id();
                    pos->posX = (*pos_m)->getPosX();
                    pos->posY = (*pos_m)->getPosY();
                    server->send_all(pos);
                    pos_m++;
                } else{
                    //printf("impacto\n");
                    pos->id = ok;
                    if(id > 3){
                        pos->posX = enemigos[ok-4]->getVidaActual();
                    } else{
                        pos->posX = jugadores[ok]->getVidaActual();
                    }
                    pos->posY = (*pos_m)->get_id();
                    strcpy(pos->descrip, "hit");
                    server->send_all(pos);
                    pos_m = misiles.erase(pos_m);
                }

            }

        } else if(strcmp(v->descrip, "off") != 0){
            jugadores[id]->setVelX(vx);
            jugadores[id]->setVelY(vy);
            jugadores[id]->mover(enemigos);
            pos->posX = jugadores[id]->getPosX();
            pos->posY = jugadores[id]->getPosY();
            server->send_all(pos);
        } else if(strcmp(v->descrip, "off") == 0){
            //jugadores[id]->desconectar();
            pos->id = id;
            strcpy(pos->descrip, "off");
            server->send_all(pos);
        }
    }
    strcpy(pos->descrip, v->descrip);
    free(v);
    return pos;
}

bool NivelServidor::esValidoReconectar(){
    return TIEMPO_NIVEL_SEGS - tiempo_transcurrido > 0;
}

void NivelServidor::setNaves(Server* server, int cant_jugadores){
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
        int y = 90 + rand() % (SCREEN_HEIGHT + 1);
        // SUPONGO EL BORDE DE RESPAWN COMO +/-100
        // CASO ENEMIGOS 1 y 2: sup = 800 + 100
        int x = SCREEN_WIDTH + rand() % 51;

        // CASO ENEMIGOS 3 y 4: inf = -100
        if(enemigo_random == 4 || enemigo_random == 3) x = -50 + rand() % 51;

        NaveEnemiga* enemigo = new NaveEnemiga(x, y, sprite.c_str());

        int naveSeguida = obtenerNaveSeguidaRandom(cant_jugadores);
        enemigo->setNaveSeguida(naveSeguida);
        enemigo->setJugadores(jugadores);

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

int NivelServidor::obtenerNaveSeguidaRandom(int cant_naves){
    return rand() % cant_naves;
}

int NivelServidor::obtenerNaveSeguidaPonderada(){
    int cant_jug = jugadores.size();
    int ponderacion[cant_jug];
    for(int i=0; i<cant_jug; i++){
        int score = jugadores[i]->getScore();
        int vida = jugadores[i]->getVidas();

        ponderacion[i] = score / vida;
    }
    int idx = 0;
    int min_pond = ponderacion[idx];
    for(int j=1; j<cant_jug; j++){
        if(ponderacion[j] < min_pond){
            min_pond = ponderacion[j];
            idx = j;
        }
    }
    return idx;
}

