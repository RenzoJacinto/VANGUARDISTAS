#include "global.h"
#include "Menu.h"

ManejoDeSDL sdl;
ManejoDeLog logger;

static int CANTIDAD_PARAMETROS_VALIDOS = 1;

int main( int argc, char* argv[] ){
    //Si no se pasó un solo parámetro avisa al usuario
    if(argc != CANTIDAD_PARAMETROS_VALIDOS+1){
        printf("Cantidad de parámetros inválida");
        return 0;
    }
    Menu menu = Menu();
    //Si el parámetro pasado no es válido avisa al usuario
    if (!logger.iniciarLog(atoi(argv[1]))){
        printf("Parámetro inválido");
        return 0;
    }
    //Si inicia sdl y la carga de imagenes satisfactoriamente procede a mostrar el menú
    if (sdl.iniciarSDL() && menu.cargarImagen()) menu.procesar();
    //Libera la memoria allocada
    menu.cerrar();
	sdl.cerrar();
	logger.cerrar();
	return 0;
}
