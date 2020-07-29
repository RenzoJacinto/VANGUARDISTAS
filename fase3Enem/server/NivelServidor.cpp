#include "NivelServidor.h"
#include "Temporizador.h"
#include "Server.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Enemigo1.h"
#include "Enemigo2.h"
#include "Enemigo3.h"
#include "Enemigo4.h"
#include "Nave.h"
#include <sys/socket.h>
#include "time_nanoseconds.h"

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

        /*if(! jugadoresSiguenVivos()){
            // Enviar GAME OVER
        }*/

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

        velocidades_t* v = create_velocidad(renderizados+4, "rend", 0, 0);
        server->encolar(v);
    }

    posiciones_t* pos = create_posicion(-1, "fin", 0, 0);
    server->send_all(pos);
    free(pos);

}

posiciones_t* NivelServidor::procesar(Server* server, velocidades_t* v){

    int id = v->id;
    int vx = v->VelX;
    int vy = v->VelY;

    posiciones_t* pos = create_posicion(id, v->descrip, 0, 0);

    if(strcmp(v->descrip, "test") == 0){
        jugadores[id]->set_modeTest();
        server->send_all(pos);
        return pos;
    }

    if(strcmp(v->descrip, "shot0") == 0){
        int danio = 0;
        if(id > 3){
            danio = enemigos[id-4]->get_damage();
        } else{
            danio = jugadores[id]->get_damage();
        }
        Misil* misil = new Misil(vx, vy, id, danio);
        misiles.push_back(misil);
        pos->posX = vx;
        pos->posY = vy;
        server->send_all(pos);
    } else{
        if(id>3){
            for(int i = 0; i < id - 4; i++){
                if(enemigos[i]->isAlive()){
                    int colision_id = enemigos[i]->procesarAccion(jugadores);
                    bool disparo = enemigos[i]->seDisparo();
                    if (disparo){
                        int xMisil = enemigos[i]->getPosX()+enemigos[i]->getRadio();
                        int yMisil = enemigos[i]->getPosY();
                        velocidades_t* vMisil = create_velocidad(i+4, "shot0", xMisil, yMisil);
                        server->encolar(vMisil);
                        enemigos[i]->reiniciarDisparo();

                        //enemigos[i]->setNaveSeguida(obtenerNaveSeguidaPonderada());
                    }
                    if(colision_id != -1){
                        //strcpy(pos->descrip, "colision");
                        strcpy(pos->descrip, "colision");
                        pos->posX = 0;
                        pos->posY = i;
                        pos->id = colision_id;
                        server->send_all(pos);
                    } else{
                        pos->posX = enemigos[i]->getPosX();
                        pos->posY = enemigos[i]->getPosY();
                        pos->id = i+4;
                        strcpy(pos->descrip, enemigos[i]->getImagen());
                        server->send_all(pos);
                    }
                }
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
            if(jugadores[id]->isAlive()){
                jugadores[id]->setVelX(vx);
                jugadores[id]->setVelY(vy);
                int colision_id = jugadores[id]->mover(enemigos);
                if(colision_id != -1){
                    pos->posX = 0;
                    pos->posY = colision_id;
                    pos->id = id;
                    strcpy(pos->descrip, "colision");
                    server->send_all(pos);
                } else {
                    pos->posX = jugadores[id]->getPosX();
                    pos->posY = jugadores[id]->getPosY();
                    server->send_all(pos);
                }
            }
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

        char descrip[5];
        if(server->desconecto(i)) {
            printf("nave %d desconectada\n", i);
            strcpy(descrip, "off");
        }
        else strcpy(descrip, "on");
        posiciones_t* pos = create_posicion(i, descrip, 200, 100*(i+1));
        server->send_all(pos);
        //printf("SERVER: se crea nave jugador, id: %d\n", pos->id);
        free(pos);
    }

    for(int i = 0; i<cant_enemigos; i++){
        std::string sprite = "enemigo";
        int enemigo_random = 1 + randomNumber() % 4;
        sprite += std::to_string(enemigo_random);

        // Obtencion de la posicion pos = inf + rand()%(sup+1-inf)
        int y = 90 + rand() % (SCREEN_HEIGHT + 1);
        // SUPONGO EL BORDE DE RESPAWN COMO +/-100
        // CASO ENEMIGOS 1 y 2: sup = 800 + 100
        int x = SCREEN_WIDTH + rand() % 51;

        // CASO ENEMIGOS 3 y 4: inf = -100
        if(enemigo_random == 4 || enemigo_random == 3) x = -50 + rand() % 51;

        NaveEnemiga* enemigo;
        switch(enemigo_random){
            case 1: enemigo = new Enemigo1(x,y); break;
            case 2: enemigo = new Enemigo2(x,y); break;
            case 3: enemigo = new Enemigo3(x,y); break;
            case 4: enemigo = new Enemigo4(x,y); break;
        }

        //NaveEnemiga* enemigo = new NaveEnemiga(x, y, sprite.c_str(), jugadores);

        enemigos.push_back(enemigo);
        posiciones_t* pos = create_posicion(i+4, enemigo->getImagen(), x ,y);

        server->send_all(pos);
        free(pos);
        //printf("se crea nave enemiga\n");
    }
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->id = -1;
    server->send_all(pos);
    free(pos);
}

velocidades_t* NivelServidor::create_velocidad(int id, const char* descrip, int x, int y){
    velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
    v->VelX = x;
    v->VelY = y;
    strcpy(v->descrip, descrip);
    v->id = id;

    return v;
}

posiciones_t* NivelServidor::create_posicion(int id,  const char* descrip, int x, int y){
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    pos->posX = x;
    pos->posY = y;
    strcpy(pos->descrip, descrip);
    pos->id = id;

    return pos;
}

