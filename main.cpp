#include "global.h"
#include "Menu.h"

ManejoDeSDL sdl;
ManejoDeLog logger;

int main( int argc, char* argv[] ){
    if(argc != 2) logger.debug("Cantidad de parametros incorrecta");
    Menu menu = Menu();
    if (logger.iniciarLog(atoi(argv[1])) && sdl.iniciarSDL() && menu.cargarImagen()) menu.procesar();
    menu.cerrar();
	sdl.cerrar();
	logger.cerrar();
	return 0;
}
