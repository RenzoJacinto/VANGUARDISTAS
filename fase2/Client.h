#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"
#include <thread>

class Client: public Estado{

    public:

        Client(char* IP, int port, pthread_mutex_t m);
        bool iniciar();
        bool sendData();
        bool receiveData();
        void processData();
        bool iniciarSesion();
        void close();

        //asd
        void desencolar();
        void encolar();

    private:
        std::string ip;

        std::thread hiloPop;
        std::thread hiloPush;

};

#endif
