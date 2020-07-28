#include "Enemigo3.h"

Enemigo3::Enemigo3(int x, int y, const char* sprite){
    naveSeguida = -1;
    disparo = false;
    std::string sp(sprite);
    std::string mensaje =  ">>>> CARGANDO LA NAVE " + sp + " ....";
    logger.info(mensaje.c_str());
    if(crearNave(x, y)){
        mensaje = "Se creo el " + sp;
        logger.debug(mensaje.c_str());
    }

    alto = 80;
    ancho = 80;
    score = 500;
    energia_actual = 100;
    energia_total = 100;
    DISTANCIA_DE_COMBATE = 0;

    vidas = 1;

    //clave[0] = 0;
    strcpy(clave, sprite);
    radio=alto/2;
    mensaje = "<<<< SE CARGO LA NAVE " + sp;
    logger.info(mensaje.c_str());
    fireRate.iniciar();
}

void Enemigo3::setNaveSeguida(int id){
    naveSeguida = id;
}

int Enemigo3::getNaveSeguida(){
    return naveSeguida;
}

void Enemigo3::procesarAccion(NaveJugador* nave){
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

int Enemigo3::getDistanciaNave(NaveJugador* nave){
    return abs(nave->getPosX() - getPosX());
}

void Enemigo3::seguirNave(NaveJugador* nave, int distanciaNave){
    if (distanciaNave > DISTANCIA_DE_COMBATE){
        if (nave->getPosX() < getPosX()) mover(-2, 0);
        if (nave->getPosX() > getPosX()) mover(2, 0);
    }else{
        if (nave->getPosX() < getPosX()) mover(1, 0);
        if (nave->getPosX() > getPosX()) mover(-1, 0);
    }
}

void Enemigo3::acomodarseEnEjeY(int navePosY){
    if (getPosY() < navePosY) mover(0, 2);
    else mover(0, -2);
}

void Enemigo3::setJugadores(vector<NaveJugador*> listaJugadores){
    jugadores = listaJugadores;
}

bool Enemigo3::seDisparo(){
    return disparo;
}

void Enemigo3::reiniciarDisparo(){
    fireRate.iniciar();
}
