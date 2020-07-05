#include "Server.h"
#include "global.h"
#include "JuegoServidor.h"
#include <string>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct hilosServer{
    Server* server;
    int i;
} hilosServer_t;

void* encolar_procesar(void* p){
    hilosServer_t* data = (hilosServer_t*)p;
    printf("SERVER crea hilo encolar ID: %d\n", data->i);
    Server* sv = data->server;
    sv->recibir_encolar(data->i);
    free(data);
    return NULL;
}

void* desencolar_procesar(void* p){
    printf("SERVER crea hilo desencolar\n");
    Server* sv = (Server*) p;
    sv->desencolar_procesar_enviar();
    return NULL;
}

Server::Server(int port, pthread_mutex_t m){
    max_users = json.get_max_users();
    if(max_users>MAX_CLIENTS) max_users=MAX_CLIENTS;

    puerto = port;
    mutex = m;
    cant_sockets = 0;
    ult_id_enemigo = 0;

    cola = new ColaMultihilo();

    for(int i = 0; i < max_users; ++i){
        desc[i] = false;
        desc_usuarios[i] = false;
    }

    ifstream whitelist;
    whitelist.open("config/whitelist.json", ios::in);
    whitelist >> j_wl;
    whitelist.close();
}

int Server::get_socket_actual(){
    return client_sockets[cant_sockets];
}

void Server::aumentar_socket() {
    cant_sockets++;
}

void* hilo_login(void* data){
    hilosServer_t* d = (hilosServer_t*) data;
    d->server->iniciar_cliente(d->i);
    return NULL;
}

void Server::iniciar_cliente(int i)
{
    struct sockaddr_in client_addr;

    socklen_t clilen;
    clilen = sizeof(client_addr);
    velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
    while(true)
    {
        printf("esperando conexion del cliente %d\n", i);
        client_sockets[i] = accept(socket, (struct sockaddr *) &client_addr, &clilen);
        if (client_sockets[i] < 0){
            printf("fallo accept\n");
            std::cout<<"2: "<<std::strerror(errno)<<"\n";
            logger.error("Fallo el accept del cliente");
        } else{
            printf("accepted\n");
            std::string msj = "";
            msj = "@Conexion del cliente " + std::to_string(i) + " aceptada";
            logger.debug(msj.c_str());
            if(loguin_users(i, false, v)) break;
            printf("el cliente %d no pudo loguearse\n", i);
        }
    }
    free(v);
}

void* hilo_reconexion(void* data){
    hilosServer_t* d = (hilosServer_t*) data;
    d->server->reconectar_cliente(d->i);
    return NULL;
}

void Server::reconectar_cliente(int i)
{
    struct sockaddr_in client_addr;

    socklen_t clilen;
    clilen = sizeof(client_addr);
    velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
    while(true)
    {
        printf("esperando conexion del cliente %d\n", i);
        client_sockets[i] = accept(socket, (struct sockaddr *) &client_addr, &clilen);
        if (client_sockets[i] < 0){
            printf("fallo accept\n");
            std::cout<<"2: "<<std::strerror(errno)<<"\n";
            logger.error("Fallo el accept del cliente");
        } else{
            printf("accepted\n");
            std::string msj = "";
            msj = "@Conexion del cliente " + std::to_string(i) + " aceptada";
            logger.debug(msj.c_str());
            if(loguin_users(i, true, v)) break;
            printf("el cliente %d no pudo loguearse\n", i);
        }
    }
    printf("ID NAVE RECONEXION: %d\n", v->id);
    //velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
    //v->id = i;
    v->descrip[0] = 0;
    strncat(v->descrip, "off", 5);
    send(client_sockets[i], v, sizeof(velocidades_t), MSG_NOSIGNAL);
    //free(v);
    //hilosServer_t* d = (hilosServer_t*) data;
    printf("relogueo el socket %d, con usuario %s e ID: %d\n", i, usuario_per_socket.at(i).c_str(), v->id);
    juego->iniciar_reconexion(v->id, this, i);
    printf("envio datos reconexion\n");
    desc[i] = false;
    desc_usuarios[v->id] = false;
    free(v);
    //recibir_encolar(i);
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
    //struct sockaddr_in client_addr;

    //socklen_t clilen;
    //clilen = sizeof(client_addr);

    //typedef void * (*THREADFUNCPTR)(void *);
    logger.info("#Listen ...");
    if (listen(socket , max_users) < 0){
        logger.error("Error en el Listen");
        std::cout<<std::strerror(errno)<<"\n";
        return false;
    }

    for(int i = 0; i<max_users; i++){
        hilosServer_t* data = (hilosServer_t*)malloc(sizeof(hilosServer_t));
        data->server = this;
        data->i = i;
        pthread_create(&clientes[i], NULL, hilo_login, data);
    }
    //typedef void * (*THREADFUNCPTR)(void *);
    /*int actual_socket = 0;
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
            hilosServer_t* data = (hilosServer_t*)malloc(sizeof(hilosServer_t));
            data->server = this;
            data->i = client_sockets[actual_socket];
            pthread_create(&clientes[actual_socket], NULL, hilo_login, data);
            actual_socket++;
        }

        //HACE UN USLEEP DE 1 SEG
        for(int i = time(NULL) + 1; time(NULL) != i; time(NULL));
        printf("termine\n");
    }*/

    for(int i = 0; i < max_users; ++i){
        pthread_join(clientes[i], NULL);
    }

    for(int i = 0; i<max_users; ++i){
        velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
        v->id = i;
        v->descrip[0] = 0;
        strncat(v->descrip, "on", 5);
        send(get_socket(i), v, sizeof(velocidades_t), MSG_NOSIGNAL);
        free(v);
    }

    for(int i = 0; i<max_users; ++i){
        desc[i] = false;
    }

    juego = new JuegoServidor(json.get_cantidad_enemigo("nivel1"), max_users, this);
    printf("creo el juego\n");

    for(int i = 0; i<max_users; i++){
        hilosServer_t* data = (hilosServer_t*)malloc(sizeof(hilosServer_t));
        data->server = this;
        data->i = i;
        pthread_create(&clientes[i], NULL, encolar_procesar, data);
    }

    juego->iniciarNivel(json.get_cantidad_enemigo("nivel1"), this, 240);
    cerrar();

    return true;
}

void Server::encolar(void* dato) {
    velocidades_t* v = (velocidades_t*) dato;
    if(v->id>3 && v->id == ult_id_enemigo) {
        free(v);
        return;
    }
    //printf("ID A ENCOLAR: %d\n", v->id);
    ult_id_enemigo = v->id;
    cola->push(dato);
}

void* Server::recibir_encolar(int socket){
    while(true) {
        void* data = receiveData(socket);
        printf("recibe data\n");
        if(!data) continue;
        printf("intenta encolar\n");
        encolar(data);
    }

    return NULL;
}

void* Server::desencolar_procesar_enviar(){
    while(true){
        if(cola->estaVacia()) continue;
        void* dato = cola->pop();
        if(dato){
            posiciones_t* pos = juego->procesar((velocidades_t*) dato);
            printf("ID: %d, posX: %d, posY: %d\n", pos->id, pos->posX, pos->posY);
            send_all(pos);
        }
    }
    return NULL;
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

        if(bytes_writen <= 0 && !desc[socket]) {
            velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
            std::string msg = usuario_per_socket.at(socket);
            int id = usuarios_ingresados.at(msg);
            v->id = id;
            v->descrip[0] = 0;
            strncat(v->descrip, "off", 5);
            encolar(v);
            desconectar(socket);
            printf("ID DESC: %d\n", socket);
            desc_usuarios[id] = true;
            reconectar_cliente(socket);
            //pthread_cancel(clientes[i]);
            //hilosServer_t* data = (hilosServer_t*)malloc(sizeof(hilosServer_t));
            //data->server = this;
            //data->i = i;
            //pthread_create(&clientes[i], NULL, hilo_reconexion, data);
            //printf("error en el recv SERVER\n");
            //free(v);
            return NULL;
        }
        else total_bytes_writen += bytes_writen;
    }
    //printf("data recibida\n");
	return v;
}

void* Server::processData(void* dato){
    return NULL;

}

bool Server::loguin_users(int i, bool esReconex, velocidades_t* v){

    logger.info("~~ Verificando las credenciales de los usuarios");

    size_t size_client = sizeof(credenciales_t);
    credenciales_t cliente;

    std::string msj = "+++ ID: ";
    int veces_check = 0;

    int data_send = 1;
    while(data_send != 0){
        if(veces_check < 2)
        {
            if(recv(client_sockets[i], &cliente, size_client, MSG_NOSIGNAL) < 0)
                logger.error("No se recibio correctamente la data");

            string ids(cliente.id);
            string cpass(cliente.pass);

            std::cout<<"CLIENTE "<<socket<<"\n";
            std::cout<<"ID: "<<ids<<"\n";
            std::cout<<"Pass: "<<cpass<<"\n";

            msj += ids + " ; PASS: " + cpass;
            logger.info(msj.c_str());

            if(!esReconex)
            {
                //velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
                v->VelX = check_loguin_user(&cliente);
                v->id = i;
                if(send(client_sockets[i], v, sizeof(velocidades_t), MSG_NOSIGNAL) < 0 )
                    logger.error("No se envio correctamente la data");
                printf("envio accion\n");
                if(v->VelX == 0)
                {
                    //pthread_mutex_lock(&mutex);
                    usuarios_ingresados.insert({ids, i});
                    usuario_per_socket.insert({i, ids});
                    //pthread_mutex_unlock(&mutex);
                    //free(v);
                    std::cout<<"Logueado!!\n";
                    return true;
                }
                //free(v);
                veces_check++;
            }
            else
            {
                //velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
                v->VelX = check_loguin_user_reconexion(&cliente);
                if(v->VelX != -1) {
                    v->id = usuarios_ingresados.at(ids);
                    printf("ID USER %s: %d", ids.c_str(), v->id);
                }
                if(send(client_sockets[i], v, sizeof(velocidades_t), MSG_NOSIGNAL) < 0 )
                    logger.error("No se envio correctamente la data");
                printf("envio accion\n");
                if(v->VelX == 0)
                {
                    //pthread_mutex_lock(&mutex);
                    //usuarios_ingresados.insert({ids, i});
                    //pthread_mutex_unlock(&mutex);
                    std::cout<<"Logueado!!\n";
                    std::map<int, std::string>::iterator it = usuario_per_socket.find(i);
                    if(it != usuario_per_socket.end())
                    {
                        printf("actualizo valor del socket %d a %s\n", i, ids.c_str());
                        it->second = ids;
                    }
                    //free(v);
                    return true;
                }
                //free(v);
                veces_check++;
            }
        }

        else break;
    }
    return false;
}

// correcta credenciales -> 0 else 1
int Server::check_loguin_user(credenciales_t* cliente){
    //pthread_mutex_lock(&mutex);
    //printf("buscando credenciales de %s\n" cliente->id);
    if(usuarios_ingresados.find(cliente->id) != usuarios_ingresados.end()){
        //if(usuarios_ingresados.at(cliente->id) == 1){
        //    logger.info("Usuario ya ingresado");
            return -1;
        //}
    }
    //pthread_mutex_unlock(&mutex);
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

int Server::check_loguin_user_reconexion(credenciales_t* cliente){
    //pthread_mutex_lock(&mutex);
    printf("buscando credenciales de %s\n", cliente->id);
    if(usuarios_ingresados.find(cliente->id) == usuarios_ingresados.end()){
        //    logger.info("Usuario ya ingresado");
        printf("User not on this game\n");
        return -1;
        //}
    }
    //pthread_mutex_unlock(&mutex);
    int id_user = usuarios_ingresados.at(cliente->id);
    if(!desc_usuarios[id_user])
    {
        printf("user %d still connected\n", id_user);
        return -2;
    }
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

int Server::get_id_actual(){
    return cant_sockets;
}

void Server::send_all(posiciones_t* pos){
    for(int i = 0; i<max_users; i++){
        if(desconecto(i)) continue;
        int total_bytes_writen = 0;
        int bytes_writen = 0;
        int sent_data_size = sizeof(posiciones_t);
        while(sent_data_size > total_bytes_writen) {
            bytes_writen = send(client_sockets[i], pos+total_bytes_writen, sizeof(posiciones_t)-total_bytes_writen, MSG_NOSIGNAL);
            total_bytes_writen += bytes_writen;
            if(bytes_writen<=0){
                /*velocidades_t* v = (velocidades_t*)malloc(sizeof(velocidades_t));
                v->id = i;
                v->descrip[0] = 0;
                strncat(v->descrip, "off", 5);
                encolar(v);
                desconectar(i);
                std::string msg = usuario_per_socket.at(i);
                int id = usuarios_ingresados.at(msg);
                printf("ID DESC: %d\n", id);
                desc_usuarios[id] = true;
                pthread_cancel(clientes[i]);
                hilosServer_t* data = (hilosServer_t*)malloc(sizeof(hilosServer_t));
                data->server = this;
                data->i = i;
                pthread_create(&clientes[i], NULL, hilo_reconexion, data);*/
                break;
            }

            }
            //printf("envio mensaje a todos\n");
        }
}

void* Server::desencolar(){
    return cola->pop();
}

void Server::desconectar(int i)
{
    desc[i] = true;
}

bool Server::desconecto(int i)
{
    return desc[i];
}

void Server::conectar(int i)
{
    desc[i] = false;
}
