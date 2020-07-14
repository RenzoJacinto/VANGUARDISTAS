#include "Nivel.h"
#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Client.h"
#include "global.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

Nivel::Nivel(){
    dataFinNivel.h = 600;
    dataFinNivel.w = 800;
    dataFinNivel.x = 0;
    dataFinNivel.y = 0;
    renderizados = 1;
}


bool Nivel::iniciarNivel(Client* client){
    bool quit = false;
    printf("intenta gettear nave %d\n", client->get_id());
    NaveJugador* jugador1 = jugadores[client->get_id()];
    //printf("aaa\n");
    sounds.playMusic(gMusic);

    int id_nave = jugador1->get_id();

    while( usuarioNoRequieraSalir(quit) ){
        while( hayEventos() ) {
            if( eventoEsSalir() ) quit = true;
            int new_misil = 1;
            jugador1->handleEvent( e, gMusic, &new_misil);
            if(new_misil == 0){
                velocidades_t* v_shot = (velocidades_t*) malloc(sizeof(velocidades_t));
                v_shot->id = id_nave;
                strcpy(v_shot->descrip, "shot");
                v_shot->VelX = jugador1->getPosX();
                v_shot->VelY = jugador1->getPosY();
                client->sendData(v_shot);
            }
        }

        velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
        v->id = id_nave;
        v->descrip[0] = 0;
        strncat(v->descrip, "on", 5);
        v->VelX = jugador1->getVelX();
        v->VelY = jugador1->getVelY();

        //printf("CLIENT ID: %d vel: %d - %d\n", v->id, v -> VelX, v->VelY);

        if(! client->sendData(v)){
            client->renderServerCaido();
            logger.error("El servidor esta caido, terminando la ejecucion...");
            free(v);
            return true;
        }

        /*vector<Misil*>::iterator pos_m;
        for(pos_m = misiles.begin(); pos_m != misiles.end(); pos_m++){
            velocidades_t* v_shot = (velocidades_t*) malloc(sizeof(velocidades_t));
            v_shot->id = id_nave;
            strcpy(v_shot->descrip, "shot");
            v_shot->VelX = (*pos_m)->getVel();
            v_shot->VelY = 0;
            client->sendData(v_shot);
        }*/
        //misiles.clear();

        while(!client->cola_esta_vacia()){
            void* dato = client->desencolar();
            posiciones_t* pos = (posiciones_t*) dato;
            if(pos->id == -1){
                return false;
            }
            procesar((posiciones_t*) dato);
        }

        free(v);

        renderizar();

    }
    client->finalizar();
    return quit;
}
void Nivel::renderizar(){

        SDL_RenderClear( sdl.getRenderer() );
        renderBackground();

        //render jugador
        vector<NaveJugador*>::iterator pos;
        for(pos = jugadores.begin(); pos != jugadores.end(); pos++){
            (*pos)->renderizar();
        }

        //render enemigos
        for(int i = 0; i < renderizados; i++){
            enemigos[i]->renderizar();
        }

        vector<Misil*>::iterator pos_m;
        std::cout<<"SIZE_REND: "<<misiles.size()<<"\n";
        for(pos_m = misiles.begin(); pos_m != misiles.end(); pos_m++){
            (*pos_m)->mover();
            (*pos_m)->renderizar();
        }

        //Todo este bloque deberiamos declararlo en otro lado

        SDL_RenderPresent( sdl.getRenderer() );
}

void Nivel::procesar(posiciones_t* pos){
    if(strcmp(pos->descrip, "shot") == 0){
        std::cout<<"x: "<<pos->posX<<"\n";
        std::cout<<"y: "<<pos->posY<<"\n";
        std::cout<<"-------------\n";
        Misil* misil = new Misil(pos->posX, pos->posY, pos->id);
        misiles.push_back(misil);
        std::cout<<"SIZE: "<<misiles.size()<<"\n";
    } else{
        if(pos->id>3){
            aumentarRenderizados(pos->id-4);
            for(int i = 0; i < renderizados ; i++){
                enemigos[i]->mover(jugadores[0]);
            }
            parallax();
            return;
        }
        if(strcmp(pos->descrip, "off") != 0){
            jugadores[pos->id]->setPosX(pos->posX);
            jugadores[pos->id]->setPosY(pos->posY);
            if(!jugadores[pos->id]->isOn()) jugadores[pos->id]->conectar();
        } else{
            jugadores[pos->id]->desconectar();
        }
    }
    free(pos);
}

void Nivel::finalizar() {

    sounds.stopMusic();
    sounds.freeMusic(gMusic);

    logger.info("Finalizó el nivel");
    gFinNivel.render(0,0, &dataFinNivel);
    SDL_RenderPresent( sdl.getRenderer() );
    logger.info("Se renderizo el final del nivel");
}

void Nivel::aumentarRenderizados(int i){
    pthread_mutex_lock(&mutex);
    if(i>renderizados)renderizados++;
    pthread_mutex_unlock(&mutex);
}

void Nivel::setNaves(Client* client){
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    while(true){
        pos = (posiciones_t*)client->receiveData();
        printf("recibe nave, ID: %d\n", pos->id);
        if(pos->id == -1) break;
        if(pos->id>3){
            NaveEnemiga* enemigo = new NaveEnemiga(pos->posX, pos->posY, pos->descrip);
            enemigos.push_back(enemigo);
        } else{
            printf("creo nave jugador\n");
            NaveJugador* nave = new NaveJugador(pos->posX, pos->posY, pos->id);
            if(strcmp(pos->descrip, "off") == 0) {
                printf("nave off\n");
                nave->desconectar();
            }
            jugadores.push_back(nave);
        }
    }
}

void Nivel::parallax(){}

void Nivel::cerrar(){}

void Nivel::cargarNivel(Client* client){}

void Nivel::renderBackground(){}

void Nivel::reconectar(Client* client){}
