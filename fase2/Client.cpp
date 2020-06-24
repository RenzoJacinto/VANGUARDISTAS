#include "Client.h"
#include "global.h"

#include "ColaMultihilo.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


Client::Client(char* IP, int port){
    puerto = port;
    string sIP(IP);
    ip = sIP;
}

bool Client::iniciar(){

    logger.info(">>>> INICIANDO CLIENTE ....");

    logger.info("#Socket ...");
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if(socket == 0){
        logger.error("No se pudo crear el socket");
        return false;
    }
    logger.debug("@Socket creado\n");

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(puerto);

    logger.info("#Conexion con el servidor ...");
    if(connect(socket , (struct sockaddr *)&server , sizeof(struct sockaddr_in)) < 0){
        logger.error("Conexion con el servidor fallida");
        return false;
    }
    logger.debug("@Conectado");

    // Creo los hilos de envio y recibimiento de data
 //   hiloPop = thread(&Client::receiveData(), this, &socket);
  //  hiloPush = thread(&Client::sendData(), this, &socket);

    iniciarSesion();

    // Deberia recibir y enviar data aca

    return true;
}

bool Client::sendData(){

    return true;
}

bool Client::receiveData(){

    return false;
}

void Client::processData(){


}

bool Client::iniciarSesion(){
    struct client cliente;
    cliente.id = "juancito";
    cliente.passwd = "juan123";

    /* Deberia pushear en la cola esta data,
    lo dejo asi como ejemplo, pero deberia inicializarse en una imagen por pantalla
        qThreads.push(client);
    */
    return true;
}

void Client::close(){
    //close(socket);
    logger.debug("Socket del cliente cerrado");
}
