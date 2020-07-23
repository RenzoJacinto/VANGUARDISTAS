#ifndef NAVE_H_
#define NAVE_H_

#include <stdio.h>
#include <string>
#include "global.h"
#include "list"

class NaveJugador;
class NaveEnemiga;

class Nave{

    public:

        //Se inicializa la Nave en esas coordenadas
		bool crearNave( int x, int y);

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


    protected:

        //Las coordenadas X e Y de la Nave
		int mPosX, mPosY;

		int energia_actual;
		int energia_total;
		//La velocidad de la Nave
		int mVelX, mVelY;

};

#endif /* NAVE_H_ */

