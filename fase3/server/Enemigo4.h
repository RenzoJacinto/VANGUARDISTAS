#ifndef ENEMIGO4_H
#define ENEMIGO4_H

#include "NaveEnemiga.h"

class Enemigo4: public NaveEnemiga {

    public:

        Enemigo4(int x, int y);

		int procesarAccion(vector<NaveJugador*> jugadores);
};

#endif // NAVEENEMIGA_H


