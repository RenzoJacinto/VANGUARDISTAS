#include "Client.h"
#include "global.h"

#include "ColaMultihilo.h"
#include "Menu.h"

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

    cola = new ColaMultihilo();
    terminar = false;
    juego = new JuegoCliente();
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
    init_menu();
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

void Client::init_menu()
{
    if(! sdl.iniciarSDL()) logger.error("Fallo la inicializacion de SDL");
    Menu menu = Menu();
    if (menu.cargarImagen()) menu.procesar();
}

void* hiloEncolar(void* p){
    printf("CLIENT crea hilo encolar\n");
    Client* client = (Client*)p;
    client->recibir_encolar();
    return NULL;
}

void* hiloDesencolar(void* p){
    printf("CLIENT crea hilo desencolar\n");
    Client* client = (Client*) p;
    client->desencolar_procesar();
    return NULL;
}
void Client::iniciar_juego()
{
    juego->cargarNivel(this);
    printf("CLIENT carga niveles\n");
    char buf[5] = "ASD";
    send(get_socket(), buf, 5, MSG_NOSIGNAL);
    pthread_t hilo_encolar;
    pthread_create(&hilo_encolar, NULL, hiloEncolar, this);
    //pthread_t hilo_desencolar;
    //pthread_create(&hilo_desencolar, NULL, hiloDesencolar, this);
    juego->iniciarNivel(this);
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
    while(true)
    {
        if (cola->estaVacia()) continue;
        void* dato = cola->pop();
        juego->procesar((posiciones_t*) dato);
    }
    return NULL;
}

void* Client::desencolar()
{
    return cola->pop();
}

bool Client::sendData(void* dato){

    velocidades_t* v = (velocidades_t*) dato;
    int bytes_writen = 0;
    int total_bytes_writen = 0;
    int sent_data_size = sizeof(velocidades_t);
	while(sent_data_size > total_bytes_writen)
	{
        bytes_writen = send(get_socket(), v + total_bytes_writen, sizeof(velocidades_t) - total_bytes_writen, MSG_NOSIGNAL);
        total_bytes_writen += bytes_writen;
        if(bytes_writen<=0) {
            printf("error en send CLIENT\n");
            break;
        }
    }
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
    printf("recibio data no nula\n");
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

void Client::cerrar(){
    close(socket);
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