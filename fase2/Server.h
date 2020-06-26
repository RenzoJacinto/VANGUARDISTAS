#ifndef SERVER_H_
#define SERVER_H_

#include "Estado.h"
#include "json.hpp"

class Server: public Estado{

    public:
        Server(int port, pthread_mutex_t m);
        bool iniciar();
        void* recibir_encolar();
        void* desencolar_procesar_enviar();
        bool sendData(void* dato);
        void* receiveData();
        void* processData(void* dato);
        void* validar_credenciales(void* socket);
        void close();

        static const int MAX_CLIENTS = 4;

    private:
        int max_users;
        int cant_sockets;
        int client_sockets[MAX_CLIENTS];
        ColaMultihilo* cola;
        pthread_t hiloRecibirEncolar;
        pthread_t hiloDesencolarProcesarEnviar;
        pthread_t hilo1;
        pthread_t hilo2;
        pthread_t clientes[MAX_CLIENTS];
        pthread_attr_t attr;

        nlohmann::json j_wl;
};

#endif
