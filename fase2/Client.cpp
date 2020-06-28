#include "Client.h"
#include "global.h"

#include "ColaMultihilo.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <thread>

typedef struct Credenciales {
    char id[50];
    char pass[50];
} credenciales_t;

Client::Client(char* IP, int port, pthread_mutex_t m){
    puerto = port;
    string sIP(IP);
    ip = sIP;
    mutex = m;
    estado = "client";
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
    socket_id = socket;
    logger.debug("@Socket creado\n");

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

    credenciales_t* credenciales = (credenciales_t*)malloc(sizeof(credenciales_t));
    credenciales->id[0] = 0;
    strncat(credenciales->id, "carlos", 49);
    credenciales->pass[0] = 0;
    strncat(credenciales->pass, "asdasd", 49);
    char* buf = nullptr;
    printf("enviando\n");
    int bytes = send(socket, credenciales, sizeof(credenciales_t), MSG_NOSIGNAL);
    if(bytes>0){printf("sent\n");}
    recv(socket , buf, 5, MSG_NOSIGNAL);
    // Creo los hilos de envio y recibimiento de data
    /*int j;
    j = pthread_create(&hiloEnviar, NULL, (void* (*)(void*))enviar(), NULL);
    if (j){exit(-1);}
    j = pthread_create(&hiloRecibirEncolar, NULL, (void* (*)(void*))recibir_encolar(), NULL);
    if (j){exit(-1);}
    j = pthread_create(&hiloDesencolarProcesar, NULL, (void* (*)(void*))desencolar_procesar(), NULL);
    if (j){exit(-1);}

    while(!cola->estaVacia()){

        enviar();
        recibir_encolar();
        desencolar_procesar();

    }*/

    return true;
}

void* Client::enviar(){
    return NULL;
}

void* Client::recibir_encolar(){
    cola->push(receiveData());
    return NULL;
}

void* Client::desencolar_procesar(){
    while(!cola->estaVacia()){
        processData(cola->pop());
    }
    return NULL;
}

bool Client::sendData(void* dato){
	return true;
}

void* Client::receiveData(){
	return NULL;
}

void* Client::processData(void* dato){
    return NULL;

}

bool Client::iniciarSesion(){


    return true;
}

void Client::close(){
    //close(socket);
    logger.debug("Socket del cliente cerrado");
}

int Client::get_socket() {
    return socket_id;
}
