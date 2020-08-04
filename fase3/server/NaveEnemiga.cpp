#include "NaveEnemiga.h"
#include "time_nanoseconds.h"

NaveEnemiga::NaveEnemiga(){
    id_nave = 0;
    turret = false;
}
int NaveEnemiga::procesarAccion(vector<NaveJugador*> jugadores){return -1;}

int NaveEnemiga::mover(int velX, int velY, vector<NaveJugador*> jugadores){

    /*if(!isAlive()){
        destruirNave();
        return;
    }*/

    setPosX(getPosX()+velX);
    setPosY(getPosY()+velY);

    for(unsigned int i = 0; i < jugadores.size(); ++i){
        if( checkCollision( jugadores[i] , this ) && jugadores[i]->isAlive()){
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
    if (abs(getPosY() - navePosY) > 2)
    {
        if (navePosY > mPosY) ok = mover(0, 2, jugadores);
        else ok = mover(0, -2, jugadores);
    }
    return ok;
}

bool NaveEnemiga::seDisparo(){
    return disparo;
}

void NaveEnemiga::reiniciarDisparo(){
    fireRate.finalizar();
    fireRate.iniciar();
}

void NaveEnemiga::destruirNave(){
    radio=0;
    mPosX=0;
    mPosY=0;
    mVelX=0;
    mVelY=0;
}

int NaveEnemiga::obtenerNaveSeguidaRandom(vector<NaveJugador*> jugadores, int cant_naves){
    int nave = randomNumber() % cant_naves;
    while(! jugadores[nave]->isAlive()) nave = randomNumber() % cant_naves;
    return nave;
}

int NaveEnemiga::obtenerNaveSeguidaPonderada(vector<NaveJugador*> jugadores){
    int cant_jug = jugadores.size();
    int ponderacion[cant_jug];
    for(int i=0; i<cant_jug; i++){
        int score = jugadores[i]->getScore();
        int vida = jugadores[i]->getVidas();
        if(vida > 0) ponderacion[i] = score / vida;
        else ponderacion[i] = -1;
    }
    int idx = 0;
    int min_pond = ponderacion[idx];
    for(int j=1; j<cant_jug; j++){
        if(ponderacion[j] < min_pond && ponderacion[j] != -1 && jugadores[j]->isAlive()){
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
        if(min_dist > dist_new && jugadores[i]->isAlive()){
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

bool NaveEnemiga::onScreen()
{
    bool a = mPosY > 0 && mPosY < SCREEN_HEIGHT;
    bool b = mPosX > 0 && mPosX < SCREEN_WIDTH;
    return a && b;
}

int NaveEnemiga::naveDerechaCercana(vector<NaveJugador*> jugadores)
{
    int min_dist = getDistanciaNave(jugadores[0]);
    int idx = 0;
    int x = jugadores[0]->getPosX()+80;
    bool hayNave = x > mPosX-radio;
    int cant_jug = jugadores.size();
    for(int i=1; i<cant_jug; i++){
        int dist_new = getDistanciaNave(jugadores[i]);
        int x = jugadores[i]->getPosX() + 80;
        if(x > mPosX-radio && min_dist > dist_new && jugadores[i]->isAlive()){
            min_dist = dist_new;
            hayNave = true;
            idx = i;
        }
    }
    if(!hayNave) return -1;
    return idx;
}

bool NaveEnemiga::isTurret()
{
    return turret;
}
