#include "Server.h"
#include "global.h"
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

struct sv{
    Server* server;
    int client_socket;
};

void* server_encolar(void* sv_cSock){
    struct sv* sv_aux = (struct sv*)sv_cSock;
    Server* server = sv_aux->server;
    return server->encolar(sv_aux->client_socket);
}

void* server_desencolar(void* sv){
    Server* server = (Server*)sv;
    return server->desencolar();
}

Server::Server(int port){
    max_users = json.get_max_users();
    if(max_users > MAX_CLIENTS) max_users = MAX_CLIENTS;

    cola = new ColaMultihilo();
    puerto = port;

    ifstream whitelist;
    whitelist.open("config/whitelist.json", ios::in);
    whitelist >> j_wl;
    whitelist.close();

    pthread_mutex_init(&mutex, NULL);
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
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(int))){
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

    socklen_t clilen;
    clilen = sizeof(client_addr);

    //typedef void * (*THREADFUNCPTR)(void *);
    logger.info("#Listen ...");
    if (listen(socket , max_users) < 0){
        logger.error("Error en el Listen");
        std::cout<<std::strerror(errno)<<"\n";
        return false;
    }
    //typedef void * (*THREADFUNCPTR)(void *);
    int actual_socket = 0;
    while(actual_socket < max_users && actual_socket < MAX_CLIENTS){

        std::string msj = "@Esperando a que se conecten " + std::to_string(max_users-actual_socket) + " usuarios en el puerto: " + std::to_string(puerto);
        logger.debug(msj.c_str());
        // SOCKET DEL CLIENTE
        logger.info("#Aceptar cliente ...");
        printf("esperando conexiones\n");
        std::cout<<"1: "<<std::strerror(errno)<<"\n";
        client_sockets[actual_socket] = accept(socket, (struct sockaddr *) &client_addr, &clilen);
        if (client_sockets[actual_socket] < 0){
            printf("fallo accept\n");
            std::cout<<"2: "<<std::strerror(errno)<<"\n";
            logger.error("Fallo el accept del cliente");
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

    pthread_create(&hiloPop, NULL, server_desencolar, this);
    for(int j=0; j<max_users; j++){
        //seteo para la func de cada hilo de cada cliente
        struct sv* sv_sock = (struct sv*)malloc(sizeof(struct sv));
        sv_sock->server = this;
        sv_sock->client_socket = client_sockets[j];
        pthread_create(&hilosPush[j], NULL, server_encolar, sv_sock);
    }

    //pthread_create(&hiloProcesar, NULL, server_procesar, this);

    ManejoDeNiveles niveles;
    niveles.procesarServer(cola);

    cerrar();
    return true;
}

void* Server::encolar(int client_socket){

    int size_view = sizeof(client_vw_t);
    int size_pos = sizeof(position_t);
    while(true){

        position_t pos;
        //pthread_mutex_lock(&mutex);
        if(receiveData(client_socket, &pos, size_pos) < 0) break;
        //pthread_mutex_unlock(&mutex);
        client_vw_t* client_view = (client_vw_t*)malloc(size_view);
        client_view->serial = get_serial_by_socket(client_socket) + 1;
        client_view->tipo_nave = TIPO_JUGADOR;
        client_view->x = pos.x;
        client_view->y = pos.y;
        printf("X: %d\n", client_view->x);
        printf("Y: %d\n", client_view->y);
        printf("Ser: %d\n", client_view->serial);
        printf("Tipo: %d\n", client_view->tipo_nave);
        printf("-----\n");
        cola->push(client_view);
        //
    }
    return NULL;
}

void* Server::desencolar(){
    int size_view =  sizeof(client_vw_t);
    while(true){
        while(!cola->estaVacia()){

            client_vw_t* data = cola->pop();

            std::cout<<"DESENCOLAR\n";
            printf("typ: %d\n", data->tipo_nave);
            printf("ser: %d\n", data->serial);
            printf("X: %d\n", data->x);
            printf("Y: %d\n", data->y);
            std::cout<<"-----------\n";

            //broadcast
            for(int i=0; i<max_users; i++){
                if(sendData(client_sockets[i], data,size_view) < 0 ) {
                    logger.info("Se desconecto un user");
                }
            }
        }
    }
    return NULL;
}

int Server::sendData(int client_socket, client_vw_t* dato, int data_size){

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
    return total_bytes_enviados;
}

int Server::receiveData(int client_socket, position_t* dato, int data_size){
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
    return total_bytes_recibidos;
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

    std::string msj = "+++ ID: ";
    int veces_check[max_users];
    for(int j=0; j<max_users; j++) veces_check[j]=0;

    for(int i=0; i<max_users; i++){
        int data_send = 1;
        while(data_send != 0){
            if(veces_check[i] < 2){
                if(recv(client_sockets[i], &cliente, size_client, MSG_NOSIGNAL) < 0)
                    logger.error("No se recibio correctamente la data");

                string ids(cliente.id);
                string cpass(cliente.pass);

                std::cout<<"CLIENTE "<<i<<"\n";
                std::cout<<"ID: "<<ids<<"\n";
                std::cout<<"Pass: "<<cpass<<"\n";

                msj += ids + " ; PASS: " + cpass;
                logger.info(msj.c_str());

                data_send = check_loguin_user(&cliente);
                if(send(client_sockets[i], &data_send, sizeof(int), MSG_NOSIGNAL) < 0 )
                    logger.error("No se envio correctamente la data");
                if(data_send == 1) logger.info("Error de logueo, credenciales invalidas");

                (veces_check[i])++;
            } else break;
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

int Server::get_serial_by_socket(int client_socket){
    int actual_sock = 0;
    for(int j=0; j<max_users; j++){
        if(client_socket == client_sockets[j]) actual_sock = j;
    }
    return actual_sock;
}

