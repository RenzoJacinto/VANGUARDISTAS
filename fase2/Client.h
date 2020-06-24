#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"

class Client: public Estado{

    public:

        Client(char* IP, int port);
        bool iniciar();
        bool sendData();
        bool receiveData();
        void processData();
        bool iniciarSesion();
        void close();

    private:
        std::string ip;

        //thread_t hiloPop;
        //thread_t hiloPush;

};

#endif
