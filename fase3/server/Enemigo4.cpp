#include "Enemigo4.h"

Enemigo4::Enemigo4(int x, int y, const char* sprite){
    naveSeguida = -1;
    disparo = false;
    std::string sp(sprite);
    std::string mensaje =  ">>>> CARGANDO LA NAVE " + sp + " ....";
    logger.info(mensaje.c_str());
    if(crearNave(x, y)){
        mensaje = "Se creo el " + sp;
        logger.debug(mensaje.c_str());
    }

    alto = 63;
    ancho = 63;
    score = 1000;
    energia_actual = 200;
    energia_total = 200;
    DISTANCIA_DE_COMBATE = 500;

    vidas = 1;

    //clave[0] = 0;
    strcpy(clave, sprite);
    radio=alto/2;
    mensaje = "<<<< SE CARGO LA NAVE " + sp;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}


void Enemigo4::setNaveSeguida(int id){
    naveSeguida = id;
}

int Enemigo4::getNaveSeguida(){
    return naveSeguida;
}

void Enemigo4::procesarAccion(NaveJugador* nave){
    int distanciaNave = getDistanciaNave(nave);
    if (distanciaNave < DISTANCIA_DE_COMBATE_INICIAL){
        srand(current_time_nanoseconds() * 1000000000);
        DISTANCIA_DE_COMBATE = 100 + (getRadio() *(rand() % 10));
        DISTANCIA_DE_COMBATE_INICIAL = 0;
    }
    disparo = false;
    int posY = getPosY();
    int navePosY = nave->getPosY();
    if (abs(distanciaNave - DISTANCIA_DE_COMBATE) > 2 || abs(posY - navePosY) > 2){
        if (abs(distanciaNave - DISTANCIA_DE_COMBATE) > 2) seguirNave(nave, distanciaNave);
        if (abs(posY - navePosY) > 2) acomodarseEnEjeY(navePosY);
    } else if (fireRate.transcurridoEnSegundos()>2) {
        disparo = true;
        //printf("dispara\n");
    }
}

int Enemigo4::getDistanciaNave(NaveJugador* nave){
    return abs(nave->getPosX() - getPosX());
}

void Enemigo4::seguirNave(NaveJugador* nave, int distanciaNave){
    if (distanciaNave > DISTANCIA_DE_COMBATE){
        if (nave->getPosX() < getPosX()) mover(-2, 0);
        if (nave->getPosX() > getPosX()) mover(2, 0);
    }else{
        if (nave->getPosX() < getPosX()) mover(1, 0);
        if (nave->getPosX() > getPosX()) mover(-1, 0);
    }
}

void Enemigo4::acomodarseEnEjeY(int navePosY){
    if (getPosY() < navePosY) mover(0, 2);
    else mover(0, -2);
}

void Enemigo4::setJugadores(vector<NaveJugador*> listaJugadores){
    jugadores = listaJugadores;
}

bool Enemigo4::seDisparo(){
    return disparo;
}

void Enemigo4::reiniciarDisparo(){
    fireRate.iniciar();
}
