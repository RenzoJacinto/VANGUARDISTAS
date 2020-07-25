#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite){
    std::string sp(sprite);
    std::string mensaje =  ">>>> CARGANDO LA NAVE " + sp + " ....";
    logger.info(mensaje.c_str());

    if(crearNave(x, y, "enemigas", sprite)){
        mensaje = "Se creo el " + sp;
        logger.debug(mensaje.c_str());
    }
    energia_actual = -1;
    energia_total = -1;

    vidas = CANT_VIDAS_ENEMIGO;

    if(strcmp(sprite, "enemigo3")==0 || strcmp(sprite, "enemigo4")==0) desplazamiento = 1;
    else desplazamiento = -1;

    clave[0] = 0;
    strncat(clave, sprite, 15);
    radio=getAltoImagen()/2;
    mensaje = "<<<< SE CARGO LA NAVE " + sp;
    logger.info(mensaje.c_str());
}

void NaveEnemiga::mover( NaveJugador* jugador ){

    setPosX(getPosX()+desplazamiento);

    if( checkCollision( jugador , this ) ){
        setPosX(getPosX()-desplazamiento);
    }
}

void NaveEnemiga::renderizar(){

    gNaveTexture.render(getPosX() - getRadio(), getPosY() - getRadio());
    int w = getAnchoImagen();
    if(energia_total != -1) w = (int) (w * energia_actual / energia_total);
    SDL_Rect vida = { mPosX-getRadio()+5, mPosY+getRadio()+6, w, 10 };
    SDL_SetRenderDrawColor( sdl.getRenderer(), 0x00, 0xFF, 0x00, 0xFF );
    SDL_RenderFillRect( sdl.getRenderer(), &vida );
    textureVida.render(mPosX-getRadio()+5, mPosY+getRadio()+6);
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

char* NaveEnemiga::getClave(){
    return clave;
}
