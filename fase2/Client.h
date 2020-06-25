#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"

class Client: public Estado{

    public:

        Client(char* IP, int port, pthread_mutex_t m);
        bool iniciar();
        void* recibir_encolar();
        void* desencolar_procesar_enviar();
        bool sendData(void* dato);
        void* receiveData();
        void* processData(void* dato);
        bool iniciarSesion();
        void close();

    private:
        std::string ip;
        ColaMultihilo* cola;
        pthread_t hiloRecibirEncolar;
        pthread_t hiloDesencolarProcesarEnviar;

        pthread_t hiloPop;
        pthread_t hiloPush;

};

#endif
