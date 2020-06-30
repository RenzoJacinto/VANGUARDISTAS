#include "Server.h"
#include "global.h"
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct Credenciales {
    char id[50];
    char pass[50];
} credenciales_t;

typedef struct velocidades {
    int id;
    int VelX;
    int VelY;
} velocidades_t;

nlohmann::json USERS;
void* hilo_validar_credenciales(void* p)
{
    Server* sv = (Server*) p;

    pthread_mutex_lock(&mutex);
    int client = sv->get_socket_actual();
    int id = sv->get_id_actual();
    sv->aumentar_socket();
    pthread_mutex_unlock(&mutex);

    sv->validar_credenciales(client, id);

    return NULL;
}

void* validar_credenciales_aux(int client, int id)
{
    printf("validando credenciales\n");
    credenciales_t* datos = (credenciales_t*)malloc(sizeof(credenciales_t));
    int bytes = recv( client , datos, sizeof(credenciales_t), MSG_NOSIGNAL);
    printf("SOCKET: %d - ID: %s\n", client, datos->id);
    if(bytes > 0){
        try{
            char password[50];
            password[0] = 0;
            std::string pass = USERS.at(datos->id);
            std::cout<<pass;
            strncat(password, pass.c_str(), 49);
            if(strcmp(password, datos->pass) != 0)
            {
                char invalid_pass[22];
                invalid_pass[0] = 0;
                strncat(invalid_pass, "Contrasenia invalida", 21);
                send(client, &invalid_pass, 22, 0);
                validar_credenciales_aux(client, id);
                return NULL;
            }
            printf("datos correctos\n");
            velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
            v->id = id;
            v->VelX=0;
            v->VelY=0;
            send(client, v, sizeof(velocidades_t), 0);
            free(v);
            printf("id enviado\n");
            //pthread_mutex_lock(&mutex);
            //cant_sockets++;
            //pthread_mutex_unlock(&mutex);
            return NULL;
        }
        catch(nlohmann::detail::out_of_range)
        {
            char invalid_id[18];
            invalid_id[0] = 0;
            strncat(invalid_id, "Usuario invalido", 17);
            send(client, &invalid_id, 18, 0);
            validar_credenciales_aux(client, id);
        }
    }
    return NULL;
}


Server::Server(int port, pthread_mutex_t m){
    max_users = json.get_max_users();
    puerto = port;
    mutex = m;
    cant_sockets = 0;
    estado = "server";
    cola = new ColaMultihilo();

    ifstream whitelist;
    whitelist.open("config/whitelist.json", ios::in);
    whitelist >> j_wl;
    USERS =j_wl;
    whitelist.close();
}

int Server::get_socket_actual()
{
    return client_sockets[cant_sockets];
}

void Server::aumentar_socket() {
    cant_sockets++;
}

bool Server::iniciar(){

    logger.info(">>>> INICIANDO SERVER ....");

    logger.info("#Socket ...");
    socket = ::socket(AF_INET , SOCK_STREAM , 0);
    if(socket == 0){
        logger.error("No se pudo crear el socket");
        return false;
    }
    logger.debug("@Socket creado");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(puerto);

    // BIND
    logger.info("#Bind ...");
    if( bind(socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        logger.error("Fallo en el Bind");
        return false;
    }
    logger.debug("@Bind correcto");

    // ESPERA A QUE SE CONECTEN LOS USUARIOS, como maximo "max_users"
    struct sockaddr_in client_addr;
    int actual_socket = 0;
    if (listen(socket , 2) < 0){
            logger.error("Error en el Listen");
            return false;
    }
    //typedef void * (*THREADFUNCPTR)(void *);
    while(actual_socket < 2/*max_users*/ && cant_sockets < MAX_CLIENTS){
        logger.info("#Listen ...");

        std::string msj = "@Esperando a que se conecten usuarios en el puerto: " + std::to_string(puerto);
        logger.debug(msj.c_str());
        // SOCKET DEL CLIENTE
        logger.info("#Aceptar cliente ...");
        printf("esperando conexiones\n");
        socklen_t client_len;
        client_len = sizeof(struct sockaddr_in);
        client_sockets[actual_socket] = accept(socket, (struct sockaddr *) &client_addr, &client_len);
        if (client_sockets[actual_socket] < 0){
            printf("fallo accept\n");
            logger.error("Fallo el accept del cliente");
            continue;
        } else{
            //actual_socket++;
            printf("accepted\n");
            msj = "@Conexion del cliente " + std::to_string(cant_sockets) + " aceptada";
            logger.debug(msj.c_str());
            //pthread_t hilo;
            //validar_credenciales(&client_sockets[cant_sockets]);
            //printf("%d\n", cant_sockets);
            validar_credenciales_aux(client_sockets[actual_socket], actual_socket);
            //pthread_create(&clientes[actual_socket], NULL, hilo_validar_credenciales, this);
            printf("%d\n", cant_sockets);
            actual_socket++;
            cant_sockets++;
        }
        //pthread_join(clientes[0], NULL);
        //pthread_join(clientes[1], NULL);

        //HACE UN USLEEP DE 1 SEG
        //for(int i = time(NULL) + 1; time(NULL) != i; time(NULL));
        printf("termine\n");
    }


    //if(! validar_credenciales()); //DEBERIA ESPERAR A QUE INGRESEN OTROS O QUE VUELVA A INGRESAR;

    //int j;
    //j = pthread_create(&hiloRecibirEncolar, NULL, (void* (*)(void*))recibir_encolar(), NULL);
    //if (j){exit(-1);}
    //j = pthread_create(&hiloDesencolarProcesarEnviar, NULL, (void* (*)(void*))desencolar_procesar_enviar(), NULL);
    //if (j){exit(-1);}

    //while(true){

      //  recibir_encolar();
        //desencolar_procesar_enviar();

    //}

    return true;
}

void* Server::recibir_encolar(int socket){
    while(true) {
        void* data = receiveData(socket);
        if(!data) continue;
        cola->push(data);
    }

    return NULL;
}

void* Server::desencolar_procesar_enviar(){
    return cola->pop();
}

bool Server::sendData(void* dato){


	return true;
}

void* Server::receiveData(int socket){
    velocidades_t* v = (velocidades_t*) malloc(sizeof(velocidades_t));
    int bytes_writen = 0;
    int total_bytes_writen = 0;
    int sent_data_size = sizeof(velocidades_t);
    while(sent_data_size > total_bytes_writen) {
        bytes_writen = recv(client_sockets[socket], v+total_bytes_writen, sizeof(velocidades_t)-total_bytes_writen, MSG_NOSIGNAL);
        total_bytes_writen += bytes_writen;
        if(bytes_writen <= 0) {
            //printf("error en el recv SERVER\n");
            free(v);
            return NULL;
        }
    }
    //printf("data recibida\n");
	return v;
}

void* Server::processData(void* dato){
    return NULL;

}

void Server::cerrar(){
    for(int i=0; i<max_users && i<MAX_CLIENTS; i++){
        close(client_sockets[i]);
    }
    close(socket);
}

int Server::get_socket(int i) {
    return client_sockets[i];
}

bool Server::cola_esta_vacia() {
    return cola->estaVacia();
}

void* Server::validar_credenciales(int client, int id)
{
    printf("validando credenciales\n");
    credenciales_t* datos = (credenciales_t*)malloc(sizeof(credenciales_t));
    int bytes = recv( client , datos, sizeof(credenciales_t), MSG_NOSIGNAL);
    printf("SOCKET: %d - ID: %s\n", client, datos->id);
    if(bytes > 0){
        try{
            char password[50];
            password[0] = 0;
            std::string pass = j_wl.at(datos->id);
            std::cout<<pass;
            strncat(password, pass.c_str(), 49);
            if(strcmp(password, datos->pass) != 0)
            {
                char invalid_pass[22];
                invalid_pass[0] = 0;
                strncat(invalid_pass, "Contrasenia invalida", 21);
                send(client, &invalid_pass, 22, 0);
                validar_credenciales(client, id);
                return NULL;
            }
            printf("datos correctos\n");
            velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
            v->id = id;
            v->VelX=0;
            v->VelY=0;
            send(client, v, sizeof(velocidades_t), 0);
            free(v);
            printf("id enviado\n");
            //pthread_mutex_lock(&mutex);
            //cant_sockets++;
            //pthread_mutex_unlock(&mutex);
            return NULL;
        }
        catch(nlohmann::detail::out_of_range)
        {
            char invalid_id[18];
            invalid_id[0] = 0;
            strncat(invalid_id, "Usuario invalido", 17);
            send(client, &invalid_id, 18, 0);
            validar_credenciales(client, id);
        }
    }
    return NULL;
}

int Server::get_id_actual(){
    return cant_sockets;
}

int Server::get_max_users(){
    return max_users;
}
