#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

#include "Nave.h"

class NaveJugador;

class NaveEnemiga: public Nave {

    public:

        NaveEnemiga(int x, int y);

		//Mueve la Nave considerando la posicion de la Nave del jugador
		void mover( NaveJugador* jugador );

		void renderizar();

		int getRadio();

		void seleccionarImagen();

		const char* getImagen();

		int getAltoImagen();

		int getAnchoImagen();

    protected:

        int radio;

        const char* imagen;

        bool desplazamientoPositivo;
};

#endif // NAVEENEMIGA_H

