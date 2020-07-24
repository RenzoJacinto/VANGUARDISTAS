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

		const char* getImagen();

		int getAltoImagen();

		int getAnchoImagen();

		char* getClave();

		void setEnergias(int actual, int total);

		void setEnergiasReconex(int actual, int total);

    protected:

        int radio;

        const char* imagen;

        int energia_actual;
        int energia_total;

        TextureW textureVida;

        // izq->der = 1 ; der->izq = -1
        int desplazamiento;
        char clave[15];
        const char* sEnemigo1;
        const char* sEnemigo2;
        const char* sEnemigo3;
        const char* sEnemigo4;
};

#endif // NAVEENEMIGA_H

