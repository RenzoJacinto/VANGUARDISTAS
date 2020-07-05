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
#include "list"

class NaveJugador;
class NaveEnemiga;

class Nave{

    public:

        //Se inicializa la Nave en esas coordenadas
		bool crearNave( int x, int y, const char* tipo, const char* subtipo);

		void cerrarNave();

		int getPosX();

		void setPosX(int n);

		int getPosY();

		void setPosY(int n);

        int getVelX();

		void setVelX(int n);

        int getVelY();

		void setVelY(int n);

		double distanceSquared( int x1, int y1, int x2, int y2 );

		bool checkCollision( NaveJugador* jugador, NaveEnemiga* enemigo );

		bool encontrarEnemigos(NaveJugador* jugador, vector<NaveEnemiga*> enemigos);

        bool encontrarJugadores(NaveEnemiga* enemigo, vector<NaveJugador*> jugadores);

    protected:

        //Las coordenadas X e Y de la Nave
		int mPosX, mPosY;

		//La velocidad de la Nave
		int mVelX, mVelY;

        TextureW gNaveTexture;

};

#endif /* NAVE_H_ */

