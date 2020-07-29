#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

#include "Nave.h"

class NaveJugador;

class NaveEnemiga: public Nave {

    public:

        NaveEnemiga(int x, int y, const char* sprite);

		//Mueve la Nave considerando la posicion de la Nave del jugador
		void mover( NaveJugador* jugador );

		void renderizar();

		int getRadio();

		void setImagen(const char* img);

		int getAltoImagen();

		int getAnchoImagen();

		char* getClave();

        int getScore();

    protected:

        int radio;

        // izq->der = 1 ; der->izq = -1
        int desplazamiento;
        char clave[15];
        int score;
        const char* sEnemigo1;
        const char* sEnemigo2;
        const char* sEnemigo3;
        const char* sEnemigo4;
};

#endif // NAVEENEMIGA_H

