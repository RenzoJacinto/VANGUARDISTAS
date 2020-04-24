#include "NaveJugador.h"
#include "ManejoDeSDL.h"
#include "global.h"


NaveJugador::NaveJugador( int x, int y )
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


void NaveJugador::handleEvent( SDL_Event& e )
{
    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}


void NaveJugador::mover( NaveEnemiga* enemigo )
{
    //Move the dot left or right
    setPosX(getPosX()+getVelX());
	desplazarColicionador();

    //If the dot collided or went too far to the left or right
	if( ( getPosX() - getColicionador().r < 0 ) || ( getPosX() + getColicionador().r > sdl.getScreenWidth() )  || checkCollision( getColicionador(), enemigo->getColicionador() ) )
    {
        //Move back
        setPosX(getPosX()-getVelX());
		desplazarColicionador();
    }

    //Move the dot up or down
    setPosY(getPosY()+getVelY());
	desplazarColicionador();

    //If the dot collided or went too far up or down
    if( ( getPosY() - getColicionador().r < 0 ) || ( getPosY() + getColicionador().r > sdl.getScreenHeight() ) || checkCollision( getColicionador(), enemigo->getColicionador() ) )
    {
        //Move back
        setPosY(getPosY()-getVelY());
		desplazarColicionador();
    }
}


void NaveJugador::renderizar()
{
    //Show the dot
	sdl.renderNave(getPosX() - getColicionador().r, getPosY()- getColicionador().r );
}
