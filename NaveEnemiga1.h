#ifndef NAVEENEMIGA1_H
#define NAVEENEMIGA1_H

#include "NaveEnemiga.h"

class NaveEnemiga1: public NaveEnemiga {
      public:

        //Las dimensiones de la Nave del enemigo
		static const int NAVE_WIDTH = 50;
		static const int NAVE_HEIGHT = 50;

		const char* imagen = "sprites/enemigo1.png";

        NaveEnemiga1(int x, int y);
};

#endif // NAVEENEMIGA1_H

