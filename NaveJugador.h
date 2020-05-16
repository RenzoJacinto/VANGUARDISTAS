#ifndef NAVEJUGADOR_H
#define NAVEJUGADOR_H

#include "Nave.h"
#include "list"

class NaveEnemiga;

class NaveJugador: public Nave {

    public:

        NaveJugador( int x, int y);

        //Las dimensiones de la Nave del Jugador
		static const int NAVE_WIDTH = 50;
		static const int NAVE_HEIGHT = 20;

		//Velocidad de la Nave del Jugador
		static const int NAVE_VEL = 5;

        //Toma las teclas oprimidas y ajusta la velocidad de la Nave
		void handleEvent( SDL_Event& e );

		//Mueve la Nave considerando la posicion de la Nave enemiga
		void mover( list<NaveEnemiga*>  enemigo );

		void renderizar();

        int getAlto();

        int getAncho();

    private:

        int alto,ancho;
};

#endif // NAVEJUGADOR_H

