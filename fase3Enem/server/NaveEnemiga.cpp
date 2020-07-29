#include "NaveEnemiga.h"
#include "time_nanoseconds.h"

NaveEnemiga::NaveEnemiga(){}
int NaveEnemiga::procesarAccion(vector<NaveJugador*> jugadores){}

int NaveEnemiga::mover(int velX, int velY, vector<NaveJugador*> jugadores){

    /*if(!isAlive()){
        destruirNave();
        return;
    }*/

    setPosX(getPosX()+velX);
    setPosY(getPosY()+velY);

    for(unsigned int i = 0; i < jugadores.size(); ++i){
        if( checkCollision( jugadores[i] , this ) ){
            jugadores[i]->die();
            die();
            return i;
        }
    }


    return -1;
}

int NaveEnemiga::getRadio(){
    return radio;
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

int NaveEnemiga::getDistanciaNave(NaveJugador* nave){
    return distanceSquared(mPosX, mPosY, nave->getPosX(), nave->getPosY());
}

int NaveEnemiga::getDistanciaNaveEnX(NaveJugador* nave){
    return abs(nave->getPosX() - getPosX());
}

int NaveEnemiga::getDistanciaNaveEnXConSigno(NaveJugador* nave){
    return nave->getPosX() - getPosX();
}

int NaveEnemiga::seguirNave(NaveJugador* nave, int distanciaNave, vector<NaveJugador*> jugadores){
    int ok = -1;
    if (distanciaNave > DISTANCIA_DE_COMBATE){
        if (nave->getPosX() < getPosX()) ok = mover(-2, 0, jugadores);
        if (nave->getPosX() > getPosX()) ok = mover(2, 0, jugadores);
    } else{
        if (nave->getPosX() < getPosX()) ok = mover(1, 0, jugadores);
        if (nave->getPosX() > getPosX()) ok = mover(-1, 0, jugadores);
    }

    return ok;
}

int NaveEnemiga::acomodarseEnEjeY(int navePosY, vector<NaveJugador*> jugadores){
    int ok = -1;
    if (getPosY() < navePosY) ok = mover(0, 2, jugadores);
    else ok = mover(0, -2, jugadores);
    return ok;
}

bool NaveEnemiga::seDisparo(){
    return disparo;
}

void NaveEnemiga::reiniciarDisparo(){
    fireRate.iniciar();
}

void NaveEnemiga::destruirNave(){
    radio=0;
    mPosX=0;
    mPosY=0;
    mVelX=0;
    mVelY=0;
}

int NaveEnemiga::obtenerNaveSeguidaRandom(int cant_naves){
    return randomNumber() % cant_naves;
}

int NaveEnemiga::obtenerNaveSeguidaPonderada(vector<NaveJugador*> jugadores){
    int cant_jug = jugadores.size();
    int ponderacion[cant_jug];
    for(int i=0; i<cant_jug; i++){
        int score = jugadores[i]->getScore();
        int vida = jugadores[i]->getVidas();

        ponderacion[i] = score / vida;
    }
    int idx = 0;
    int min_pond = ponderacion[idx];
    for(int j=1; j<cant_jug; j++){
        if(ponderacion[j] < min_pond){
            min_pond = ponderacion[j];
            idx = j;
        }
    }
    return idx;
}

int NaveEnemiga::obtenerNaveSeguidaMasCercana(vector<NaveJugador*> jugadores){
    int min_dist = getDistanciaNave(jugadores[0]);
    int idx = 0;
    int cant_jug = jugadores.size();
    for(int i=1; i<cant_jug; i++){
        int dist_new = getDistanciaNave(jugadores[i]);
        if(min_dist > dist_new){
            min_dist = dist_new;
            idx = i;
        }
    }
    return idx;
}

bool NaveEnemiga::cambioDeLado(){
    int distanciaNueva = 0;

    distanciaActual = distanciaNueva;
    return distanciaNueva < 0 && strcmp(imagenEspejo, "") != 0;
}

void NaveEnemiga::actualizarSprite(){
    char imagen[10];
    strcpy(imagen, imagenActual);
    strcpy(imagenActual, imagenEspejo);
    strcpy(imagenEspejo, imagen);
    strcpy(clave, imagenActual);
}

char* NaveEnemiga::getImagen(){
    return clave;
}

