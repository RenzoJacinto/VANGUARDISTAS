#ifndef SERVER_H_
#define SERVER_H_

#include "Estado.h"
#include "json.hpp"
#include <unistd.h>

class Server: public Estado{

    public:
        Server(int port, pthread_mutex_t m);
        bool iniciar();
        int get_socket_actual();
        void aumentar_socket();
        void* recibir_encolar(int i);
        void* desencolar_procesar_enviar();
        bool sendData(void* dato);
        bool cola_esta_vacia();
        void* receiveData(int socket);
        void* processData(void* dato);
        void* validar_credenciales(int client, int id);
        int get_socket(int i);
        void cerrar();
        int get_id_actual();
        int get_max_users();

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
