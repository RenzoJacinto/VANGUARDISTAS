#include "global.h"
#include "Menu.h"

ManejoDeSDL sdl;
ManejoDeLog logger;

int main( int argc, char* args[] ){
    Menu menu = Menu();
    if (logger.iniciarLog() && sdl.iniciarSDL()) menu.procesar();
    menu.cerrar();
	sdl.cerrar();
	logger.cerrar();
	return 0;
}
