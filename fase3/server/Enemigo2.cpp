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

    vel = 7;

    mensaje = "<<<< SE CARGO LA NAVE ENEMIGO2" ;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}

// Se acerca a 500 de distancia (en x) y dispara cada 1 seg
int Enemigo2::procesarAccion(vector<NaveJugador*> jugadores){

    if(nave_seguida == -1) nave_seguida = obtenerNaveSeguidaPonderada(jugadores);
    NaveJugador* nave = jugadores[nave_seguida];

    int distanciaNaveX = getDistanciaNaveEnX(nave);

    int ok = -1;
    if(distanciaNaveX > DISTANCIA_DE_COMBATE_INICIAL) ok = mover(-vel, 0, jugadores);
    else if(distanciaNaveX < DISTANCIA_DE_COMBATE_INICIAL ) ok = mover(2, 0, jugadores);

    int navePosY = nave->getPosY();
    int delta = navePosY - mPosY;

    if(delta < 0) ok = mover(0, -vel, jugadores);
    else if(delta > 0) ok = mover(0, vel, jugadores);

    disparo = false;

    if (fireRate.transcurridoEnSegundos() > 3 && onScreen() && nave->isAlive()) {
        disparo = true;
    }

    return ok;
}

