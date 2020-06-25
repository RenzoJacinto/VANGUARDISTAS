#ifndef SERVER_H_
#define SERVER_H_

#include "Estado.h"
#include "json.hpp"

class Server: public Estado{

    public:
        Server(int port, pthread_mutex_t m);
        bool iniciar();
        void* desencolar();
        void* encolar(void* dato);
        bool sendData();
        bool receiveData();
        void processData();
        bool comprobarIdentificacion();
        void close();

        static const int MAX_CLIENTS = 4;

    private:
        int max_users;

        int client_sockets[MAX_CLIENTS];

        ColaMultihilo* cola;

        pthread_t clientes[MAX_CLIENTS];
        pthread_attr_t attr;

        nlohmann::json j_wl;
};

#endif
