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

void* hilo_validar_credenciales(void* p)
{
    Server* sv = (Server*) p;

    pthread_mutex_lock(&mutex);
    int client = sv->get_socket_actual();
    sv->aumentar_socket();
    pthread_mutex_unlock(&mutex);

    sv->validar_credenciales(client);

    return NULL;
}

Server::Server(int port, pthread_mutex_t m){
    max_users = json.get_max_users();
    puerto = port;
    mutex = m;
    cant_sockets = 0;
    estado = "server";

    ifstream whitelist;
    whitelist.open("config/whitelist.json", ios::in);
    whitelist >> j_wl;
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
    int client_addrlen;
    int actual_socket = 0;
    //typedef void * (*THREADFUNCPTR)(void *);
    while(actual_socket < 2/*max_users*/ && cant_sockets < MAX_CLIENTS){
        logger.info("#Listen ...");
        if (listen(socket , max_users) < 0){
            logger.error("Error en el Listen");
            return false;
        }
        std::string msj = "@Esperando a que se conecten usuarios en el puerto: " + std::to_string(puerto);
        logger.debug(msj.c_str());
        // SOCKET DEL CLIENTE
        logger.info("#Aceptar cliente ...");
        printf("esperando conexiones\n");
        client_sockets[actual_socket] = accept(socket, (struct sockaddr *) &client_addr, (socklen_t*) &client_addrlen);
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
            //validar_credenciales(client_sockets[actual_socket]);
            pthread_create(&clientes[actual_socket], NULL, hilo_validar_credenciales, this);
            printf("%d\n", cant_sockets);
            actual_socket++;
        }

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

void* Server::recibir_encolar(){
    cola->push(receiveData());
    return NULL;
}

void* Server::desencolar_procesar_enviar(){
    while(!cola->estaVacia()){
        sendData(processData(cola->pop()));
    }
    return NULL;
}

bool Server::sendData(void* dato){


	return true;
}

void* Server::receiveData(){
	return NULL;
}

void* Server::processData(void* dato){
    return NULL;

}

void Server::close(){
    /*for(int i=0; i<max_users && i<MAX_CLIENTS; i++){
        close(client_sockets[i]);
    }
    close(socket);*/
}

int Server::get_socket(int i) {
    return client_sockets[i];
}

void* Server::validar_credenciales(int client)
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
                int byes1 = send(client, &invalid_pass, 22, 0);
                validar_credenciales(client);
                return NULL;
            }
            printf("datos correctos\n");
            char hola[6];
            hola[0] = 0;
            strncat(hola, "hola", 5);
            int byes1 = send(client, &hola, 6, 0);
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
            int byes1 = send(client, &invalid_id, 18, 0);
            validar_credenciales(client);
        }
    }
}
