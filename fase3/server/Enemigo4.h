#ifndef ENEMIGO4_H
#define ENEMIGO4_H

#include "NaveEnemiga.h"

class Enemigo4: public NaveEnemiga {

    public:

        Enemigo4(int x, int y);

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


