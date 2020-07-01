#ifndef SERVER_H_
#define SERVER_H_

#include "Estado.h"
#include "json.hpp"
#include "Nivel.h"
#include "ManejoDeNiveles.h"

class Server: public Estado{

    public:
        Server(int port);
        bool iniciar();

        //hilos
        void* encolar(int client_socket);
        void* desencolar();

        int sendData(int client_socket, client_vw_t* dato, int data_size);
        int receiveData(int client_socket, position_t* dato, int data_size);
        void processData();

        int check_loguin_user(client_t* cliente);
        void loguin_users();

        void cerrar();
        int get_serial_by_socket(int client_socket);

        static const int MAX_CLIENTS = 4;

    private:
        int max_users;
        int client_sockets[MAX_CLIENTS];
        ColaMultihilo* cola;

        pthread_t hiloPop;
        pthread_t hilosPush[MAX_CLIENTS];

        nlohmann::json j_wl;

        TextureW gNaves[MAX_CLIENTS];

        Nivel server_level;
};

#endif
