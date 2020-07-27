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

void NaveEnemiga::mover( NaveJugador* jugador ){

    if(!isAlive()) return;
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
