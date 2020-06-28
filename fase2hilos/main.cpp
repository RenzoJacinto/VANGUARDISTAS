#include "global.h"
#include "Menu.h"
#include "Server.h"
#include "Client.h"
#include "ManejoDeNiveles.h"

ManejoDeSDL sdl;
ManejoDeLog logger;
ManejoDeJson json;
Estado* estado;
pthread_mutex_t mutex;

//static int CANTIDAD_PARAMETROS_VALIDOS = 2;

bool iniciar_conexion(char* argv[]);

int main( int argc, char* argv[] ){

    if(!json.abrir_archivo()){
        logger.cerrar();
        return 0;
    }

    int nivel_log = json.get_nivel_de_log();
    logger.crearLogger(nivel_log);

    if(!iniciar_conexion(argv)){
        logger.cerrar();
        return 0;
    }

    std::string estado_json = json.get_estado_conexion();
    if(strcmp(estado_json.c_str(), "server") == 0) {
        estado -> iniciar();
        ManejoDeNiveles niveles;
        niveles.procesar_servidor();
    }
    else{
        if(sdl.iniciarSDL()) estado->iniciar();
        Menu menu = Menu();
        //Si inicia sdl y la carga de imagenes satisfactoriamente procede a mostrar el menú
        if (menu.cargarImagen()) menu.procesar();

         //Libera la memoria allocada
        menu.cerrar();
        sdl.cerrar();
        logger.cerrar();
	}
	free(estado);
	return 0;
}


bool iniciar_conexion(char* args[]){
    pthread_mutex_init(&mutex, NULL);
    std::string estado_json = json.get_estado_conexion();
    if (strcmp(estado_json.c_str(), "server") == 0){
        estado = new Server(atoi(args[1]), mutex);
        return true;
    } else if(strcmp(estado_json.c_str(), "client") == 0){
        printf("es cliente\n");
        estado = new Client(args[1], atoi(args[2]), mutex);
        return true;
    }else{
        logger.error("El estado obtenido es inexistente");
        return false;
    }
}
