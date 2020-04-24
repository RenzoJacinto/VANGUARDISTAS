#include "NaveEnemiga.h"
#include "ManejoDeSDL.h"
#include "global.h"

NaveEnemiga::NaveEnemiga( int x, int y )
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

	//Set collision circle size
	mColicionador.r = DOT_WIDTH / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

	//Move collider relative to the circle
	desplazarColicionador();
}


void NaveEnemiga::mover( NaveJugador* jugador )
{
    //Move the dot left or right
    setPosX(getPosX()-1);
	desplazarColicionador();

    //If the dot collided or went too far to the left or right
	if( checkCollision( getColicionador(), jugador->getColicionador() ) )
    {
        //Move back
        setPosX(getPosX()+1);
		desplazarColicionador();
    }


    if ( ( getPosX() - getColicionador().r < 0 ) || ( getPosX() + getColicionador().r > sdl.getScreenWidth() ) )
    {
        //Move back
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


void NaveEnemiga::renderizar()
{
    //Show the dot
	sdl.renderEnemigo(getPosX() - getColicionador().r, getPosY()- getColicionador().r);
}

