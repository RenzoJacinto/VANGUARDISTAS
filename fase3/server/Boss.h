#ifndef BOSS_H
#define BOSS_H

#include "NaveEnemiga.h"

class Boss: public NaveEnemiga {

    public:

        Boss(int x, int y);

		int procesarAccion(vector<NaveJugador*> jugadores);

};

#endif // NAVEENEMIGA_H
