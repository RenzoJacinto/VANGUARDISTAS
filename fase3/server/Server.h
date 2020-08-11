#ifndef SERVER_H_
#define SERVER_H_

#include "typesmsj.h"
#include "json.hpp"
#include "JuegoServidor.h"
#include <unistd.h>

class JuegoServidor;

class Server{

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
        int check_loguin_user_reconexion(credenciales_t* cliente);
        bool loguin_users(int i, bool esReconex, velocidades_t* v);
        void cerrar();
        void send_all(posiciones_t* pos);
        int get_id_actual();
        void encolar(void* dato);
        void* desencolar();
        bool desconecto(int i);
        void desconectar(int i);
        void iniciar_cliente(int i);
        void reconectar_cliente(int i);
        void conectar(int i);
        int getMaxUsers();

        void crear_hilos_recibir(int nivel);
        void cerrar_hilos_recibir();
        void vaciar_cola();
        void rechazar_conexiones();

        static const int MAX_CLIENTS = 4;

    private:
        int puerto;
        int socket;

        JuegoServidor* juego;
        int ult_id_enemigo;
        int max_users;
        int cant_sockets;
        int client_sockets[MAX_CLIENTS];
        ColaMultihilo* cola;
        bool desc[MAX_CLIENTS];
        bool desc_usuarios[MAX_CLIENTS];
        pthread_t hiloRechazarConexiones;
        int users_conectados;
        pthread_t clientes[MAX_CLIENTS];
        pthread_attr_t attr;

        nlohmann::json j_wl;
        std::map<std::string,int> usuarios_ingresados;
        std::map<int,std::string> usuario_per_socket;
};

#endif
