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

		void renderBoom();

		bool isAlive();

        void die();

        bool boomAvailable();

        void set_animations(const char* tipo, const char* subtipo);
        void setEnergias(int actual, int total);
		void setEnergiasReconex(int actual, int total);

        static const int FRAMES_BOOM = 6;
        static const int FRAMES_HIT_RECEIVE = 5;
        static const int CANT_VIDAS_JUGADOR = 3;
        static const int CANT_VIDAS_ENEMIGO = 1;

    protected:

        //Las coordenadas X e Y de la Nave
		int mPosX, mPosY;

		//La velocidad de la Nave
		int mVelX, mVelY;

        TextureW gNaveTexture;
        TextureW textureBoom;
        TextureW textureVida;
        //TextureW textureHitReceive;

        SDL_Rect dataBoom[FRAMES_BOOM];
        //SDL_Rect dataHitReceive[FRAMES_HIT_RECEIVE];

        int framesBoom;
        int frame;
        int boomInfo;
        //int framesHitReceive;

        bool alive;
        bool boom;

        bool mode_test;

        int energia_actual;
        int energia_total;

        int vidas;
};

#endif /* NAVE_H_ */

