#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite){
    naveSeguida = -1;
    disparo = false;
    std::string sp(sprite);
    std::string mensaje =  ">>>> CARGANDO LA NAVE " + sp + " ....";
    logger.info(mensaje.c_str());
    if(crearNave(x, y)){
        mensaje = "Se creo el " + sp;
        logger.debug(mensaje.c_str());
    }

    if(sp == "enemigo3" || sp == "enemigo4") desplazamiento = 1;
    else desplazamiento = -1;

    if(sp == "enemigo1" || sp == "enemigo3"){
        alto = 80;
        ancho = 80;
        score = 500;
        energia_actual = 100;
        energia_total = 100;
    } else{
        alto = 63;
        ancho = 63;
        score = 1000;
        energia_actual = 200;
        energia_total = 200;
    }

    vidas = 1;

    //clave[0] = 0;
    strcpy(clave, sprite);
    radio=alto/2;
    mensaje = "<<<< SE CARGO LA NAVE " + sp;
    logger.info(mensaje.c_str());
}

void NaveEnemiga::mover(int velX, int velY){

    if(!isAlive()) return;

    for(unsigned int i = 0; i < jugadores.size(); ++i){
        if( checkCollision( jugadores[i] , this ) ) return;
    }

    setPosX(getPosX()+velX);
    setPosY(getPosY()+velY);

}

int NaveEnemiga::getRadio(){
    return radio;
}

const char* NaveEnemiga::getImagen(){
    return imagen;
}

int NaveEnemiga::getAltoImagen(){
	return alto;
}

int NaveEnemiga::getAnchoImagen(){
	return ancho;
}

char* NaveEnemiga::getClave(){
    return clave;
}

void NaveEnemiga::setNaveSeguida(int id){
    naveSeguida = id;
}

int NaveEnemiga::getNaveSeguida(){
    return naveSeguida;
}

const int DISTANCIA_DE_COMBATE = 100;

void NaveEnemiga::procesarAccion(NaveJugador* nave){
    disparo = false;
    int distanciaNave = getDistanciaNave(nave);
    int posY = getPosY();
    int navePosY = nave->getPosY();
    if (distanciaNave != DISTANCIA_DE_COMBATE || posY != navePosY){
        if (distanciaNave != DISTANCIA_DE_COMBATE) seguirNave(nave, distanciaNave);
        if (posY != navePosY) acomodarseEnEjeY(navePosY);
    }
    else disparo = true;
}

int NaveEnemiga::getDistanciaNave(NaveJugador* nave){
    return abs(nave->getPosX() - getPosX());
}

void NaveEnemiga::seguirNave(NaveJugador* nave, int distanciaNave){
    if (distanciaNave > DISTANCIA_DE_COMBATE){
        if (nave->getPosX() < getPosX()) mover(-2, 0);
        if (nave->getPosX() > getPosX()) mover(2, 0);
    }else{
        if (nave->getPosX() < getPosX()) mover(2, 0);
        if (nave->getPosX() > getPosX()) mover(-2, 0);
    }
}

void NaveEnemiga::acomodarseEnEjeY(int navePosY){
    if (getPosY() < navePosY) mover(0, 2);
    else mover(0, -2);
}

void NaveEnemiga::setJugadores(vector<NaveJugador*> listaJugadores){
    jugadores = listaJugadores;
}

bool NaveEnemiga::seDisparo(){
    return disparo;
}
