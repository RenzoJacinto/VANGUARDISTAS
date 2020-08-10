#ifndef ENEMIGO3_H
#define ENEMIGO3_H

#include "NaveEnemiga.h"

class Enemigo3: public NaveEnemiga {

    public:
        ~Enemigo3();
        Enemigo3(int x, int y);

		int procesarAccion(vector<NaveJugador*> jugadores);
};

#endif // NAVEENEMIGA_H


