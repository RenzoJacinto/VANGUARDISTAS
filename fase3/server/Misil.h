#ifndef MISIL_H
#define MISIL_H

#include "global.h"
#include "NaveEnemiga.h"

class Misil{

    public:

        Misil( int x, int y, int id, int danio);
        int get_id();

        //Las dimensiones de la Nave del Jugador
        int id_nave;
		static const int MISIL_WIDTH = 33;
		static const int MISIL_HEIGHT = 20;

		//Velocidad de la Nave del Jugador
		static const int MISIL_VEL = 15;

		//Mueve la Nave considerando la posicion de la Nave enemiga
		int mover(vector<NaveEnemiga*> enemigos, int renderizados);
		int mover(vector<NaveJugador*> jugadores);

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

        int damage;
};

#endif
