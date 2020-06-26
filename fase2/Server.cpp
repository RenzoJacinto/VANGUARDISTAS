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

bool Server::iniciar(){

    logger.info(">>>> INICIANDO SERVER ....");

    logger.info("#Socket ...");
    socket = ::socket(AF_INET , SOCK_STREAM , 0);
    if(socket == 0){
        printf("error crear?\n");
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
        printf("error bind\n");
        return false;
    }
    logger.debug("@Bind correcto");

    // ESPERA A QUE SE CONECTEN LOS USUARIOS, como maximo "max_users"
    struct sockaddr_in client_addr;
    int client_addrlen;
    typedef void * (*THREADFUNCPTR)(void *);
    while(cant_sockets < 2/*max_users*/ && cant_sockets < MAX_CLIENTS){
        logger.info("#Listen ...");
        if (listen(socket , max_users) < 0){
            logger.error("Error en el Listen");
            return false;
        }
        std::string msj = "@Esperando a que se conecten usuarios en el puerto: " + std::to_string(puerto);
        logger.debug(msj.c_str());
        // SOCKET DEL CLIENTE
        logger.info("#Aceptar cliente ...");
        client_sockets[cant_sockets] = accept(socket, (struct sockaddr *) &client_addr, (socklen_t*) &client_addrlen);
        if (client_sockets[cant_sockets] < 0){
            printf("fallo accept\n");
            logger.error("Fallo el accept del cliente");
            continue;
        } else{
            printf("accepted\n");
            msj = "@Conexion del cliente " + std::to_string(cant_sockets) + " aceptada";
            logger.debug(msj.c_str());
            cant_sockets++;
            pthread_t hilo;
            printf("aaaa\n");
            //validar_credenciales(&client_sockets[cant_sockets]);
           // printf("%d\n", cant_sockets);

            pthread_create(&hilo, NULL, (THREADFUNCPTR) &Server::validar_credenciales, &client_sockets[cant_sockets]);
            //actual_socket++;
            printf("%d\n", cant_sockets);
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

void* Server::validar_credenciales(void* client_socket)
{
    for(int i = time(NULL) + 15; time(NULL) != i; time(NULL));

    int client = *(int*)(client_socket);
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
            validar_credenciales(client_socket);
        }
    }
}
