#ifndef NAVEENEMIGA_H
#define NAVEENEMIGA_H

#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "colicionador.h"

class NaveJugador;

class NaveEnemiga: public Nave
{
    public:

        //Las dimensiones de la Nave del enemigo
		static const int NAVE_WIDTH = 40;
		static const int NAVE_HEIGHT = 40;

		TextureW gNaveTexture;

        //Se inicializa la Nave en esas coordenadas
		NaveEnemiga( int x, int y );

		//Mueve la Nave considerando la posicion de la Nave del jugador
		void mover( NaveJugador* jugador );

		void renderizar();

};

#endif // NAVEENEMIGA_H
