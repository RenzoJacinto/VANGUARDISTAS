#ifndef ENEMIGO3_H
#define ENEMIGO3_H

#include "NaveEnemiga.h"

class Enemigo3: public NaveEnemiga {

    public:

        Enemigo3(int x, int y);

		int procesarAccion(vector<NaveJugador*> jugadores);
        bool isTurret();
		bool cambioSprite();
		void setTurret();
		void quitTurret();

    private:
        bool turret;
        bool spriteChanged;
};

#endif // NAVEENEMIGA_H


