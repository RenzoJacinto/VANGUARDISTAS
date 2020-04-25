#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

#include "Nave.h"
#include "NaveJugador.h"

class NaveJugador;

class NaveEnemiga: public Nave
{
    public:

        NaveEnemiga( int x, int y, const char* imagen );

        //Las dimensiones de la Nave del enemigo
		static const int NAVE_WIDTH = 40;
		static const int NAVE_HEIGHT = 40;

		//Mueve la Nave considerando la posicion de la Nave del jugador
		void mover( NaveJugador* jugador );

};

#endif // NAVEENEMIGA_H
