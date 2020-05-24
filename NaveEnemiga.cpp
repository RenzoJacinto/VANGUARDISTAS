#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite){
    if(crearNave(x, y, sprite)){
        std::string sp(sprite);
        std::string mensaje = "Se creo el enemigo con imagen: " + sp;
        logger.debug(mensaje.c_str());
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
    /*
    if ( getPosX() - getRadio() < 0 ) {
        int posicionX = sdl.getScreenWidth();
        int posicionY = rand() % sdl.getScreenHeight();
        setPosX( posicionX );
        setPosY( posicionY );
    }*/


        //Esta comentado por ahora, porque la nave solo se mueve en el eje X
//    //Move the dot up or down
//    mPosY += mVelY;
//	desplazarColicionador();
//
//    //If the dot collided or went too far up or down
//    if( ( getPosY() - getColicionador().r < 0 ) || ( getPosY() + getColicionador().r > SCREEN_HEIGHT ) || checkCollision( getColicionador(), enemigo.getCollider() ) )
//    {
//        //Move back
//        mPosY -= mVelY;
//		desplazarColicionador();
//    }
}

void NaveEnemiga::renderizar(){
    if(getPosX() >= 0 && getPosX() <= 800){
        gNaveTexture.render(getPosX() - getRadio(), getPosY() - getRadio());
	}
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
