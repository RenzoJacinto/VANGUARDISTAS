#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

//#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "colicionador.h"

class NaveJugador;

class NaveEnemiga
{
    public:

        //Las dimensiones de la Nave del enemigo
		static const int DOT_WIDTH = 40;
		static const int DOT_HEIGHT = 40;

        //Se inicializa la Nave en esas coordenadas
		NaveEnemiga( int x, int y );

		//Mueve la Nave considerando la posicion de la Nave del jugador
		void mover( NaveJugador* jugador );

		void renderizar();

		Circle& getColicionador();

		//Mueve el círculo de colisión en relación con el desplazamiento de la Nave
		void desplazarColicionador();

		int getPosX();

		void setPosX(int n);

		int getPosY();

		void setPosY(int n);

		bool checkCollision( Circle& a, Circle& b );

		double distanceSquared( int x1, int y1, int x2, int y2 );

    private:

        //Las coordenadas X e Y de la Nave
		int mPosX, mPosY;

		//La velocidad de la Nave
		int mVelX, mVelY;

        //La Nave representada por un circula colisionador
		Circle mColicionador;

};

#endif // NAVEENEMIGA_H
