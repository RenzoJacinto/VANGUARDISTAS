#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "global.h"

bool Nave::crearNave( int x, int y, const char* tipo, const char* subtipo ){
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

   //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    if(strcmp(estado->est, "server") == 0) return true;
    std::string imagen = json.get_sprite_nave(tipo, subtipo);
    std::string mensaje = "La imagen (" + imagen + ") no fue encontrada, se carga la imagen que muestra el error";

    if(!gNaveTexture.loadFromFile(imagen)){
        logger.error(mensaje.c_str());
        imagen = json.get_imagen_default("nave");
        gNaveTexture.loadFromFile(imagen.c_str());
    }
    string dir(imagen);
    mensaje = "Se cargo la nave con imagen: " + dir;
    logger.debug(mensaje.c_str());
    return true;
}

void Nave::cerrarNave(){
    gNaveTexture.free();
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

    //If the closest point is inside the circle
    if( distanceSquared( enemigo->getPosX(), enemigo->getPosY(), cX, cY ) < enemigo->getRadio() * enemigo->getRadio() ){
        //This box and the circle have collided
        return false;
    }

    //If the shapes have not collided
    return false;
}

//En caso de encontrar algun enemigo cerca, verifica si se produce colision y en caso negativo devuelve True
bool Nave::encontrarEnemigos( NaveJugador* jugador, vector<NaveEnemiga*>  enemigos )
{
    bool colision = false;
//    vector<NaveEnemiga*>::iterator pos;
//
//    for(pos = enemigos.begin();pos != enemigos.end();pos++)
//    {
//        colision = checkCollision( jugador , *pos );
//        if (colision) break;
//    }

    return colision;
}

