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

void* renderEnemies(void* p)
{
    Nivel* niv = (Nivel*)p;
    niv->renderEnemigos();
    return NULL;
}

void* renderBoom(void* p)
{
    Nivel* niv = (Nivel*)p;
    niv->renderBooms();
    return NULL;
}

bool Nivel::iniciarNivel(Client* client){
    bool quit = false;
    printf("intenta gettear nave %d\n", client->get_id());
    NaveJugador* jugador1 = jugadores[client->get_id()];
    shotFX = sounds.loadEffect("sounds/shot.wav");
    explosion = sounds.loadEffect("sounds/explocion.wav");
    //printf("aaa\n");
    sounds.playMusic(gMusic);

    int id_nave = jugador1->get_id();

    while( usuarioNoRequieraSalir(quit) ){

        SDL_RenderClear( sdl.getRenderer() );

        renderBackground();

        renderizar();

        while( hayEventos() ) {
            if( eventoEsSalir() ) quit = true;
            int new_misil = 1;
            jugador1->handleEvent( e, gMusic, &new_misil);
            if(new_misil == 0){
                velocidades_t* v_shot = (velocidades_t*) malloc(sizeof(velocidades_t));
                v_shot->id = id_nave;
                strcpy(v_shot->descrip, "shot0");
                v_shot->VelX = jugador1->getPosX();
                v_shot->VelY = jugador1->getPosY();
                client->sendData(v_shot);
                free(v_shot);
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

        while(!client->cola_esta_vacia()){
            void* dato = client->desencolar();
            posiciones_t* pos = (posiciones_t*) dato;
            if(pos->id == -1){
                return false;
            }
            procesar((posiciones_t*) dato);
        }

        free(v);

        //renderizar();

    }
    client->finalizar();
    return quit;
}
void Nivel::renderizar(){

        list<Misil*>::iterator pos_m = misiles.begin();
        while(pos_m != misiles.end()){
            //(*pos_m)->mover();
            (*pos_m)->renderizar();
            pos_m = misiles.erase(pos_m);
        }
        //std::cout<<"SIZE: "<<misiles.size()<<"\n";

        //render jugador

        vector<NaveJugador*>::iterator pos;
        for(pos = jugadores.begin(); pos != jugadores.end(); pos++){
            (*pos)->renderizar();
        }

        for(int i = 0; i < renderizados; i++){
            if(enemigos[i]->isAlive()) enemigos[i]->renderizar();
        }

        for(int i = 0; i < renderizados; i++){
            if(enemigos[i]->boomAvailable())
            {
                sounds.playEffect(explosion);
                enemigos[i]->renderBoom();
            }
        }
        //Todo este bloque deberiamos declararlo en otro lado

        SDL_RenderPresent( sdl.getRenderer() );
}

void Nivel::procesar(posiciones_t* pos){
    if(strcmp(pos->descrip, "shot0") == 0 || strcmp(pos->descrip, "shot1") == 0)
    {
        if(strcmp(pos->descrip, "shot0") == 0) sounds.playEffect(shotFX);
        Misil* misil = new Misil(pos->posX, pos->posY, pos->id);
        misiles.push_back(misil);
    }
    else if(strcmp(pos->descrip, "hit") == 0)
    {
        if(enemigos[pos->id - 4]->isAlive()) enemigos[pos->id - 4]->die();
    }
    else{
        if(pos->id>3){
            aumentarRenderizados(pos->id-3);
            enemigos[pos->id - 4]->setPosX(pos->posX);
            enemigos[pos->id - 4]->setPosY(pos->posY);
            if(pos->id-3 == renderizados) parallax();
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

    sounds.freeEffect(shotFX);

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
            NaveJugador* nave = new NaveJugador(pos->posX, pos->posY, pos->id, client->get_id_user(pos->id));
            if(strcmp(pos->descrip, "off") == 0) {
                printf("nave off\n");
                nave->desconectar();
            }
            jugadores.push_back(nave);
        }
    }
}

void Nivel::renderEnemigos()
{
    for(int i = 0; i < renderizados; i++){
        if(enemigos[i]->isAlive()) enemigos[i]->renderizar();
    }
}
void Nivel::renderBooms()
{
    for(int i = 0; i < renderizados; i++){
        if(enemigos[i]->boomAvailable()) enemigos[i]->renderBoom();
    }
}

void Nivel::parallax(){}

void Nivel::cerrar(){}

void Nivel::cargarNivel(Client* client){}

void Nivel::renderBackground(){}

void Nivel::reconectar(Client* client){}
