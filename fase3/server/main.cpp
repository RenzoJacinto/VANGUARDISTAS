#include "global.h"
#include "Menu.h"
#include "Client.h"
#include "ManejoDeNiveles.h"

ManejoDeSDL sdl;
ManejoDeLog logger;
ManejoDeJson json;
Client* cliente;
pthread_mutex_t mutex;
ManejoDeSonidos sounds;

//static int CANTIDAD_PARAMETROS_VALIDOS = 2;

bool iniciar_conexion(char* argv[], int argc);

int main( int argc, char* argv[] ){

    if(!json.abrir_archivo()){
        logger.cerrar();
        return 0;
    }

    int nivel_log = json.get_nivel_de_log();
    logger.crearLogger(nivel_log);

    if(!iniciar_conexion(argv, argc)){
        logger.cerrar();
        free(cliente);
        return 0;
    }
    if(cliente != NULL) cliente->iniciar();
    else{
        logger.error("Parametros invalidos");
        logger.cerrar();
        free(cliente);
        return 0;
    }

    //Libera la memoria allocada
    logger.cerrar();
    free(cliente);
	return 0;
}

bool iniciar_conexion(char* args[], int argc){
    pthread_mutex_init(&mutex, NULL);
    bool ok = true;
    if(argc == 3) cliente = new Client(args[1], atoi(args[2]), mutex);
    else{
        logger.error("Cantidad de parametros para el cliente invalida");
        ok = false;
    }

    return ok;
}
