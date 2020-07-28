#ifndef ENEMIGO4_H
#define ENEMIGO4_H

#include "NaveEnemiga.h"

class Enemigo4: public NaveEnemiga {

    public:

        Enemigo4(int x, int y, const char* sprite);

		void setNaveSeguida(int id);

		int getNaveSeguida();

		void procesarAccion(NaveJugador* nave);

		int getDistanciaNave(NaveJugador* nave);

		void seguirNave(NaveJugador* nave, int distanciaNave);

		void acomodarseEnEjeY(int posY);

		void setJugadores(vector<NaveJugador*> listaJugadores);

		bool seDisparo();

		void reiniciarDisparo();

		void destruirNave();

};

#endif // NAVEENEMIGA_H


