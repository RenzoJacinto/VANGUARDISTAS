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
        long int receiveN();
        void finalizar();
        void* processData(void* dato);
        bool iniciarSesion();
        bool cola_esta_vacia();
        int get_socket();
        int get_id();
        void cerrar();

    private:
        std::string ip;
        ColaMultihilo* cola;
        int socket_id;
        int id;
        bool terminar;
        pthread_t hiloEnviar;
        pthread_t hiloRecibirEncolar;
        pthread_t hiloDesencolarProcesar;

};

#endif
