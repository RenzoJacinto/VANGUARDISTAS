#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite){
    std::string sp(sprite);
    std::string mensaje =  ">>>> CARGANDO LA NAVE " + sp + " ....";
    logger.info(mensaje.c_str());
    if(crearNave(x, y, "enemigas", sprite)){
        mensaje = "Se creo el " + sp;
        logger.debug(mensaje.c_str());
    }
    if(x<=0) desplazamiento = 1;
    else desplazamiento = -1;

    radio=getAltoImagen()/2;
    mensaje = "<<<< SE CARGO LA NAVE " + sp;
    logger.info(mensaje.c_str());
}

void NaveEnemiga::mover(){

    setPosX(getPosX()+desplazamiento);

    /*if( checkCollision( jugador , this ) ){
        setPosX(getPosX()-desplazamiento);
    }*/
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

TextureW NaveEnemiga::get_texture(){
    return gNaveTexture;
}
