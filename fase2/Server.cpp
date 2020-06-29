#include "Server.h"
#include "global.h"
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


Server::Server(int port, pthread_mutex_t m){
    max_users = json.get_max_users();
    if(max_users > MAX_CLIENTS) max_users = MAX_CLIENTS;

    puerto = port;
    mutex = m;

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
        logger.error("No se pudo crear el socket");
        return false;
    }
    logger.debug("@Socket creado");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(puerto);

    int opt = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        logger.error("Error al setsockopt");
        return false;
    }

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

    //typedef void * (*THREADFUNCPTR)(void *);
    logger.info("#Listen ...");
    if (listen(socket , max_users) < 0){
        logger.error("Error en el Listen");
        return false;
    }

    int actual_socket = 0;
    while(actual_socket < max_users && actual_socket < MAX_CLIENTS){

        std::string msj = "@Esperando a que se conecten " + std::to_string(max_users-actual_socket) + " usuarios en el puerto: " + std::to_string(puerto);
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
            printf("accepted\n");
            msj = "@Conexion del cliente " + std::to_string(actual_socket) + " aceptada";
            logger.debug(msj.c_str());
            actual_socket++;
        }

        //HACE UN USLEEP DE 1 SEG
        for(int i = time(NULL) + 1; time(NULL) != i; time(NULL));
        printf("termine\n");
    }
    loguin_users();
    cerrar();
    return true;
}

void* Server::encolar(){
    while(true){
        receiveData(0, NULL, 0);
        //cola->push(NULL);
    }
    return NULL;
}

void* Server::desencolar(){
    while(!cola->estaVacia()){
        //processData(cola->pop());
        sendData(0, NULL, 0);
    }
    return NULL;
}

int Server::sendData(int client_socket, void* dato, int data_size){

    int total_bytes_enviados = 0;
    int bytes_enviados = 0;
    bool ok = true;

    std::string msj = "";
    std::string str_sock = std::to_string(client_socket);

    while ((data_size > total_bytes_enviados) && ok){
        bytes_enviados = send(client_socket, (dato + total_bytes_enviados), (data_size-total_bytes_enviados), MSG_NOSIGNAL);

        if (bytes_enviados < 0) {
            msj = "No se envio correctamente la data del cliente con socket " + str_sock;
            logger.error(msj.c_str());  // Error
            return bytes_enviados;
        } else if(bytes_enviados == 0) {
            msj = "El cliente de socket " + str_sock +" se desconecto"; // Socket closed
            logger.error(msj.c_str());
            ok = false;
        } else total_bytes_enviados += bytes_enviados;
    }

    return 0;

}

int Server::receiveData(int client_socket, void* dato, int data_size){
	int total_bytes_recibidos = 0;
    int bytes_recibidos = 0;
    bool ok = true;

    std::string msj = "";
    std::string str_sock = std::to_string(client_socket);

    while ((data_size > total_bytes_recibidos) && ok) {
        bytes_recibidos = recv(client_socket, (dato + total_bytes_recibidos), (data_size - total_bytes_recibidos), MSG_NOSIGNAL);

        if (bytes_recibidos < 0){
            msj = "No se recibio correctamente la data del cliente con socket " + str_sock;
            logger.error(msj.c_str()); // Error
            return bytes_recibidos;
        } else if (bytes_recibidos == 0) {
            msj = "El cliente de socket " + str_sock +" se desconecto";
            logger.error(msj.c_str()); // Socket closed
            ok = false;
        } else total_bytes_recibidos += bytes_recibidos;

    }
    return 0;
}

void Server::processData(){


}

void Server::cerrar(){
    for(int i=0; i<max_users && i<MAX_CLIENTS; i++){
        close(client_sockets[i]);
    }
    close(socket);
}

void Server::loguin_users(){

    logger.info("~~ Verificando las credenciales de los usuarios");

    size_t size_client = sizeof(client_t);
    client_t cliente;


    int data_send = 1;
    std:string msj = "+++ ID: ";
    int veces_check[max_users];
    for(int j=0; j<max_users; j++) veces_check[j]=0;
    while(true){
        for(int i=0; i<max_users; i++){
            if(veces_check[i] == 2) break;
            if(receiveData(client_sockets[i],&cliente, size_client) < 0)
                logger.error("No se recibio correctamente la data");

            std::cout<<"ID: "<<cliente.id<<"\n";
            std::cout<<"PASS: "<<cliente.pass<<"\n";

            string ids(cliente.id);
            string cpass(cliente.pass);
            msj += ids + " ; PASS: " + cpass;
            logger.info(msj.c_str());


            data_send = check_loguin_user(&cliente);
            if(sendData(client_sockets[i], &data_send, sizeof(int)) < 0 )
                logger.error("No se envio correctamente la data");
            printf("DATA_SEND: %d\n", data_send);

            (veces_check[i])++;
        }
    }

}

// correcta credenciales -> 0 else 1
int Server::check_loguin_user(client_t* cliente){
    int ok = 0;
    std::string msj = "--- ";
    nlohmann::json& j_aux = json.searchValue(j_wl, cliente->id);
    if(j_aux == "errorKey"){
        msj += "Id inexistente";
        logger.info(msj.c_str());
        ok = 1;
    } else{
        std::string password = j_wl.at(cliente->id);
        if(password != cliente->pass){
            msj += "Password incorrecta";
            logger.info(msj.c_str());
            ok = 1;
        }
    }
    return ok;
}

