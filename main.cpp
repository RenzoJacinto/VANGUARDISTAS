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

int main( int argc, char* args[] ){
    Menu menu = Menu();
    if (logger.iniciarLog() && sdl.iniciarSDL() && menu.cargarImagen()) menu.procesar();
    menu.cerrar();
	sdl.cerrar();
	logger.cerrar();
	return 0;
}
