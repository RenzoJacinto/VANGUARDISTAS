#ifndef NAVEJUGADOR_H
#define NAVEJUGADOR_H

//#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "colicionador.h"

class NaveEnemiga;

class NaveJugador
{
    public:

        //Las dimensiones de la Nave del Jugador
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Velocidad de la Nave del Jugador
		static const int DOT_VEL = 7;

        //Se inicializa la Nave en esas coordenadas
		NaveJugador( int x, int y );

        //Toma las teclas oprimidas y ajusta la velocidad de la Nave
		void handleEvent( SDL_Event& e );

		//Mueve la Nave considerando la posicion de la Nave enemiga
		void mover( NaveEnemiga* enemigo );

		void renderizar();

		Circle& getColicionador();

		//Mueve el círculo de colisión en relación con el desplazamiento de la Nave
		void desplazarColicionador();

		int getPosX();

		void setPosX(int n);

		int getPosY();

		void setPosY(int n);

        int getVelX();

		void setVelX(int n);

        int getVelY();

		void setVelY(int n);

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

#endif // NAVEJUGADOR_H
