#include "Client.h"
#include "global.h"

#include "ColaMultihilo.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>


Client::Client(char* IP, int port, pthread_mutex_t m){
    puerto = port;
    string sIP(IP);
    ip = sIP;
    mutex = m;
}

bool Client::iniciar(){

    logger.info(">>>> INICIANDO CLIENTE ....");
    printf("arranca\n");
    logger.info("#Socket ...");
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(socket == 0){
        printf("error creacion\n");
        logger.error("No se pudo crear el socket");
        return false;
    }
    logger.debug("@Socket creado");

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);

    logger.info("#Conexion con el servidor ...");
    if(connect(socket , (struct sockaddr *)&server , sizeof(struct sockaddr_in)) < 0){
        printf("error conex\n");
        logger.error("Conexion con el servidor fallida");
        return false;
    }
    logger.debug("@Conectado");

    iniciarSesion();

    // Creo los hilos de envio y recibimiento de data
    //typedef void * (*THREADFUNCPTR)(void *);

    //pthread_create(&hiloPop, NULL, (THREADFUNCPTR)Client::desencolar, NULL);
    //pthread_create(&hiloPush, NULL, (THREADFUNCPTR)Client::encolar, NULL);

    juego.cerrar();
    return true;
}

void* Client::encolar(){
    while(true){
        receiveData(NULL, 0);
        cola->push(NULL);
    }
    return NULL;
}

void* Client::desencolar(){
    while(true){
        while(! cola->estaVacia()){
            processData(cola->pop());
            sendData( NULL, 0);
        }
    }
    return NULL;
}

int Client::sendData(void* dato, int total_data_size){

	int total_bytes_enviados = 0;
    int bytes_enviados = 0;
    bool ok = true;

    while ((total_data_size > total_bytes_enviados) && ok){
        bytes_enviados = send(socket, (dato + total_bytes_enviados), (total_data_size-total_bytes_enviados), MSG_NOSIGNAL);

        if(bytes_enviados < 0) {
            logger.error("No se envio correctamente la data"); // Error
            return bytes_enviados;
        } else if(bytes_enviados == 0) {
            logger.error("Se cerro el servidor"); // Socket closed
            ok = false;
        } else total_bytes_enviados += bytes_enviados;
    }
    return 0;
}

int Client::receiveData(void* dato, int bytes_totales){


    int total_bytes_recibidos=0;
    int bytes_recibidos = 0;
    bool ok = true;
    while((bytes_totales > total_bytes_recibidos) && ok){
        bytes_recibidos = recv(socket, (dato + total_bytes_recibidos), (bytes_totales - total_bytes_recibidos), MSG_NOSIGNAL);

        if(bytes_recibidos < 0){
            logger.error("No se recibio correctamente la data"); // Error
            return bytes_recibidos;
        } else if (bytes_recibidos == 0){ // Socket closed
            logger.error("Se cerro el servidor");
            ok = false;
        } else total_bytes_recibidos += bytes_recibidos;

    }
	return total_bytes_recibidos;
}

void Client::processData(void* dato){

}

bool Client::iniciarSesion(){

    int veces_check = 0;

    int size_client = sizeof(client_t);
    client_t* cliente = (client_t*)malloc(size_client);
    cliente->id[0] = 0;
    cliente->pass[0] = 0;

    int accion_recibida = 0;

    bool ok = true;
    while(veces_check < 2){
        juego.init_menu();

        strcpy(cliente->id,juego.get_id().c_str());
        strcpy(cliente->pass,juego.get_password().c_str());

        if(sendData(cliente, size_client) < 0){
            logger.error("Error en el envio de la data");
            ok = false;
        }
        std::cout<<"ID: "<<cliente->id<<"\n";
        std::cout<<"PASS: "<<cliente->pass<<"\n";

        if(receiveData(&accion_recibida, sizeof(int)) < 0){
            logger.error("Error en el recibimiento de la data");
            ok = false;
        }
        if(accion_recibida == 0 && ok) break;
        else{
            veces_check++;
            juego.render_errorLoguin(2-veces_check);
        }
        printf("AC_RECI: %d\n", accion_recibida);

    }
    //free(cliente);

    return ok;
}

void Client::cerrar(){
    close(socket);
    logger.debug("Socket del cliente cerrado");
}
