#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "ManejoDeSDL.h"
#include "Nave.h"
#include "TextureW.h"
#include "global.h"

ManejoDeSDL sdl;

int main( int argc, char* args[] ){

	if(!sdl.iniciarSDL()){
		printf( "Error al inicializar SDL!\n" );
	} else{
		if( !sdl.cargarImagenMenu() ) printf( "Error al cargar las imagenes del Menú!\n" );
		else sdl.procesoMenu();
    }
	sdl.cerrar();
	return 0;
}
