#ifndef MISIL_H
#define MISIL_H

#include "global.h"

class Misil{

    public:

        Misil( int x, int y, int id);
        int get_id();

        //Las dimensiones de la Nave del Jugador
        int id_nave;
		static const int MISIL_WIDTH = 80;
		static const int MISIL_HEIGHT = 34;

		//Velocidad de la Nave del Jugador
		static const int MISIL_VEL = 20;

		//Mueve la Nave considerando la posicion de la Nave enemiga
		void mover();

		bool renderizar();

        int getAlto();

        int getAncho();

        int getVel();

    private:
        TextureW misilTexture;
        int alto;
        int ancho;
        int id;

        int vel;
        int x;
        int y;
};

#endif
