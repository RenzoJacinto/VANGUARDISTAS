#include "NaveJugador.h"
#include "global.h"

NaveJugador::NaveJugador( int x, int y )
{

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
	if(!gNaveTexture.loadFromFile("sprites/nave.png") && logger.seDebeInformarError()) logger.informar(SDL_GetError());
}


void NaveJugador::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= NAVE_VEL; break;
            case SDLK_DOWN: mVelY += NAVE_VEL; break;
            case SDLK_LEFT: mVelX -= NAVE_VEL; break;
            case SDLK_RIGHT: mVelX += NAVE_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += NAVE_VEL; break;
            case SDLK_DOWN: mVelY -= NAVE_VEL; break;
            case SDLK_LEFT: mVelX += NAVE_VEL; break;
            case SDLK_RIGHT: mVelX -= NAVE_VEL; break;
        }
    }
}


void NaveJugador::mover( NaveEnemiga* enemigo ){
    //Move the dot left or right
    setPosX(getPosX()+getVelX());
	desplazarColicionador();

	if( ( getPosX() - getColicionador().r < 0 ) || ( getPosX() + getColicionador().r > sdl.getScreenWidth() )  || checkCollision( getColicionador(), enemigo->getColicionador() ) ){
        setPosX(getPosX()-getVelX());
		desplazarColicionador();
    }

    setPosY(getPosY()+getVelY());
	desplazarColicionador();

    if( ( getPosY() - getColicionador().r < 0 ) || ( getPosY() + getColicionador().r > sdl.getScreenHeight() ) || checkCollision( getColicionador(), enemigo->getColicionador() ) ){
        //Move back
        setPosY(getPosY()-getVelY());
		desplazarColicionador();
    }
}
