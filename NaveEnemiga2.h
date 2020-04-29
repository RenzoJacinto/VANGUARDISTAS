#ifndef NAVEENEMIGA2_H
#define NAVEENEMIGA2_H

#include "NaveEnemiga.h"

class NaveEnemiga2: public NaveEnemiga {
    public:

        //Las dimensiones de la Nave del enemigo
		static const int NAVE_WIDTH = 33;
		static const int NAVE_HEIGHT = 33;

		const char* imagen = "sprites/enemigo2.png";

        NaveEnemiga2(int x, int y);
};

#endif // NAVEENEMIGA2_H
