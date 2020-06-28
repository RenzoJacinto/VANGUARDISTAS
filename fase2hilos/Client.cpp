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

typedef struct posiciones {
    int id;
    int posX;
    int posY;
} posiciones_t;

typedef struct velocidades {
    int id;
    int VelX;
    int VelY;
} velocidades_t;

Client::Client(char* IP, int port, pthread_mutex_t m){
    puerto = port;
    string sIP(IP);
    ip = sIP;
    mutex = m;
    estado = "client";
    cola = new ColaMultihilo();
    terminar = false;
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
    velocidades_t* v =(velocidades_t*)malloc(sizeof(velocidades_t));
    printf("enviando\n");
    int bytes = send(socket, credenciales, sizeof(credenciales_t), MSG_NOSIGNAL);
    if(bytes>0){printf("sent\n");}
    bytes = recv(socket , v, sizeof(velocidades_t), MSG_NOSIGNAL);
    id = v->id;
    printf("incio correctamente, id: %d\n", id);
    free(v);
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
    while(!terminar)
    {
        void* data = receiveData();
        if(!data) continue;
        cola->push(data);
    }
    return NULL;
}

void* Client::desencolar_procesar(){
    return cola->pop();
}

bool Client::sendData(void* dato){
	return true;
}

void* Client::receiveData(){
    posiciones_t* pos = (posiciones_t*)malloc(sizeof(posiciones_t));
    int bytes_writen = 0;
    int total_bytes_writen = 0;
    int sent_data_size = sizeof(posiciones_t);
    while(sent_data_size > total_bytes_writen) {
        bytes_writen = recv(socket_id, pos+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
        total_bytes_writen += bytes_writen;
        if(bytes_writen<=0) {
            //printf("error en el recv CLIENT\n");
            free(pos);
            return NULL;
        }
    }
    //printf("what");
	return pos;
}

void* Client::processData(void* dato){
    return NULL;

}

bool Client::iniciarSesion(){


    return true;
}

void Client::finalizar() {
    pthread_mutex_lock(&mutex);
    terminar = true;
    pthread_mutex_unlock(&mutex);
}

void Client::close(){
    //close(socket);
    logger.debug("Socket del cliente cerrado");
}

int Client::get_socket() {
    return socket_id;
}

bool Client::cola_esta_vacia(){
    return cola->estaVacia();
}

int Client::get_id() {
    return id;
}
