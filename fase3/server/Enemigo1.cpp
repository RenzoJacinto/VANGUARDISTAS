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

    int nave_seguida = obtenerNaveSeguidaRandom(jugadores, jugadores.size());
    NaveJugador* nave = jugadores[nave_seguida];

    int distanciaNaveX = getDistanciaNaveEnX(nave);

    int ok = -1;
    mover(-getRadio() * (randomNumber() % 3), 0, jugadores);
    if(distanciaNaveX < DISTANCIA_DE_COMBATE_INICIAL ) ok = mover(getRadio() * (randomNumber() % 3), 0, jugadores);

    int navePosY = nave->getPosY();
    int delta = navePosY - mPosY;

    if(delta < 0) ok = mover(0, -getRadio() * (randomNumber() % 3), jugadores);
    else if(delta > 0) ok = mover(0, getRadio() * (randomNumber() % 3), jugadores);

    disparo = false;

    if (fireRate.transcurridoEnSegundos() > 1) {
        disparo = true;
    }

    return ok;
}
