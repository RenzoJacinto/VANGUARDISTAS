#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

#include "Nave.h"

class NaveJugador;

class NaveEnemiga: public Nave {

    public:
		//Mueve la Nave considerando la posicion de la Nave del jugador
		void mover( NaveJugador* jugador );

		void renderizar();

		int getRadio();

        int radio;
};

#endif // NAVEENEMIGA_H

