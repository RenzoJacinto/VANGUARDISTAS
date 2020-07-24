#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite){
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
        energia_actual = 100;
        energia_total = 100;
    } else{
        alto = 63;
        ancho = 63;
        energia_actual = 200;
        energia_total = 200;
    }

    //clave[0] = 0;
    strcpy(clave, sprite);
    radio=alto/2;
    alive = true;
    mensaje = "<<<< SE CARGO LA NAVE " + sp;
    logger.info(mensaje.c_str());
}

void NaveEnemiga::mover( NaveJugador* jugador ){

    if(!alive) return;
    setPosX(getPosX()+desplazamiento);

    if( checkCollision( jugador , this ) ){
        setPosX(getPosX()-desplazamiento);
    }
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

bool NaveEnemiga::impacto_misil(int x_misil, int y_misil, int ancho_misil, int alto_misil){

    if(!alive) return false;
    bool ok = false;

    bool a = (x_misil >= mPosX - radio && x_misil <= mPosX - radio + ancho);
    bool b = (x_misil+ancho_misil >= mPosX - radio && x_misil+ancho_misil <= mPosX - radio + ancho);

    bool c = (y_misil >= mPosY - radio && y_misil <= mPosY - radio + alto);
    bool d = (y_misil+alto_misil >= mPosY - radio && y_misil+alto_misil <= mPosY - radio + alto);

    bool colisionX = (a || b);
    bool colisionY = (c || d);

    //printf("XM: %d, YM: %d, XN: %d - %d, YN: %d - %d\n", x_misil, y_misil, mPosX, mPosX+ancho, mPosY, mPosY+alto);

    if(colisionX && colisionY)
    {
        energia_actual -= 50;
        if(energia_actual <= 0) alive = false;
        std::cout<<"IMPACTO EL PIBE\n";
        ok = true;
    }

    return ok;
}

bool NaveEnemiga::isAlive()
{
    return energia_actual > 0;
}

int NaveEnemiga::getVidaActual()
{
    return energia_actual;
}

int NaveEnemiga::getVidaTotal()
{
    return energia_total;
}
