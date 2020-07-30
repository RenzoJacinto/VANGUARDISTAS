#include "Enemigo1.h"
#include "time_nanoseconds.h"

Enemigo1::Enemigo1(int x, int y){

    disparo = false;

    std::string mensaje =  ">>>> CARGANDO LA NAVE ENEMIGO1 ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y)){
        mensaje = "Se creo el enemigo1";
        logger.debug(mensaje.c_str());
    }

    strcpy(imagenActual, "enemigo1");
    strcpy(imagenEspejo, "enemigo3");

    alto = 80;
    ancho = 80;
    score = 500;
    energia_actual = 100;
    energia_total = 100;
    DISTANCIA_DE_COMBATE_INICIAL = 250;

    vidas = 1;

    strcpy(clave, imagenActual);
    radio=alto/2;

    damage = 15;

    mensaje = "<<<< SE CARGO LA NAVE ENEMIGO1" ;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}

// Dispara cada 1 segundo de lejos (250 en x), a un jugador al azar
int Enemigo1::procesarAccion(vector<NaveJugador*> jugadores){

    int nave_seguida = obtenerNaveSeguidaRandom(jugadores.size());
    NaveJugador* nave = jugadores[nave_seguida];
    int distanciaNave = getDistanciaNaveEnX(nave);

    if (distanciaNave < DISTANCIA_DE_COMBATE_INICIAL){
        DISTANCIA_DE_COMBATE = 100 + (getRadio() *(randomNumber() % 10));
        DISTANCIA_DE_COMBATE_INICIAL = 250;
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
