#include "Nivel2.h"
#include "global.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>

Nivel2::Nivel2(){}

void Nivel2::cargarNivel(Client* client){

    logger.info(">>>> CARGANDO EL NIVEL 2 ....");
    gMusic = sounds.loadMusic(json.get_sound("music", "nivel2").c_str());

    setNaves(client);

    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    while(true){
        pos = (posiciones_t*)client->receiveData();
        //printf("recibe nave, ID: %d\n", pos->id);
        if(pos->id == -1) break;
        jugadores[pos->id]->addScore(pos->posY);
        jugadores[pos->id]->setVidas(pos->posX);
    }
    free(pos);
    cargarImagen("nivel2", "mapaBG", &gBGTexture);
    cargarImagen("nivel2", "planeta1", &gPlaneta1Texture);
    cargarImagen("nivel2", "planeta2", &gPlaneta2Texture);
    cargarImagen("nivel2", "asteroides1", &gAsteroides1Texture);
    cargarImagen("nivel2", "asteroides2", &gAsteroides2Texture);
    cargarImagen("nivel2", "asteroides3", &gAsteroides3Texture);

    cargarImagen("nivel2", "finNivel", &gFinNivel);

    scrollingOffsetPlaneta1 = 300;
    scrollingOffsetPlaneta2 = 1000;

    scrollingOffsetAsteroides1 = 0;
    scrollingOffsetAsteroides2 = 0;
    scrollingOffsetAsteroides3 = 0;

    dataAsteroides1.h = 600;
    dataAsteroides1.w = 800;
    dataAsteroides1.x = 0;
    dataAsteroides1.y = 0;

    dataAsteroides2.h = 600;
    dataAsteroides2.w = 872;
    dataAsteroides2.x = 0;
    dataAsteroides2.y = 0;

    dataAsteroides3.h = 600;
    dataAsteroides3.w = 872;
    dataAsteroides3.x = 0;
    dataAsteroides3.y = 0;

    logger.info("<<<< SE CARGO EL NIVEL 2");
}

void Nivel2::cerrar(){
	gBGTexture.free();
	gPlaneta1Texture.free();
	gPlaneta2Texture.free();
	gAsteroides1Texture.free();
	gAsteroides2Texture.free();
	gAsteroides3Texture.free();


	gFinNivel.free();
	borrarNaves();
}

void Nivel2::renderBackground(){

	gBGTexture.render(0, 0);

	gPlaneta2Texture.render(scrollingOffsetPlaneta2, 100);

	gPlaneta1Texture.render(scrollingOffsetPlaneta1, 200);

    gAsteroides3Texture.render( scrollingOffsetAsteroides3, 0, &dataAsteroides3 );
	gAsteroides3Texture.render( scrollingOffsetAsteroides3 + dataAsteroides3.w, 0, &dataAsteroides3 );

	gAsteroides2Texture.render( scrollingOffsetAsteroides2, 0, &dataAsteroides2 );
	gAsteroides2Texture.render( scrollingOffsetAsteroides2 + dataAsteroides2.w, 0, &dataAsteroides2 );

    gAsteroides1Texture.render(scrollingOffsetAsteroides1, 0, &dataAsteroides1);
	gAsteroides1Texture.render(scrollingOffsetAsteroides1 + dataAsteroides1.w, 0, &dataAsteroides1);
}


void Nivel2::parallax(){

    scrollingOffsetPlaneta2 -= 0.25;

    scrollingOffsetPlaneta1 -= 0.20;

	scrollingOffsetAsteroides1 -= 5;
    if( scrollingOffsetAsteroides1 < -dataAsteroides1.w ) scrollingOffsetAsteroides1 = 0;

    scrollingOffsetAsteroides2 -= 3;
    if( scrollingOffsetAsteroides2 < -dataAsteroides2.w ) scrollingOffsetAsteroides2 = 0;

    scrollingOffsetAsteroides3 -= 2;
    if( scrollingOffsetAsteroides3 < -dataAsteroides3.w ) scrollingOffsetAsteroides3 = 0;
}

void Nivel2::reconectar(Client* client)
{
    logger.debug("Recibiendo estado actual del nivel");
    posicionesR_t* pos = (posicionesR_t*)malloc(sizeof(posicionesR_t));
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetPlaneta1 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetPlaneta2 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetAsteroides1 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetAsteroides2 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetAsteroides3 = (double) pos->posX;

    while(true)
    {
        recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
        if(pos->id == -1) break;
        if(pos->id>3){
            NaveEnemiga* enemigo = new NaveEnemiga(pos->posX, pos->posY, pos->descrip);
            enemigo->setEnergiasReconex(pos->energiaActual, 0);
            enemigo->setVidas(pos->vidas);
            enemigos.push_back(enemigo);
        } else{
            NaveJugador* nave = new NaveJugador(pos->posX, pos->posY, pos->id, client->get_id_user(pos->id));
            if(strcmp(pos->descrip, "off")==0) nave->desconectar();
            nave->setEnergiasReconex(pos->energiaActual, 0);
            nave->setVidas(pos->vidas);
            nave->setScore(pos->score);
            jugadores.push_back(nave);
        }
    }

    cargarImagen("nivel2", "mapaBG", &gBGTexture);
    cargarImagen("nivel2", "planeta1", &gPlaneta1Texture);
    cargarImagen("nivel2", "planeta2", &gPlaneta2Texture);
    cargarImagen("nivel2", "asteroides1", &gAsteroides1Texture);
    cargarImagen("nivel2", "asteroides2", &gAsteroides2Texture);
    cargarImagen("nivel2", "asteroides3", &gAsteroides3Texture);

    cargarImagen("nivel2", "finNivel", &gFinNivel);

    dataAsteroides1.h = 600;
    dataAsteroides1.w = 800;
    dataAsteroides1.x = 0;
    dataAsteroides1.y = 0;

    dataAsteroides2.h = 600;
    dataAsteroides2.w = 872;
    dataAsteroides2.x = 0;
    dataAsteroides2.y = 0;

    dataAsteroides3.h = 600;
    dataAsteroides3.w = 872;
    dataAsteroides3.x = 0;
    dataAsteroides3.y = 0;

    gMusic = sounds.loadMusic(json.get_sound("music", "nivel2").c_str());
}
