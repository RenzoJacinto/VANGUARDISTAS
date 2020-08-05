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
    DISTANCIA_DE_COMBATE_INICIAL = 300;

    vidas = 1;
    vel = 1;

    damage = 35;

    strcpy(clave, imagenActual);
    radio=alto/2;

    mensaje = "<<<< SE CARGO EL BOSS FINAL" ;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
    searchRate.iniciar();
}

// Se acerca a 500 de distancia (en x) y dispara cada 1 seg
int Boss::procesarAccion(vector<NaveJugador*> jugadores){

    if (searchRate.transcurridoEnSegundos() > 5){
        nave_seguida = obtenerNaveSeguidaRandom(jugadores, jugadores.size());
        searchRate.finalizar();
        searchRate.iniciar();
    }

    NaveJugador* nave = jugadores[nave_seguida];

    int distanciaNaveX = getDistanciaNaveEnX(nave);

    int ok = -1;
    if (!onScreen()) ok = mover(-vel, 0, jugadores);
    else if(distanciaNaveX > DISTANCIA_DE_COMBATE_INICIAL  || mPosX + getRadio() > 800) ok = mover(-vel, 0, jugadores);
    else if(distanciaNaveX < DISTANCIA_DE_COMBATE_INICIAL && mPosX < 800 - getRadio()) ok = mover(vel, 0, jugadores);

    int navePosY = nave->getPosY();
    int delta = navePosY - mPosY;

    if(delta < 0) ok = mover(0, -vel, jugadores);
    else if(delta > 0) ok = mover(0, vel, jugadores);

    disparo = false;

    if (fireRate.transcurridoEnSegundos() > 1.5 && onScreen() && nave->isAlive()) {
        int rd = randomNumber() % 10;
        if (rd < 4) disparoTriple = true;
        if (rd >= 5) disparoTriple = false;
        disparo = true;
        misil_posX = mPosX - getRadio();
        if(disparoTriple) misil_posY = mPosY; //- (getRadio()/2) + randomNumber() % (getRadio() + 1);
        else misil_posY = mPosY - (getRadio()/2) + randomNumber() % (getRadio() + 1);
        fireRate.finalizar();
        fireRate.iniciar();
    }

    return ok;
}

