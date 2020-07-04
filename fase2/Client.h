#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"
#include "JuegoCliente.h"


class Client: public Estado{

    public:

        Client(char* IP, int port, pthread_mutex_t m);
        bool iniciar();
        void* enviar();
        void* recibir_encolar();
        void* desencolar_procesar();
        bool sendData(void* dato);
        void* receiveData();
        void finalizar();
        void* processData(void* dato);
        bool iniciarSesion();
        bool cola_esta_vacia();
        int get_socket();
        int get_id();
        void cerrar();
        void iniciar_juego();
        void init_menu();
        void* desencolar();
        void reiniciar_juego();

    private:
        std::string ip;
        ColaMultihilo* cola;
        int socket;
        int id;
        bool terminar;
        JuegoCliente* juego;
        pthread_t hiloEnviar;
        pthread_t hiloRecibirEncolar;
        pthread_t hiloDesencolarProcesar;

};

#endif
