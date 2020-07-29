#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

#include "Nave.h"
#include "NaveJugador.h"
#include "Temporizador.h"

class NaveJugador;

class NaveEnemiga: public Nave {

    public:

        NaveEnemiga(int x, int y, const char* sprite, vector<NaveJugador*> jugadores);

		//Mueve la Nave considerando la posicion de la Nave del jugador
		void mover(int velX, int velY);

		int getRadio();

		int getAltoImagen();

		int getAnchoImagen();

		char* getClave();

		void setNaveSeguida(int id);

		int getNaveSeguida();

		void procesarAccion(NaveJugador* nave);

		int getDistanciaNave(NaveJugador* nave);

		int getDistanciaNaveConSigno(NaveJugador* nave);

		void seguirNave(NaveJugador* nave, int distanciaNave);

		void acomodarseEnEjeY(int posY);

		void setJugadores(vector<NaveJugador*> listaJugadores);

		bool seDisparo();

		void reiniciarDisparo();

		void destruirNave();

		int obtenerNaveSeguidaRandom(int cant_naves);

        int obtenerNaveSeguidaPonderada();

        void actualizarSprite();

        char* getImagen();

    protected:
        char imagenActual[10];
        char imagenEspejo[10];
        int naveSeguida;
        bool disparo;

        int distanciaActual;
        int DISTANCIA_DE_COMBATE;
        int DISTANCIA_DE_COMBATE_INICIAL;
        Temporizador fireRate;
        // izq->der = 1 ; der->izq = -1
        char clave[15];

        vector<NaveJugador*> jugadores;


        /*const char* sEnemigo1;
        const char* sEnemigo2;
        const char* sEnemigo3;
        const char* sEnemigo4;*/
};

#endif // NAVEENEMIGA_H

