#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

#include "Nave.h"

class NaveJugador;

class NaveEnemiga: public Nave {

    public:

        NaveEnemiga( int x, int y, const char* imagen );

        //Las dimensiones de la Nave del enemigo
		static const int NAVE_WIDTH = 50;
		static const int NAVE_HEIGHT = 50;

		//Mueve la Nave considerando la posicion de la Nave del jugador
		void mover( NaveJugador* jugador );

		void renderizar();

		int getRadio();

    private:

        int radio;
};

#endif // NAVEENEMIGA_H

