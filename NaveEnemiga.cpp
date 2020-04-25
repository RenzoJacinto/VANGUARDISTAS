#include "NaveEnemiga.h"
#include "ManejoDeSDL.h"
#include "global.h"

void NaveEnemiga::mover( NaveJugador* jugador ){

    setPosX(getPosX()-1);
	desplazarColicionador();


	if( checkCollision( getColicionador(), jugador->getColicionador() ) ){
        setPosX(getPosX()+1);
		desplazarColicionador();
    }


    if ( ( getPosX() - getColicionador().r < 0 ) || ( getPosX() + getColicionador().r > sdl.getScreenWidth() ) ){
        int posicionX = sdl.getScreenWidth() - 20;
        int posicionY = rand() % sdl.getScreenHeight();
        setPosX( posicionX );
        setPosY( posicionY );
		desplazarColicionador();
    }

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

