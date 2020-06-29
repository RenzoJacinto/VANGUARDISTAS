#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"
#include "Juego.h"

class Client: public Estado{

    public:

        Client(char* IP, int port, pthread_mutex_t m);
        bool iniciar();

        //hilos
        void* encolar();
        void* desencolar();

        int sendData(void* dato, int size_data);
        int receiveData(void* dato, int size_data);
        void processData(void* dato);

        bool iniciarSesion();

        void cerrar();

    private:
        std::string ip;
        ColaMultihilo* cola;
        pthread_t hiloPop;
        pthread_t hiloPush;

        Juego juego;

};

#endif
