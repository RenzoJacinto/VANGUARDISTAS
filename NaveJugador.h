#ifndef NAVEJUGADOR_H
#define NAVEJUGADOR_H

#include "Nave.h"
#include "NaveEnemiga.h"
#include <SDL2/SDL.h>

class NaveEnemiga;

class NaveJugador: public Nave
{
    public:

        NaveJugador( int x, int y, const char* imagen );

        //Las dimensiones de la Nave del Jugador
		static const int NAVE_WIDTH = 20;
		static const int NAVE_HEIGHT = 20;

		//Velocidad de la Nave del Jugador
		static const int NAVE_VEL = 7;

        //Toma las teclas oprimidas y ajusta la velocidad de la Nave
		void handleEvent( SDL_Event& e );

		//Mueve la Nave considerando la posicion de la Nave enemiga
		void mover( NaveEnemiga* enemigo );
};

#endif // NAVEJUGADOR_H
