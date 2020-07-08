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
    est = "client";

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

    logger.debug("@Socket creado");

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);

    logger.info("#Conexion con el servidor ...");
    if(connect(socket , (struct sockaddr *)&server , sizeof(struct sockaddr_in)) < 0){
        //printf("error conex %d\n", o);
        std::cout<<"2: "<<std::strerror(errno)<<"\n";
        logger.error("Conexion con el servidor fallida");
        return false;
    }
    logger.debug("@Conectado");

    if(! juego->iniciarSDL()){
        return false;
    }
    int s;
    if(recv(socket, &s, sizeof(int), MSG_NOSIGNAL)<=0) return false;
    if(s==1){
        juego->renderServerLleno();
        juego->cerrar_ventana();
        logger.error("El servidor esta lleno, se termina la ejecucion");
        return false;
    }
    if(! iniciarSesion()){
        juego->cerrar_ventana();
        return false;
    }

    velocidades_t v ;
    printf("enviando\n");

    if(recv(socket , &v, sizeof(velocidades_t), MSG_NOSIGNAL)<=0)
    {
        renderServerCaido();
        juego->cerrarMenu();
        return false;
    }
    //id = v.id;
    if(strcmp(v.descrip, "on") == 0){
        printf("incio correctamente, id: %d\n", id);
        juego->cerrarMenu();
        juego->iniciarJuego(this, 0);
    } else if(strcmp(v.descrip, "off") == 0){
        printf("reconectar\n");
        juego->cerrarMenu();
        printf("nivel: %d\n", v.id);
        juego->reconectar(this, v.id);
    } else{
        printf("none\n");
        juego->cerrarMenu();
        juego->reconectarSiguiente(this, v.id);
    }
    return true;
}

void* hiloEncolar(void* p){
    printf("CLIENT crea hilo encolar\n");
    Client* client = (Client*)p;
    client->recibir_encolar();
    return NULL;
}

void Client::reiniciar_juego(int nivel){
    juego->reconectar(this, nivel);
}

void Client::reiniciar_siguiente(int nivel){
    juego->reconectarSiguiente(this, nivel);
}

void* hiloDesencolar(void* p){
    printf("CLIENT crea hilo desencolar\n");
    Client* client = (Client*) p;
    client->desencolar_procesar();
    return NULL;
}

void Client::iniciar_juego(){
    //pthread_t hilo_desencolar;
    //pthread_create(&hilo_desencolar, NULL, hiloDesencolar, this);
    juego->iniciarJuego(this, 0);
}

void* Client::enviar(){
    return NULL;
}

void* Client::recibir_encolar(){
    while(true){
        void* data = receiveData();
        if(!data) continue;
        posiciones_t* pos = (posiciones_t*) data;
        cola->push(data);
        if(pos->id == -1) break;
    }
    return NULL;
}

void* Client::desencolar_procesar(){
    while(true){
        if (cola->estaVacia()) continue;
        //void* dato = cola->pop();
        //juego->procesar((posiciones_t*) dato);
    }
    return NULL;
}

void* Client::desencolar(){
    return cola->pop();
}

bool Client::sendData(void* dato){

    velocidades_t* v = (velocidades_t*) dato;
    int bytes_writen = 0;
    int total_bytes_writen = 0;
    int sent_data_size = sizeof(velocidades_t);
	while(sent_data_size > total_bytes_writen){
        bytes_writen = send(get_socket(), v + total_bytes_writen, sizeof(velocidades_t) - total_bytes_writen, MSG_NOSIGNAL);
        total_bytes_writen += bytes_writen;
        if(bytes_writen<=0) {
            printf("error en send CLIENT\n");
            return false;
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
        bytes_writen = recv(socket, pos+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
        total_bytes_writen += bytes_writen;
        if(bytes_writen<=0) {
            printf("error en el recv CLIENT\n");
            free(pos);
            return NULL;
        }
    }
    //printf("recibio data no nula\n");
	return pos;
}

void* Client::processData(void* dato){
    return NULL;

}

bool Client::iniciarSesion(){

    int veces_check = 0;

    int size_client = sizeof(credenciales_t);
    credenciales_t* cliente = (credenciales_t*)malloc(size_client);
    cliente->id[0] = 0;
    cliente->pass[0] = 0;

    velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));

    bool ok = true;
    while(veces_check < 2){
        juego->init_menu();

        strcpy(cliente->id,juego->get_id().c_str());
        strcpy(cliente->pass,juego->get_password().c_str());

        if(send(socket, cliente, size_client, MSG_NOSIGNAL) < 0){
            logger.error("Error en el envio de la data");
            printf("error en el send\n");
            //renderServerCaido();
            ok = false;
        }

        printf("Envio credenciales\n");
        v->VelX = 10;
        if(recv(socket, v, sizeof(velocidades_t), MSG_NOSIGNAL) < 0){
            logger.error("Error en el recibimiento de la data");
            ok = false;
        }
        printf("recibio credenciales\n");
        std::cout<<v->VelX<<"\n";
        if(v->VelX == LOGIN_CORRECTO && ok){
            id = v->id;
            juego->renderWaitUsers();
            break;
        } else{
            veces_check++;
            int intentos = 2 - veces_check;
            printf("renderiza pantalla error\n");
            //if(v->VelX == ID_NO_LOGUEADA_RECON) printf("user %d still connected\n", v->id);
            juego->render_errorLogin(intentos, v->VelX);
            //return false;
            /*std::string msj = "Error de logueo, credenciales incorrectas, quedan " + std::to_string(intentos) + " intentos";
            logger.info(msj.c_str());*/
        }

    }
    //free(cliente);
    free(v);
    return veces_check != 2;
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
    return socket;
}

bool Client::cola_esta_vacia(){
    return cola->estaVacia();
}

int Client::get_id() {
    return id;
}

void Client::crear_hilo_recibir(){
    pthread_create(&hilo_encolar, NULL, hiloEncolar, this);
}
void Client::cerrar_hilo_recibir(){
    pthread_cancel(hilo_encolar);
}
void Client::vaciar_cola(){
    while(!cola->estaVacia()){
        void* dato = cola->pop();
        free(dato);
    }
}

void Client::renderServerCaido(){
    juego->renderServerCaido();
}
