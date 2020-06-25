#ifndef ESTADO_H_
#define ESTADO_H_

#include <string>
#include <time.h>
#include <ctime>

class Estado{

    public:
        Estado();
        virtual bool iniciar();
        virtual bool sendData();
        virtual bool receiveData();
        virtual void processData();
        virtual void close();

    protected:
        int puerto;
        int socket;

        pthread_mutex_t mutex;

        struct client{
            std::string id;
            std::string passwd;
        };

        struct position{
            int x;
            int y;
        };
};

#endif
