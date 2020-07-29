#include "global.h"
#include "Server.h"


ManejoDeLog logger;
ManejoDeJson json;
Server* server;
pthread_mutex_t mutex;


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
        free(server);
        return 0;
    }
    if(server != NULL) server->iniciar();
    else{
        logger.error("Parametros invalidos");
        logger.cerrar();
        free(server);
        return 0;
    }

    //Libera la memoria allocada
    logger.cerrar();
    free(server);
	return 0;
}

bool iniciar_conexion(char* args[], int argc){
    pthread_mutex_init(&mutex, NULL);
    bool ok = true;
    if(argc == 2){
        server = new Server(atoi(args[1]), mutex);
        logger.info("Se inicializo el server");
    } else{
        logger.error("Cantidad de parametros para el server invalida");
        ok = false;
    }
    return ok;
}
