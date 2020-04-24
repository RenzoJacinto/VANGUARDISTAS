#include "Nave.h"


Nave::Nave() {}


void Nave::mover(){}


void Nave::renderizar(){}


int Nave::getPosX()
{
    return mPosX;
}


void Nave::setPosX(int num)
{
    mPosX=num;
}


int Nave::getPosY()
{
    return mPosY;
}


void Nave::setPosY(int num)
{
    mPosY=num;
}


int Nave::getVelX()
{
    return mVelX;
}


void Nave::setVelX(int num)
{
    mVelX=num;
}


int Nave::getVelY()
{
    return mVelY;
}


void Nave::setVelY(int num)
{
    mVelY=num;
}


void Nave::desplazarColicionador()
{
	//Align collider to center of dot
	getColicionador().x = getPosX();
	getColicionador().y = getPosY();
}


Circle& Nave::getColicionador()
{
	return mColicionador;
}


bool Nave::checkCollision( Circle& a, Circle& b )
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


double Nave::distanceSquared( int x1, int y1, int x2, int y2 )
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}
