#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "global.h"
#include "ManejoDeSDL.h"
#include "ManejoDeLog.h"
#include "Escenario.h"
#include "Menu.h"

ManejoDeSDL sdl;
ManejoDeLog logger;
Menu menu;

int main( int argc, char* args[] ){
    if (logger.iniciarLog() && sdl.iniciarSDL()){
		if( !menu.cargarImagen() && logger.seDebeInformarError()) logger.informar("Error al cargar las imagenes del Menú!");
		else menu.procesar();
    }
	sdl.cerrar();
	logger.cerrar();
	return 0;
}
