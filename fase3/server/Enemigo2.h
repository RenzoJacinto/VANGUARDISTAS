#ifndef ENEMIGO2_H
#define ENEMIGO2_H

#include "NaveEnemiga.h"

class Enemigo2: public NaveEnemiga {

    public:
        ~Enemigo2();
        Enemigo2(int x, int y);

		int procesarAccion(vector<NaveJugador*> jugadores);

};

#endif // NAVEENEMIGA_H


