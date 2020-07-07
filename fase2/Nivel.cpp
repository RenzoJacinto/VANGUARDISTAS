#include "Nivel.h"
#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Server.h"
#include "Client.h"
#include "Estado.h"
#include "Temporizador.h"
#include "global.h"
#include "Server.h"
#include "Estado.h"
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

    NaveJugador* jugador1 = jugadores[client->get_id()];

    while( usuarioNoRequieraSalir(quit) ){
        while( hayEventos() ) {
            if( eventoEsSalir() ) quit = true;
            jugador1->handleEvent( e );
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

void Nivel::parallax(){}

void Nivel::cerrar(){}

void Nivel::cargarNivel(Client* client){}

void Nivel::renderBackground(){}

void Nivel::reconectar(Client* client){}
