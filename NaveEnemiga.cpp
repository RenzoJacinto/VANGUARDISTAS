#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga(int x, int y, const char* sprite){
    crearNave(x, y, sprite);
    radio=getAltoImagen()/2;
}

void NaveEnemiga::mover( NaveJugador* jugador ){

    if (!desplazamientoPositivo) {

        setPosX(getPosX()-1);

        if( checkCollision( jugador , this ) ){
        setPosX(getPosX()+1);
        }

        if ( getPosX() - getRadio() < 0 ) {
            int posicionX = sdl.getScreenWidth();
            int posicionY = rand() % sdl.getScreenHeight();
            setPosX( posicionX );
            setPosY( posicionY );
        }

    } else {

        setPosX(getPosX()+1);

        if( checkCollision( jugador , this ) ){
            setPosX(getPosX()-1);
        }

        if ( getPosX() + getRadio() > sdl.getScreenWidth() ){
            int posicionX = 0;
            int posicionY = rand() % sdl.getScreenHeight();
            setPosX( posicionX );
            setPosY( posicionY );
        }
    }

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
