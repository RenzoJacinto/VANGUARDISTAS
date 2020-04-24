#ifndef NAVEJUGADOR_H
#define NAVEJUGADOR_H

#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "colicionador.h"

class NaveEnemiga;

class NaveJugador: public Nave
{
    public:

        //Las dimensiones de la Nave del Jugador
		static const int NAVE_WIDTH = 20;
		static const int NAVE_HEIGHT = 20;

		//Velocidad de la Nave del Jugador
		static const int NAVE_VEL = 2;

        //Se inicializa la Nave en esas coordenadas
		NaveJugador( int x, int y );

        //Toma las teclas oprimidas y ajusta la velocidad de la Nave
		void handleEvent( SDL_Event& e );

		//Mueve la Nave considerando la posicion de la Nave enemiga
		void mover( NaveEnemiga* enemigo );

		void renderizar();

		TextureW gNaveTexture;

};

#endif // NAVEJUGADOR_H
