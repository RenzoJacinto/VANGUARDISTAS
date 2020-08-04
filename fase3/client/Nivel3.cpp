#include "Nivel3.h"
#include "global.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>

Nivel3::Nivel3(){}

bool Nivel3::cargarNivel(Client* client){

    logger.info(">>>> CARGANDO EL NIVEL 3 ....");

    gMusic = sounds.loadMusic(json.get_sound("music", "nivel3").c_str());

    if(!setNaves(client)) return false;

    posiciones_t* pos;
    while(true){
        pos = (posiciones_t*)client->receiveData();
        if(!pos) return false;
        //printf("recibe nave, ID: %d\n", pos->id);
        if(pos->id == -1) break;
        if(strcmp(pos->descrip, "test") == 0) jugadores[pos->id]->set_modeTest();
        else{
            jugadores[pos->id]->addScore(pos->posY);
            jugadores[pos->id]->setVidas(pos->posX);
        }
    }
    free(pos);
    cargarImagen("nivel3", "mapaBG", &gBGTexture);
    cargarImagen("nivel3", "fondo1", &gFondo1Texture);
    cargarImagen("nivel3", "fondo2", &gFondo2Texture);
    cargarImagen("nivel3", "fondo3", &gFondo3Texture);
    cargarImagen("nivel3", "fondo4", &gFondo4Texture);
    cargarImagen("nivel3", "fondo5", &gFondo5Texture);
    cargarImagen("nivel3", "fondo6", &gFondo6Texture);

    cargarImagen("nivel3", "finNivel", &gFinNivel);

    scrollingOffsetBG = 0;

    scrollingOffsetFondo1 = 0;
    scrollingOffsetFondo2 = 0;
    scrollingOffsetFondo3 = 0;
    scrollingOffsetFondo4 = 0;
    scrollingOffsetFondo5 = 0;
    scrollingOffsetFondo6 = 0;

    dataBG.h = 600;
    dataBG.w = 800;
    dataBG.x = 0;
    dataBG.y = 0;

    dataFondo1.h = 600;
    dataFondo1.w = 800;
    dataFondo1.x = 0;
    dataFondo1.y = 0;

    dataFondo2.h = 600;
    dataFondo2.w = 800;
    dataFondo2.x = 0;
    dataFondo2.y = 0;

    dataFondo3.h = 600;
    dataFondo3.w = 800;
    dataFondo3.x = 0;
    dataFondo3.y = 0;

    dataFondo4.h = 600;
    dataFondo4.w = 800;
    dataFondo4.x = 0;
    dataFondo4.y = 0;

    dataFondo5.h = 600;
    dataFondo5.w = 800;
    dataFondo5.x = 0;
    dataFondo5.y = 0;

    dataFondo6.h = 600;
    dataFondo6.w = 800;
    dataFondo6.x = 0;
    dataFondo6.y = 0;

     logger.info("<<<< SE CARGO EL NIVEL 3");
     return true;
}

void Nivel3::cerrar(){
	gBGTexture.free();
	gFondo1Texture.free();
	gFondo2Texture.free();
	gFondo3Texture.free();
	gFondo4Texture.free();
	gFondo5Texture.free();
	gFondo6Texture.free();

	gFinNivel.free();
	borrarNaves();
}

void Nivel3::renderBackground(){

	gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
	gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

	gFondo1Texture.render( scrollingOffsetFondo1, 0, &dataFondo1 );
	gFondo1Texture.render( scrollingOffsetFondo1 + dataFondo1.w, 0, &dataFondo1 );

	gFondo2Texture.render( scrollingOffsetFondo2, 0, &dataFondo2 );
	gFondo2Texture.render( scrollingOffsetFondo2 + dataFondo2.w, 0, &dataFondo2 );

	gFondo3Texture.render( scrollingOffsetFondo3, 0, &dataFondo3 );
	gFondo3Texture.render( scrollingOffsetFondo3 + dataFondo3.w, 0, &dataFondo3);

	gFondo4Texture.render( scrollingOffsetFondo4, 0, &dataFondo4 );
	gFondo4Texture.render( scrollingOffsetFondo4 + dataFondo4.w, 0, &dataFondo4 );

	gFondo5Texture.render( scrollingOffsetFondo5, 0, &dataFondo5 );
	gFondo5Texture.render( scrollingOffsetFondo5 + dataFondo5.w, 0, &dataFondo5 );

	gFondo6Texture.render( scrollingOffsetFondo6, 0, &dataFondo6 );
	gFondo6Texture.render( scrollingOffsetFondo6 + dataFondo6.w, 0, &dataFondo6 );
}

void Nivel3::parallax(){

    scrollingOffsetBG -= 0.1;
    if( scrollingOffsetBG < -gBGTexture.getWidth() ) scrollingOffsetBG = 250;

    scrollingOffsetFondo1 -= 15;
    if( scrollingOffsetFondo1 < -dataFondo1.w ) scrollingOffsetFondo1 = 0;

    scrollingOffsetFondo2 -= 13.5;
    if( scrollingOffsetFondo2 < -dataFondo2.w ) scrollingOffsetFondo2 = 0;

    scrollingOffsetFondo3 -= 12;
    if( scrollingOffsetFondo3 < -dataFondo3.w ) scrollingOffsetFondo3 = 0;

    scrollingOffsetFondo4 -= 10.5;
    if( scrollingOffsetFondo4 < -dataFondo4.w ) scrollingOffsetFondo4 = 0;

    scrollingOffsetFondo5 -= 9;
    if( scrollingOffsetFondo5 < -dataFondo5.w ) scrollingOffsetFondo5 = 0;

    scrollingOffsetFondo6 -= 7.5;
    if( scrollingOffsetFondo6 < -dataFondo6.w ) scrollingOffsetFondo6 = 0;
}

void Nivel3::reconectar(Client* client){
    logger.debug("Recibiendo estado actual del nivel");
    posicionesR_t* pos = (posicionesR_t*)malloc(sizeof(posicionesR_t));
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetBG = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetFondo1 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetFondo2 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetFondo3 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetFondo4 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetFondo5 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetFondo6 = (double) pos->posX;

    while(true){
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

    cargarImagen("nivel3", "mapaBG", &gBGTexture);
    cargarImagen("nivel3", "fondo1", &gFondo1Texture);
    cargarImagen("nivel3", "fondo2", &gFondo2Texture);
    cargarImagen("nivel3", "fondo3", &gFondo3Texture);
    cargarImagen("nivel3", "fondo4", &gFondo4Texture);
    cargarImagen("nivel3", "fondo5", &gFondo5Texture);
    cargarImagen("nivel3", "fondo6", &gFondo6Texture);

    cargarImagen("nivel3", "finNivel", &gFinNivel);

    dataBG.h = 600;
    dataBG.w = 800;
    dataBG.x = 0;
    dataBG.y = 0;

    dataFondo1.h = 600;
    dataFondo1.w = 800;
    dataFondo1.x = 0;
    dataFondo1.y = 0;

    dataFondo2.h = 600;
    dataFondo2.w = 800;
    dataFondo2.x = 0;
    dataFondo2.y = 0;

    dataFondo3.h = 600;
    dataFondo3.w = 800;
    dataFondo3.x = 0;
    dataFondo3.y = 0;

    dataFondo4.h = 600;
    dataFondo4.w = 800;
    dataFondo4.x = 0;
    dataFondo4.y = 0;

    dataFondo5.h = 600;
    dataFondo5.w = 800;
    dataFondo5.x = 0;
    dataFondo5.y = 0;

    dataFondo6.h = 600;
    dataFondo6.w = 800;
    dataFondo6.x = 0;
    dataFondo6.y = 0;

    gMusic = sounds.loadMusic(json.get_sound("music", "nivel3").c_str());
}
