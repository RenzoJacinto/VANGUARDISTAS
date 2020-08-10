#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

#include "Nave.h"
#include "NaveJugador.h"
#include "Temporizador.h"

class NaveJugador;

class NaveEnemiga: public Nave {

    public:

        NaveEnemiga();
        virtual ~NaveEnemiga();
        virtual int procesarAccion(vector<NaveJugador*> jugadores);

		int getAltoImagen();
		int getAnchoImagen();
		char* getClave();

        // Distancias entre naves
		int getDistanciaNaveEnX(NaveJugador* nave);
		int getDistanciaNaveEnXConSigno(NaveJugador* nave);
		int getDistanciaNave(NaveJugador* nave);

        //movimientos
		int seguirNave(NaveJugador* nave, int distanciaNave, vector<NaveJugador*> jugadores);
		int acomodarseEnEjeY(int posY, vector<NaveJugador*> jugadores);
		//Mueve la Nave considerando la posicion de la Nave del jugador
		int mover(int velX, int velY, vector<NaveJugador*> jugadores);

		bool seDisparo();

		void reiniciarDisparo();

		void destruirNave();
        bool esBoss();
        //retorno de ids
		int obtenerNaveSeguidaRandom(vector<NaveJugador*> jugadores, int cant_naves);
        int obtenerNaveSeguidaPonderada(vector<NaveJugador*> jugadores);
        int obtenerNaveSeguidaMasCercana(vector<NaveJugador*> jugadores);
        int naveDerechaCercana(vector<NaveJugador*> jugadores);
        bool cambioDeLado();
        bool isTurret();
        bool onScreen();
        void actualizarSprite();

        char* getImagen();

        int get_posX_misil();
        int get_posY_misil();\

        bool getDisparoTriple();

    protected:
        char imagenActual[10];
        char imagenEspejo[10];
        bool disparo;
        int nave_seguida;
        bool disparoTriple;

        int distanciaActual;
        int DISTANCIA_DE_COMBATE;
        int DISTANCIA_DE_COMBATE_INICIAL;
        Temporizador fireRate;
        Temporizador searchRate;
        // izq->der = 1 ; der->izq = -1
        char clave[15];
        bool turret;
        int vel;

        int misil_posX;
        int misil_posY;

};

#endif // NAVEENEMIGA_H

