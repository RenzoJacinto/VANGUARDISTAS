#ifndef SERVER_H_
#define SERVER_H_

#include "Estado.h"
#include "json.hpp"
#include <thread>

class Server: public Estado{

    public:
        Server(int port, pthread_mutex_t m);
        bool iniciar();
        bool sendData();
        bool receiveData();
        void processData();
        bool comprobarIdentificacion();
        void close();

        static const int MAX_CLIENTS = 4;

    private:
        int max_users;

        int client_sockets[MAX_CLIENTS];

        std::thread clientes[MAX_CLIENTS];

        nlohmann::json j_wl;
};

#endif
