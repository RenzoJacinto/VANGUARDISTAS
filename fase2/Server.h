#ifndef SERVER_H_
#define SERVER_H_

#include "Estado.h"
#include "json.hpp"

class Server: public Estado{

    public:
        Server(int port, pthread_mutex_t m);
        bool iniciar();

        //hilos
        void* encolar();
        void* desencolar();

        int sendData(int client_socket, void* dato, int data_size);
        int receiveData(int client_socket, void* dato, int data_size);
        void processData();

        int check_loguin_user(struct client* cliente);
        void loguin_users();

        void cerrar();

        static const int MAX_CLIENTS = 4;

    private:
        int max_users;
        int client_sockets[MAX_CLIENTS];
        ColaMultihilo* cola;

        nlohmann::json j_wl;
};

#endif
