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

    vel = 1.2;

    mensaje = "<<<< SE CARGO LA NAVE ENEMIGO1" ;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
    searchRate.iniciar();
}

// Dispara cada 1 segundo de lejos (250 en x), a un jugador al azar
int Enemigo1::procesarAccion(vector<NaveJugador*> jugadores){

    if(nave_seguida == -1 || !jugadores[nave_seguida]->isAlive() || searchRate.transcurridoEnSegundos() > 10){
        nave_seguida = obtenerNaveSeguidaRandom(jugadores, jugadores.size());
        searchRate.finalizar();
        searchRate.iniciar();
    }

    if(!jugadores[nave_seguida]->isAlive()) nave_seguida = obtenerNaveSeguidaRandom(jugadores, jugadores.size());

    NaveJugador* nave = jugadores[nave_seguida];
    disparoTriple = false;

    int distanciaNaveX = getDistanciaNaveEnX(nave);

    int ok = -1;
    if(distanciaNaveX > DISTANCIA_DE_COMBATE_INICIAL || mPosX + getRadio() > 800) ok = mover(-vel, 0, jugadores);
    else if(distanciaNaveX < DISTANCIA_DE_COMBATE_INICIAL && mPosX < 800 - getRadio() ) ok = mover(2, 0, jugadores);

    if (ok != -1) return ok;

    int navePosY = nave->getPosY();
    int delta = navePosY - mPosY;

    if(delta < -vel) ok = mover(0, -vel, jugadores);
    else if(delta > vel) ok = mover(0, vel, jugadores);

    disparo = false;

    if (ok != -1) return ok;

    if (fireRate.transcurridoEnSegundos() > 3 && onScreen() && nave->isAlive()) {
        disparo = true;
        misil_posX = mPosX - getRadio();
        misil_posY = mPosY;
        fireRate.finalizar();
        fireRate.iniciar();
    }

    return ok;
}
