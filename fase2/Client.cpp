#include "Client.h"
#include "global.h"

#include "ColaMultihilo.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <pthread.h>


void* client_encolar(void* client){
    Client* cliente = (Client*)client;
    cliente->encolar();
    return NULL;
}
void* client_desencolar(void* client){
    Client* cliente = (Client*)client;
    cliente->desencolar();
    return NULL;
}
void*client_enviar(void* client){
    Client* cliente = (Client*)client;
    cliente->enviar();
    return NULL;
}

Client::Client(char* IP, int port){
    puerto = port;
    string sIP(IP);
    ip = sIP;

    cola = new ColaMultihilo();

    size_pos = sizeof(struct position);
    pos = (struct position* )malloc(size_pos);
    pos->x = 0;
    pos->y = 0;
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
    //seteo el socet del cliente
    //pos->client_socket = socket;

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

    if(! iniciarSesion()){
        juego.cerrar();
        return false;
    }

    // Creo los hilos de envio y recibimiento de data
    //typedef void * (*THREADFUNCPTR)(void *);

    pthread_create(&hiloPop, NULL, client_desencolar, this);
    pthread_create(&hiloPush, NULL, client_encolar, this);
    pthread_create(&hiloEnviar, NULL, client_enviar, this);

    juego.iniciar(pos);

    cerrar();
    return true;
}

void* Client::encolar(){
    int size_data = sizeof(client_vw_t);
    client_vw_t client_view;
    while(true){
        if(receiveData(&client_view, size_pos) < 0) break;
        std::cout<<"ENCOLAR\n";
        printf("X: %d\n", client_view.x);
        printf("Y: %d\n", client_view.y);
        std::cout<<"-----------\n";
        cola->push(&client_view);
    }
    return NULL;
}

void* Client::desencolar(){

    while(true){
        while(! cola->estaVacia()){
            processData(cola->pop());
        }
    }
    return NULL;
}

void* Client::enviar(){
    while(true){
        if(sendData(pos,size_pos) < 0) break;
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
    struct client_vw* client_view = (struct client_vw*)dato;
    std::cout<<"PROCESS DATA\n";
    printf("X: %d\n", client_view->x);
    printf("Y: %d\n", client_view->y);
    std::cout<<"-----------\n";
    juego.renderNave(client_view);
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

        if(receiveData(&accion_recibida, sizeof(int)) < 0){
            logger.error("Error en el recibimiento de la data");
            ok = false;
        }
        if(accion_recibida == 0 && ok) break;
        else{
            veces_check++;
            int intentos = 2 - veces_check;
            juego.render_errorLoguin(intentos);
            std::string msj = "Error de logueo, credenciales incorrectas, quedan " + std::to_string(intentos) + " intentos";
            logger.info(msj.c_str());
        }

    }
    //free(cliente);

    return ok;
}

void Client::cerrar(){
    juego.cerrar();
    logger.debug("Juego terminado");
    close(socket);
    logger.debug("Socket del cliente cerrado");
}
