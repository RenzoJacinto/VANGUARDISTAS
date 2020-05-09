#include "global.h"
#include "Menu.h"

ManejoDeSDL sdl;
ManejoDeLog logger;
ManejoDeJson json;

static int CANTIDAD_PARAMETROS_VALIDOS = 2;

int main( int argc, char* argv[] ){

    json.abrir_archivo();
    int log_json = json.get_nivel_de_log();
    int nivel_log = 0;
    int log_param = 0;

    //Si no se pasó un solo parámetro avisa al usuario
    if(argc != CANTIDAD_PARAMETROS_VALIDOS){
        // No se paso ningun valor (x param ni x config.json)
        if(log_json == 0){
            printf("Cantidad de parámetros inválida");
            return 0;
        }
        nivel_log = log_json;
    } else{
        log_param = atoi(argv[1]);
        // Evalua cual es mayor (por ser niveles se queda con el mas grande)
        if(log_param < log_json) nivel_log = log_json;
        else nivel_log = log_param;
    }
    Menu menu = Menu();
    //Si el parámetro pasado no es válido avisa al usuario
    if (!logger.iniciarLog(nivel_log)){
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
