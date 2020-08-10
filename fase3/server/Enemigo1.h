#ifndef ENEMIGO1_H
#define ENEMIGO1_H

#include "NaveEnemiga.h"

class Enemigo1: public NaveEnemiga {

    public:
        ~Enemigo1();
        Enemigo1(int x, int y);

		int procesarAccion(vector<NaveJugador*> jugadores);

};

#endif // NAVEENEMIGA_H


