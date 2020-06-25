#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"

class Client: public Estado{

    public:

        Client(char* IP, int port, pthread_mutex_t m);
        bool iniciar();
        bool sendData();
        bool receiveData();
        void processData();
        bool iniciarSesion();
        void close();

    private:
        std::string ip;
        ColaMultihilo* cola;

        pthread_t hiloPop;
        pthread_t hiloPush;

};

#endif
