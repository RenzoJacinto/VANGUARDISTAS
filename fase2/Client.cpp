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
    j = pthread_create(&hiloRecibirEncolar, NULL, (void* (*)(void*))Client::recibir_encolar(), NULL);
    if (j){exit(-1);}
    j = pthread_create(&hiloDesencolarProcesarEnviar, NULL, (void* (*)(void*))Client::desencolar_procesar_enviar(), NULL);
    if (j){exit(-1);}

    while(!cola->estaVacia()){

        recibir_encolar();
        desencolar_procesar_enviar();

    }

    return true;
}

void* Client::recibir_encolar(){
    cola->push(receiveData());
    return NULL;
}

void* Client::desencolar_procesar_enviar(){
    sendData(processData(cola->pop()));
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
