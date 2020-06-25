#include "Server.h"
#include "global.h"
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>

Server::Server(int port, pthread_mutex_t m){
    int max_users_j = json.get_max_users();
    if(max_users_j > MAX_CLIENTS) max_users = MAX_CLIENTS;
    else max_users = max_users_j;


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
    while(actual_socket < max_users && actual_socket < MAX_CLIENTS){
        logger.info("#Listen ...");
        if (listen(socket , max_users) < 0){
            logger.error("Error en el Listen");
            return false;
        }
        std::string msj = "@Esperando a que se conecten usuarios en el puerto: " + std::to_string(puerto);
        logger.debug(msj.c_str());

        // SOCKET DEL CLIENTE
        logger.info("#Aceptar cliente ...");
        client_sockets[actual_socket] = accept(socket, (struct sockaddr *) &client_addr, (socklen_t*) &client_addrlen);
        if (client_sockets[actual_socket] < 0){
            logger.error("Fallo el accept del cliente");
            return false;
        } else{
            actual_socket++;
            msj = "@Conexion del cliente " + std::to_string(actual_socket) + " aceptada";
            logger.debug(msj.c_str());
        }

        //HACE UN USLEEP DE 1 SEG
        for(int i = time(NULL) + 1; time(NULL) != i; time(NULL));
    }


    if(! comprobarIdentificacion()); //DEBERIA ESPERAR A QUE INGRESEN OTROS O QUE VUELVA A INGRESAR;


    //while(! qThreads.estaVacia()){

        //receiveData();
        //processData();
        //sendData();

    //}

    return true;
}

bool Server::sendData(){


	return true;
}

bool Server::receiveData(){
	return true;
}

void Server::processData(){


}

bool Server::comprobarIdentificacion(){

    /*  Deberia recibir por la cola el struct:
        struct client* cliente
        que contiene los datos del cliente{
        (ESTO PARA CADA CLIENTE, se ve de los sockets)


    bool ok = true;
    nlohmann::json& j_user = json.searchValue(j_wl, cliente->id.c_str());
    if(j_user == "errorKey"){
        std::string msj = "No existe el usuario " + cliente->id;
        logger.error(msj.c_str());
        ok=false;
    } else{
        if(j_user != cliente->passwd){
            logger.error("Contraseña incorrecta");
            ok=false;
        }
    }

    if(! ok); //Enviar imagen de inicio erroneo

    return ok;*/
    return true;
}

void Server::close(){
    /*for(int i=0; i<max_users && i<MAX_CLIENTS; i++){
        close(client_sockets[i]);
    }
    close(socket);*/
}
