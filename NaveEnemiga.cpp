#include "NaveEnemiga.h"
#include "ManejoDeSDL.h"
#include "global.h"

NaveEnemiga::NaveEnemiga( int x, int y ){
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

	//Set collision circle size
	mColicionador.r = NAVE_WIDTH / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

	//Move collider relative to the circle
	desplazarColicionador();
	if(!gNaveTexture.loadFromFile("sprites/enemigo.png") && logger.seDebeInformarError()) logger.informar(SDL_GetError());
}


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

