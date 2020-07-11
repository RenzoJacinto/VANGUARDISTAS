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

    while( usuarioNoRequieraSalir(quit) ){
        while( hayEventos() ) {
            if( eventoEsSalir() ) quit = true;
            jugador1->handleEvent( e, gMusic);
        }

        velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
        v->id = client->get_id();
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

        //Todo este bloque deberiamos declararlo en otro lado

        SDL_RenderPresent( sdl.getRenderer() );
}

void Nivel::procesar(posiciones_t* pos){
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
