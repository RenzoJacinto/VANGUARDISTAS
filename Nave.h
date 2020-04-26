#ifndef NAVE_H_
#define NAVE_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "ManejoDeSDL.h"
#include "global.h"
#include "colicionador.h"

class Nave{

    public:

        //Se inicializa la Nave en esas coordenadas
		void crearNave( int x, int y, const char* imagen , const int naveWidth);

		void renderizar();

		int getPosX();

		void setPosX(int n);

		int getPosY();

		void setPosY(int n);

        int getVelX();

		void setVelX(int n);

        int getVelY();

		void setVelY(int n);

		//Mueve el círculo de colisión en relación con el desplazamiento de la Nave
		void desplazarColicionador();

		Circle& getColicionador();

		bool checkCollision( Circle& a, Circle& b );

		double distanceSquared( int x1, int y1, int x2, int y2 );

    protected:

        //Las coordenadas X e Y de la Nave
		int mPosX, mPosY;

		//La velocidad de la Nave
		int mVelX, mVelY;

        //La Nave representada por un circula colisionador
		Circle mColicionador;

		TextureW gNaveTexture;

		static const int NAVE_WIDTH;
		static const int NAVE_HEIGHT;

};

#endif

