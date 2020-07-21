#ifndef MISIL_H
#define MISIL_H

#include "global.h"
#include "NaveEnemiga.h"

class Misil{

    public:

        Misil( int x, int y, int id);
        int get_id();

        //Las dimensiones de la Nave del Jugador
        int id_nave;
		static const int MISIL_WIDTH = 80;
		static const int MISIL_HEIGHT = 34;

		//Velocidad de la Nave del Jugador
		static const int MISIL_VEL = 8;

		//Mueve la Nave considerando la posicion de la Nave enemiga
		bool mover(vector<NaveEnemiga*> enemigos, int renderizados);

        int getAlto();

        int getAncho();

        int getVel();
        int getPosX();
        int getPosY();

    private:
        int alto;
        int ancho;
        int id;

        int vel;
        int x;
        int y;
};

#endif
