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
    return mPosX;
}


void Nave::setPosX(int num){
    mPosX=num;
}

int Nave::getPosY(){
    return mPosY;
}

void Nave::setPosY(int num){
    mPosY=num;
}

int Nave::getVelX(){
    return mVelX;
}

void Nave::setVelX(int num){
    mVelX=num;
}

int Nave::getVelY(){
    return mVelY;
}

void Nave::setVelY(int num){
    mVelY=num;
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
        return true;
    }

    //If the shapes have not collided
    return false;
}

//En caso de encontrar algun enemigo cerca, verifica si se produce colision y en caso negativo devuelve True
bool Nave::encontrarEnemigos( NaveJugador* jugador, vector<NaveEnemiga*>  enemigos )
{
    bool colision = false;
    vector<NaveEnemiga*>::iterator pos;

    for(pos = enemigos.begin();pos != enemigos.end();pos++)
    {
        colision = checkCollision( jugador , *pos );
        if (colision) break;
    }

    return colision;
}
