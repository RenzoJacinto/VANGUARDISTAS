#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "global.h"

bool Nave::crearNave( int x, int y){
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

   //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    mode_test = false;
    isOn = true;
    return true;
}

int Nave::getPosX(){
    //pthread_mutex_lock(&mutex);
    return mPosX;
    //pthread_mutex_unlock(&mutex);
}


void Nave::setPosX(int num){
    //pthread_mutex_lock(&mutex);
    mPosX=num;
    //pthread_mutex_unlock(&mutex);
}

int Nave::getPosY(){
    //pthread_mutex_lock(&mutex);
    return mPosY;
    //pthread_mutex_unlock(&mutex);
}

void Nave::setPosY(int num){
    //pthread_mutex_lock(&mutex);
    mPosY=num;
    //pthread_mutex_unlock(&mutex);
}

int Nave::getVelX(){
    //pthread_mutex_lock(&mutex);
    return mVelX;
    //pthread_mutex_unlock(&mutex);
}

void Nave::setVelX(int num){
    //pthread_mutex_lock(&mutex);
    mVelX=num;
    //pthread_mutex_unlock(&mutex);
}

int Nave::getVelY(){
    //pthread_mutex_lock(&mutex);
    return mVelY;
    //pthread_mutex_unlock(&mutex);
}

void Nave::setVelY(int num){
    //pthread_mutex_lock(&mutex);
    mVelY=num;
    //pthread_mutex_unlock(&mutex);
}

double Nave::distanceSquared( int x1, int y1, int x2, int y2 ){
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

bool Nave::checkCollision( NaveJugador* jugador, NaveEnemiga* enemigo ){
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( enemigo->getPosX() < jugador->getPosX() ){
        cX = jugador->getPosX();
    } else if( enemigo->getPosX() > jugador->getPosX() + jugador->getAncho() ){
        cX = jugador->getPosX() + jugador->getAncho();
    } else{
        cX = enemigo->getPosX();
    }

    //Find closest y offset
    if( enemigo->getPosY() < jugador->getPosY() ){
        cY = jugador->getPosY();
    } else if( enemigo->getPosY() > jugador->getPosY() + jugador->getAlto() ){
        cY = jugador->getPosY() + jugador->getAlto();
    } else{
        cY = enemigo->getPosY();
    }

    if( ( enemigo->getPosX() > 0 ) && ( enemigo->getPosX() < SCREEN_WIDTH ) && ( enemigo->getPosY() > 0 ) && ( enemigo->getPosY() < SCREEN_HEIGHT ) ) {
        //If the closest point is inside the circle
        if( distanceSquared( enemigo->getPosX(), enemigo->getPosY(), cX, cY ) < enemigo->getRadio() * enemigo->getRadio() ){
            //This box and the circle have collided
            return true;
        }
    }

    //If the shapes have not collided
    return false;
}

void Nave::die(){
    if(!mode_test && vidas>0){
        vidas--;
        //if(vidas != 0) energia_actual = energia_total;
        mPosX = 200;
        mPosY = 100 * (id_nave+1);
        std::cout<<"colision\n";
    }
    startWaiting();
}


bool Nave::impacto_misil(int x_misil, int y_misil, int ancho_misil, int alto_misil, int damage){

    if(energia_actual <= 0) energia_actual = energia_total;

    if(! isAlive()) return false;
    bool ok = false;

    bool a = (x_misil >= mPosX - radio && x_misil <= mPosX - radio + ancho);
    bool b = (x_misil+ancho_misil >= mPosX - radio && x_misil+ancho_misil <= mPosX - radio + ancho);

    bool c = (y_misil >= mPosY - radio && y_misil <= mPosY - radio + alto);
    bool d = (y_misil+alto_misil >= mPosY - radio && y_misil+alto_misil <= mPosY - radio + alto);

    bool colisionX = (a || b);
    bool colisionY = (c || d);

    //printf("XM: %d, YM: %d, XN: %d - %d, YN: %d - %d\n", x_misil, y_misil, mPosX, mPosX+ancho, mPosY, mPosY+alto);
    if(colisionX && colisionY){
        if(!mode_test){
            energia_actual -= damage;
            printf("energia actual: %d\n", energia_actual);
            if(energia_actual <= 0){
                die();
            }
        }
        ok = true;
    }
    //printf("termina\n");
    return ok;
}

bool Nave::isAlive(){
    return (vidas > 0 && isOn) //&& !isWaiting();
}

int Nave::getVidaActual(){
    return energia_actual;
}

int Nave::getVidaTotal(){
    return energia_total;
}

int Nave::getScore(){
    return score;
}

int Nave::get_damage(){
    return damage;
}

int Nave::getVidas(){
    return vidas;
}

int Nave::getRadio(){
    return radio;
}

void Nave::desconectar(){
    printf("desconecto\n");
    isOn = false;
}

void Nave::conectar(){
    if(!isOn){
        isOn = true;
        printf("conecto\n");
    }
}

bool Nave::isWaiting(){
    return wait;
}

void Nave::startWaiting(){
    wait = true;
}

void Nave::stopWaiting(){
    wait = false;
}
