#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "global.h"
#include "ManejoDeSDL.h"
#include "ManejoDeLog.h"

ManejoDeSDL sdl;
ManejoDeLog logger;

int main( int argc, char* args[] ){
    if (logger.iniciarLog() && sdl.iniciarSDL()){
		if( !sdl.cargarImagenMenu() && logger.seDebeInformarError()) logger.informar("Error al cargar las imagenes del Menú!");
		else sdl.procesoMenu();
    }
	sdl.cerrar();
	logger.cerrar();
	return 0;
}
