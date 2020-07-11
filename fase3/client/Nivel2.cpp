#include "Nivel2.h"
#include "global.h"

Nivel2::Nivel2(){}

void Nivel2::cargarNivel(Client* client){

    logger.info(">>>> CARGANDO EL NIVEL 2 ....");
    gMusic = sounds.loadMusic("sounds/nivel2A.wav");

    setNaves(client);

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

	scrollingOffsetAsteroides1 -= 10;
    if( scrollingOffsetAsteroides1 < -dataAsteroides1.w ) scrollingOffsetAsteroides1 = 0;

    scrollingOffsetAsteroides2 -= 5;
    if( scrollingOffsetAsteroides2 < -dataAsteroides2.w ) scrollingOffsetAsteroides2 = 0;

    scrollingOffsetAsteroides3 -= 2.5;
    if( scrollingOffsetAsteroides3 < -dataAsteroides3.w ) scrollingOffsetAsteroides3 = 0;
}

void Nivel2::reconectar(Client* client)
{
    logger.debug("Recibiendo estado actual del nivel");
    posiciones_t* pos = (posiciones_t*)client->receiveData();
    scrollingOffsetPlaneta1 = (double) pos->posX;
    pos = (posiciones_t*)client->receiveData();
    scrollingOffsetPlaneta2 = (double) pos->posX;
    pos = (posiciones_t*)client->receiveData();
    scrollingOffsetAsteroides1 = (double) pos->posX;
    pos = (posiciones_t*)client->receiveData();
    scrollingOffsetAsteroides2 = (double) pos->posX;
    pos = (posiciones_t*)client->receiveData();
    scrollingOffsetAsteroides3 = (double) pos->posX;

    while(true)
    {
        pos = (posiciones_t*)client->receiveData();
        printf("%d\n", pos->id);
        if(pos->id == -1) {
            printf("quitea\n");
            break;
        }
        if(pos->id>3){
            printf("crea nave enemiga\n");
            NaveEnemiga* enemigo = new NaveEnemiga(pos->posX, pos->posY, pos->descrip);
            enemigos.push_back(enemigo);
        } else{
            NaveJugador* nave = new NaveJugador(pos->posX, pos->posY, pos->id);
            if(strcmp(pos->descrip, "off")==0) nave->desconectar();
            printf("crea nave jug\n");
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
}
