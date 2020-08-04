#ifndef NAVEJUGADOR_H
#define NAVEJUGADOR_H

#include "Nave.h"
#include "list"
#include "Misil.h"

class NaveEnemiga;

class NaveJugador: public Nave {

    public:

        NaveJugador( int x, int y, int id, std::string id_user);
        int get_id();

        //Las dimensiones de la Nave del Jugador
        int id_nave;
		static const int NAVE_WIDTH = 80;
		static const int NAVE_HEIGHT = 34;

		//Velocidad de la Nave del Jugador
		static const int NAVE_VEL = 3;

        //Toma las teclas oprimidas y ajusta la velocidad de la Nave
		void handleEvent( SDL_Event& e , Mix_Music* gMusic, int* misil);

		//Mueve la Nave considerando la posicion de la Nave enemiga
		void mover( vector<NaveEnemiga*>  enemigo );

		void renderizar();

        int getAlto();

        int getAncho();

        void desconectar();

        bool isOn();

        void conectar();

        void addScore(int puntos);

        int getScore();

        void setScore(int puntos);
        void setVidas(int num);

        std::string get_name();
        int get_cant_vidas();

        void set_modeTest();

    private:

        int alto,ancho;
        bool conectado;
        int score;
        std::string id_cliente;
};

#endif // NAVEJUGADOR_H

