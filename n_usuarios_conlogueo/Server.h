#ifndef SERVER_H_
#define SERVER_H_

#include "Estado.h"
#include "json.hpp"
#include "JuegoServidor.h"
#include <unistd.h>

class JuegoServidor;

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


        int get_socket(int i);
        int check_loguin_user(credenciales_t* cliente);
        void loguin_users(int socket);
        void cerrar();
        void send_all(posiciones_t* pos);
        int get_id_actual();
        void encolar(void* dato);
        void* desencolar();

        static const int MAX_CLIENTS = 4;

    private:
        JuegoServidor* juego;
        int ult_id_enemigo;
        int max_users;
        int cant_sockets;
        int client_sockets[MAX_CLIENTS];
        ColaMultihilo* cola;
        //pthread_t hiloRecibirEncolar;
        //pthread_t hiloDesencolarProcesarEnviar;
        pthread_t clientes[MAX_CLIENTS];
        pthread_attr_t attr;

        nlohmann::json j_wl;
        std::map<std::string,int> usuarios_ingresados;
};

#endif
