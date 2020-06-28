#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"

class Client: public Estado{

    public:

        Client(char* IP, int port, pthread_mutex_t m);
        bool iniciar();
        void* enviar();
        void* recibir_encolar();
        void* desencolar_procesar();
        bool sendData(void* dato);
        void* receiveData();
        void* processData(void* dato);
        bool iniciarSesion();
        int get_socket();
        void close();

    private:
        std::string ip;
        ColaMultihilo* cola;
        int socket_id;
        pthread_t hiloEnviar;
        pthread_t hiloRecibirEncolar;
        pthread_t hiloDesencolarProcesar;

};

#endif
