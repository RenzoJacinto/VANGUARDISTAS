#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite){
    if(crearNave(x, y, "enemigas", sprite)){
        std::string sp(sprite);
        std::string mensaje = "Se creo el enemigo con imagen: " + sp;
        logger.info(mensaje.c_str());
    }
    if(x<=0) desplazamiento = 1;
    else desplazamiento = -1;

    radio=getAltoImagen()/2;
}

void NaveEnemiga::mover( NaveJugador* jugador ){

    setPosX(getPosX()+desplazamiento);

    if( checkCollision( jugador , this ) ){
        setPosX(getPosX()-desplazamiento);
    }
}

void NaveEnemiga::renderizar(){

    gNaveTexture.render(getPosX() - getRadio(), getPosY() - getRadio());
}

int NaveEnemiga::getRadio(){
    return radio;
}

const char* NaveEnemiga::getImagen(){
    return imagen;
}

int NaveEnemiga::getAltoImagen(){
	return gNaveTexture.getHeight();
}

int NaveEnemiga::getAnchoImagen(){
	return gNaveTexture.getWidth();
}
