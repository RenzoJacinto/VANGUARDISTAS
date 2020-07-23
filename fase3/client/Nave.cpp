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

    alive = true;
    boom = false;

    std::string imagen = json.get_sprite_nave(tipo, subtipo, "nave");
    std::string mensaje = "La imagen (" + imagen + ") no fue encontrada, se carga la imagen que muestra el error";

    if(!gNaveTexture.loadFromFile(imagen)){
        logger.error(mensaje.c_str());
        imagen = json.get_imagen_default("nave");
        gNaveTexture.loadFromFile(imagen.c_str());
    }
    string dir(imagen);
    if(! textureBoom.loadFromFile(json.get_sprite_nave(tipo, subtipo, "boom")))
        logger.error("No se pudo cargar la textura de la explosion");
    else{
        mensaje = "Se cargo el boom con imagen: " + dir;
        logger.debug(mensaje.c_str());
    }

    mensaje = "Se cargo la nave con imagen: " + dir;
    logger.debug(mensaje.c_str());
    return true;
}

void Nave::cerrarNave(){
    gNaveTexture.free();
    textureBoom.free();
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
        return true;
    }

    //If the shapes have not collided
    return false;
}

//En caso de encontrar algun enemigo cerca, verifica si se produce colision y en caso negativo devuelve True
bool Nave::encontrarEnemigos( NaveJugador* jugador, vector<NaveEnemiga*>  enemigos ){
    bool colision = false;
    vector<NaveEnemiga*>::iterator pos;

    for(pos = enemigos.begin();pos != enemigos.end();pos++){
        colision = checkCollision( jugador , *pos );
        if (colision) break;
    }

    return colision;
}

void Nave::renderBoom(){
    int frames = 6;
    SDL_Rect dataBoom[frames];
    dataBoom[ 0 ].x =   0;
    dataBoom[ 0 ].y =   0;
    dataBoom[ 0 ].w =  134;
    dataBoom[ 0 ].h = 236;

    dataBoom[ 1 ].x =  134;
    dataBoom[ 1 ].y =   0;
    dataBoom[ 1 ].w =  144;
    dataBoom[ 1 ].h = 236;

    dataBoom[ 2 ].x = 278;
    dataBoom[ 2 ].y =   0;
    dataBoom[ 2 ].w =  182;
    dataBoom[ 2 ].h = 236;

    dataBoom[ 3 ].x = 460;
    dataBoom[ 3 ].y =   0;
    dataBoom[ 3 ].w =  232;
    dataBoom[ 3 ].h = 236;

    dataBoom[ 4 ].x = 692;
    dataBoom[ 4 ].y =   0;
    dataBoom[ 4 ].w =  142;
    dataBoom[ 4 ].h = 236;

    dataBoom[ 5 ].x = 834;
    dataBoom[ 5 ].y =   0;
    dataBoom[ 5 ].w =  166;
    dataBoom[ 5 ].h = 236;

    int actualFrame = 0;
    while(actualFrame/6 < frames){
        SDL_Rect* currentClip = &dataBoom[ actualFrame/6 ];
        int w = dataBoom[ actualFrame/6 ].w;
        int h = dataBoom[ actualFrame/6 ].h;
        textureBoom.render(mPosX-w/2, mPosY-h/2, currentClip );
        printf("XBOOM: %d, YBOOM: %d\n", mPosX, mPosY);
        SDL_RenderPresent( sdl.getRenderer() );
        actualFrame++;
    }

    boom = false;
}

bool Nave::isAlive(){
    return alive;
}

bool Nave::boomAvailable(){
    return boom;
}

void Nave::die(){
    alive = false;
    boom = true;
}

