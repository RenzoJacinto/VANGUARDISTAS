#include "Nivel1.h"
#include "global.h"
#include "Client.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>

class Client;

Nivel1::Nivel1(){}

bool Nivel1::cargarNivel(Client* client){

    logger.info(">>>> CARGANDO EL NIVEL 1 ....");

    gMusic = sounds.loadMusic(json.get_sound("music", "nivel1").c_str());

    if(!setNaves(client)) return false;

    cargarImagen("nivel1", "mapaBG", &gBGTexture);
    cargarImagen("nivel1", "ciudad", &gCiudadTexture);
    cargarImagen("nivel1", "planeta", &gPlanetaTexture);
    cargarImagen("nivel1", "nube1", &gNube1);
    cargarImagen("nivel1", "nube2", &gNube2);

    cargarImagen("nivel1", "finNivel", &gFinNivel);

    scrollingOffsetBG = 0;
    scrollingOffsetCity = 0;
    tierraInicial = 700;
    scrollingOffsetNube1 = 120;
    scrollingOffsetNube2 = 0;

    dataBG.h = 600;
    dataBG.w = 2048;
    dataBG.x = 0;
    dataBG.y = 0;

    dataCiudad.h = 450;
    dataCiudad.w = 2048;
    dataCiudad.x = 0;
    dataCiudad.y = 0;

    dataNube1.h = 600;
    dataNube1.w = 800;
    dataNube1.x = 0;
    dataNube1.y = 0;

    dataNube2.h = 600;
    dataNube2.w = 800;
    dataNube2.x = 0;
    dataNube2.y = 0;

    logger.info("<<<< SE CARGO EL NIVEL 1");
    return true;
}

void Nivel1::cerrar(){
	gBGTexture.free();
	gCiudadTexture.free();
	gPlanetaTexture.free();
	gNube1.free();
	gNube2.free();

	gFinNivel.free();

	borrarNaves();
}

void Nivel1::renderBackground(){

    //parallax();

	gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
	gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

	gPlanetaTexture.render(tierraInicial, -50);

	gCiudadTexture.render(scrollingOffsetCity, 150, &dataCiudad);
	gCiudadTexture.render(scrollingOffsetCity + dataCiudad.w, 150, &dataCiudad);

	gNube1.render( scrollingOffsetNube1, 0, &dataNube1 );
	gNube1.render( scrollingOffsetNube1 + dataNube1.w, 0, &dataNube1 );
	gNube2.render( scrollingOffsetNube2, 0, &dataNube2 );
	gNube2.render( scrollingOffsetNube2 + dataNube2.w, 0, &dataNube2 );
}


void Nivel1::parallax(){

    tierraInicial -= 0.05;

    scrollingOffsetBG -= 0.075;
    if( scrollingOffsetBG < -gBGTexture.getWidth() ) scrollingOffsetBG = 0;

	scrollingOffsetCity -= 1;
    if( scrollingOffsetCity < -dataCiudad.w ) scrollingOffsetCity = 0;

    scrollingOffsetNube1 -= 3;
    if( scrollingOffsetNube1 < -dataNube1.w ) scrollingOffsetNube1 = 0;

    scrollingOffsetNube2 -= 5;
    if( scrollingOffsetNube2 < -dataNube2.w ) scrollingOffsetNube2 = 0;

}

void Nivel1::reconectar(Client* client){

    logger.debug("Recibiendo estado actual del nivel");
    posicionesR_t* pos = (posicionesR_t*)malloc(sizeof(posicionesR_t));
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetBG = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetCity = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    tierraInicial = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetNube1 = (double) pos->posX;
    recv(client->get_socket(), pos, sizeof(posicionesR_t), MSG_NOSIGNAL);
    scrollingOffsetNube2 = (double) pos->posX;

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
            else if (strcmp(pos->descrip, "test") == 0) nave->set_modeTest();
            nave->setEnergiasReconex(pos->energiaActual, 0);
            nave->setVidas(pos->vidas);
            nave->setScore(pos->score);
            jugadores.push_back(nave);
        }
    }

    cargarImagen("nivel1", "mapaBG", &gBGTexture);
    cargarImagen("nivel1", "ciudad", &gCiudadTexture);
    cargarImagen("nivel1", "planeta", &gPlanetaTexture);
    cargarImagen("nivel1", "nube1", &gNube1);
    cargarImagen("nivel1", "nube2", &gNube2);

    cargarImagen("nivel1", "finNivel", &gFinNivel);

    dataBG.h = 600;
    dataBG.w = 2048;
    dataBG.x = 0;
    dataBG.y = 0;

    dataCiudad.h = 450;
    dataCiudad.w = 2048;
    dataCiudad.x = 0;
    dataCiudad.y = 0;

    dataNube1.h = 600;
    dataNube1.w = 800;
    dataNube1.x = 0;
    dataNube1.y = 0;

    dataNube2.h = 600;
    dataNube2.w = 800;
    dataNube2.x = 0;
    dataNube2.y = 0;

    gMusic = sounds.loadMusic(json.get_sound("music", "nivel1").c_str());
}
