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
    pos->x = 10;
    pos->y = 10;
    pthread_mutex_init(&mutex, NULL);
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
    } juego.cerrarMenu();

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

    while(true){
        client_vw_t client_view;

        if(receiveData(&client_view, size_data) < 0) break;


        client_vw_t* client_recv = (client_vw_t*)malloc(size_data);
        client_recv->tipo_nave = client_view.tipo_nave;
        client_recv->x = client_view.x;
        client_recv->y = client_view.y;
        client_recv->serial = client_view.serial;

        std::cout<<"ENCOLAR\n";
        printf("ser: %d\n", client_recv->serial);
        printf("ty: %d\n", client_recv->tipo_nave);
        printf("X: %d\n", client_recv->x);
        printf("Y: %d\n", client_recv->y);
        std::cout<<"-----------\n";

        cola->push(client_recv);

    }
    return NULL;
}

void* Client::desencolar(){
//    size_t size_view = sizeof(client_vw_t);

    while(true){

        while(! cola->estaVacia()){

            client_vw_t* data = cola->pop();
            /*client_vw_t* client_view = (client_vw_t*)malloc(size_view);
            client_view->tipo_nave = data->tipo_nave;
            client_view->serial = data->serial;
            client_view->x = data->x;
            client_view->y = data->y;*/

            //pthread_mutex_lock(&mutex);
            processData(data);
            //pthread_mutex_unlock(&mutex);
        }

    }
    return NULL;
}

void* Client::enviar(){
    while(true){
        //pthread_mutex_lock(&mutex);
        if(sendData(pos,size_pos) < 0) break;
        //pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int Client::sendData(position_t* dato, int total_data_size){
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

int Client::receiveData(client_vw_t* dato, int bytes_totales){


    int total_bytes_recibidos=0;
    int bytes_recibidos = 0;
    bool ok = true;
    while((bytes_totales > bytes_recibidos) && ok){
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

void Client::processData(client_vw_t* dato){
    std::cout<<"PROCESS DATA\n";
    printf("X: %d\n", dato->x);
    printf("Y: %d\n", dato->y);
    std::cout<<"-----------\n";
    juego.renderNave(dato);
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

        if(send(socket, cliente, size_client, MSG_NOSIGNAL) < 0){
            logger.error("Error en el envio de la data");
            ok = false;
        }

        if(recv(socket, &accion_recibida, sizeof(int), MSG_NOSIGNAL) < 0){
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
