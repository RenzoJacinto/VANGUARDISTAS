#include "Enemigo2.h"
#include "time_nanoseconds.h"

Enemigo2::Enemigo2(int x, int y){

    disparo = false;

    std::string mensaje =  ">>>> CARGANDO LA NAVE ENEMIGO2 ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y)){
        mensaje = "Se creo el enemigo2";
        logger.debug(mensaje.c_str());
    }

    strcpy(imagenActual, "enemigo2");
    strcpy(imagenEspejo, "enemigo4");

    alto = 63;
    ancho = 63;
    score = 1000;
    energia_actual = 200;
    energia_total = 200;
    DISTANCIA_DE_COMBATE_INICIAL = 500;

    vidas = 1;

    damage = 25;

    strcpy(clave, imagenActual);
    radio=alto/2;

    mensaje = "<<<< SE CARGO LA NAVE ENEMIGO2" ;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}

// Se acerca a 50 de distancia (en x) y dispara cada 1 seg
int Enemigo2::procesarAccion(vector<NaveJugador*> jugadores){

    int nave_seguida = obtenerNaveSeguidaPonderada(jugadores);
    NaveJugador* nave = jugadores[nave_seguida];
    int distanciaNave = getDistanciaNaveEnX(nave);

    if (distanciaNave < DISTANCIA_DE_COMBATE_INICIAL){
        DISTANCIA_DE_COMBATE = 100 + (getRadio() *(randomNumber() % 10));
        DISTANCIA_DE_COMBATE_INICIAL = 500;
    }
    disparo = false;
    int posY = getPosY();
    int navePosY = nave->getPosY();

    int ok = -1;
    if (abs(distanciaNave - DISTANCIA_DE_COMBATE) > 2 || abs(posY - navePosY) > 2){
        if (abs(distanciaNave - DISTANCIA_DE_COMBATE) > 2) ok = seguirNave(nave, distanciaNave, jugadores);
        if (abs(posY - navePosY) > 2) ok = acomodarseEnEjeY(navePosY, jugadores);
    } else if (fireRate.transcurridoEnSegundos() > 1) {
        disparo = true;
    }

    return ok;
}

