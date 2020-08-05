#ifndef NAVEJUGADOR_H
#define NAVEJUGADOR_H

#include "Nave.h"
#include "list"
#include "NaveEnemiga.h"

//class NaveEnemiga;

class NaveJugador: public Nave {

    public:

        NaveJugador( int x, int y, int id);
        int get_id();

        //Las dimensiones de la Nave del Jugador
		static const int NAVE_WIDTH = 80;
		static const int NAVE_HEIGHT = 34;

		//Velocidad de la Nave del Jugador
		static const int NAVE_VEL = 5;

		//Mueve la Nave considerando la posicion de la Nave enemiga
		int mover( vector<NaveEnemiga*>  enemigo );

        int getAlto();

        int getAncho();

        //void desconectar();

        //bool isOn();

        //void conectar();

        void set_modeTest();

        void addScore(int puntaje);
        void setVidas(int num);
        int encontrarEnemigos(vector<NaveEnemiga*> enemigos);

        bool inModeTest();

    private:

        //bool conectado;
};

#endif // NAVEJUGADOR_H

