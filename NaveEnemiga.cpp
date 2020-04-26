#include "NaveEnemiga.h"

NaveEnemiga::NaveEnemiga( int x, int y, const char* imagen ){
    crearNave(x, y, imagen, NAVE_WIDTH);
    radio=NAVE_WIDTH/2;
}

void NaveEnemiga::mover( NaveJugador* jugador ){

    setPosX(getPosX()-1);
	//desplazarColicionador();


	if( checkCollision( jugador , this ) ){
        setPosX(getPosX()+1);
		//desplazarColicionador();
    }


    if ( ( getPosX() - getRadio() < 0 ) || ( getPosX() + getRadio() > sdl.getScreenWidth() ) ){
        int posicionX = sdl.getScreenWidth() - 20;
        int posicionY = rand() % sdl.getScreenHeight();
        setPosX( posicionX );
        setPosY( posicionY );
		//desplazarColicionador();
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

void NaveEnemiga::renderizar(){
	gNaveTexture.render(getPosX() - getRadio() , getPosY() - getRadio());
}

int NaveEnemiga::getRadio(){
    return radio;
}
