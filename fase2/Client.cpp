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


Client::Client(char* IP, int port, pthread_mutex_t m){
    puerto = port;
    string sIP(IP);
    ip = sIP;
    mutex = m;
    estado = "client";
}

bool Client::iniciar(){

    logger.info(">>>> INICIANDO CLIENTE ....");

    logger.info("#Socket ...");
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(socket == 0){
        logger.error("No se pudo crear el socket");
        return false;
    }
    logger.debug("@Socket creado\n");

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);

    logger.info("#Conexion con el servidor ...");
    if(connect(socket , (struct sockaddr *)&server , sizeof(struct sockaddr_in)) < 0){
        logger.error("Conexion con el servidor fallida");
        return false;
    }
    logger.debug("@Conectado");

    // Creo los hilos de envio y recibimiento de data
    int j;
    j = pthread_create(&hiloPop, NULL, (void* (*)(void*))Client::desencolar(), NULL);
    if (j){exit(-1);}
    j = pthread_create(&hiloPop, NULL, (void* (*)(void*))Client::encolar(NULL), NULL);
    if (j){exit(-1);}

    //j = pthread_create(&hiloPush, NULL, (THREADFUNCPTR) &ColaMultihilo::push, cola);

    desencolar();
    encolar(NULL);

    iniciarSesion();

    // Deberia recibir y enviar data aca

    return true;
}

void* Client::desencolar(){
    return cola->pop();
}

void* Client::encolar(void* dato){
    return cola->push(dato);
}

bool Client::sendData(){

    return true;
}

bool Client::receiveData(){

    return false;
}

void Client::processData(){


}

bool Client::iniciarSesion(){
    struct client cliente;
    cliente.id = "juancito";
    cliente.passwd = "juan123";

    /* Deberia pushear en la cola esta data,
    lo dejo asi como ejemplo, pero deberia inicializarse en una imagen por pantalla
        qThreads.push(client);
    */
    return true;
}

void Client::close(){
    //close(socket);
    logger.debug("Socket del cliente cerrado");
}
