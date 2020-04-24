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
	//gDotTexture.render( getPosX() - getColicionador().r, getPosY()- getColicionador().r );
}

Circle& NaveEnemiga::getColicionador()
{
	return mColicionador;
}


void NaveEnemiga::desplazarColicionador()
{
	//Align collider to center of dot
	getColicionador().x = getPosX();
	getColicionador().y = getPosY();
}


int NaveEnemiga::getPosX()
{
    return mPosX;
}

void NaveEnemiga::setPosX(int num)
{
    mPosX=num;
}

int NaveEnemiga::getPosY()
{
    return mPosY;
}

void NaveEnemiga::setPosY(int num)
{
    mPosY=num;
}


bool NaveEnemiga::checkCollision( Circle& a, Circle& b )
{
	//Calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

double NaveEnemiga::distanceSquared( int x1, int y1, int x2, int y2 )
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}
