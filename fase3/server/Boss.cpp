#include "Boss.h"
#include "time_nanoseconds.h"

Boss::Boss(int x, int y){

    disparo = false;

    std::string mensaje =  ">>>> CARGANDO EL BOSS FINAL ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y)){
        mensaje = "Se creo el boss final";
        logger.debug(mensaje.c_str());
    }

    strcpy(imagenActual, "boss");

    alto = 293;
    ancho = 363;

    score = 2000;
    energia_actual = 1000;
    energia_total = 1000;
    DISTANCIA_DE_COMBATE_INICIAL = 500;

    vidas = 1;
    vel = 1;

    damage = 35;

    strcpy(clave, imagenActual);
    radio=alto/2;

    mensaje = "<<<< SE CARGO EL BOSS FINAL" ;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}

// Se acerca a 500 de distancia (en x) y dispara cada 1 seg
int Boss::procesarAccion(vector<NaveJugador*> jugadores){

    if(nave_seguida == -1 || !jugadores[nave_seguida]->isAlive()) nave_seguida = obtenerNaveSeguidaRandom(jugadores, jugadores.size());
    NaveJugador* nave = jugadores[nave_seguida];

    int distanciaNaveX = getDistanciaNaveEnX(nave);

    int ok = -1;
    if(distanciaNaveX > DISTANCIA_DE_COMBATE_INICIAL) ok = mover(-vel, 0, jugadores);
    else if(distanciaNaveX < DISTANCIA_DE_COMBATE_INICIAL ) ok = mover(vel, 0, jugadores);

    int navePosY = nave->getPosY();
    int delta = navePosY - mPosY;

    if(delta < 0) ok = mover(0, -vel, jugadores);
    else if(delta > 0) ok = mover(0, vel, jugadores);

    disparo = false;

    if (fireRate.transcurridoEnSegundos() > 0.85 && onScreen() && nave->isAlive()) {
        disparo = true;
        fireRate.finalizar();
        fireRate.iniciar();
    }

    return ok;
}

