#include "global.h"
#include "Menu.h"
#include "Server.h"
#include "Client.h"

ManejoDeSDL sdl;
ManejoDeLog logger;
ManejoDeJson json;
Estado estado;

static int CANTIDAD_PARAMETROS_VALIDOS = 2;

int obtener_nivel_de_log(int cant_parametros, char* parametros[]);
int obtener_estado_aplicacion(int* args);

int main( int argc, char* argv[] ){

    if(!json.abrir_archivo()){
        logger.cerrar();
        return 0;
    }

    int nivel_log = obtener_nivel_de_log(argc, argv);
    if(nivel_log == 0) return 0;

    //Si el parámetro pasado no es válido avisa al usuario
    if (!logger.crearLogger(nivel_log)){
        logger.error("Nivel de log inexistente");
        logger.cerrar();
        return 0;
    }

    int args[3];
    if (obtener_estado_aplicacion(args) == -1){
        logger.error("No se pasó bien el estado de la aplicación");
        logger.cerrar();
        return 0;
    }

    if (estado.inicializar(args) == 1){
        logger.error("Error en la conexión entre servidor y cliente");
        logger.cerrar();
        return 0;
    }

    Menu menu = Menu();
    //Si inicia sdl y la carga de imagenes satisfactoriamente procede a mostrar el menú
    if (sdl.iniciarSDL() && menu.cargarImagen()) menu.procesar();

     //Libera la memoria allocada
    menu.cerrar();
	sdl.cerrar();
	logger.cerrar();
	return 0;
}

int obtener_nivel_de_log(int cant_parametros, char* parametros[]){
    int log_json = json.get_nivel_de_log();
    int nivel_log = 0;
    int log_param = 0;

    //Si no se pasó un solo parámetro avisa al usuario
    if(cant_parametros != CANTIDAD_PARAMETROS_VALIDOS){
        // No se paso ningun valor (x param ni x config.json)
        if(log_json == 0){
            logger.error("El nivel de log no ha sido especificado tanto en json como por parametro");
            logger.cerrar();
            return 0;
        }
        nivel_log = log_json;
    } else{
        log_param = atoi(parametros[1]);
        // Evalua cual es mayor (por ser niveles se queda con el mas grande)
        if(log_param < log_json) nivel_log = log_json;
        else nivel_log = log_param;
    }
    return nivel_log;
}

int obtener_estado_aplicacion(int* args){
    std::string estado_json = json.get_estado_aplicacion();
    if (strcmp(estado_json.c_str(), "server") == 0){
        estado = *new Server();
        args[1] = json.get_puerto();
        return 0;
    }else if(strcmp(estado_json.c_str(), "client") == 0){
        estado = *new Client();
        args[1] = json.get_ip();
        args[2] = json.get_puerto();
        return 0;
    }else{
        return -1;
    }
}
