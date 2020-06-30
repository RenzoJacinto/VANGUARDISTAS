#ifndef ESTADO_H_
#define ESTADO_H_

#include <string>
#include <time.h>
#include <ctime>

#include "ColaMultihilo.h"
#include "TextureW.h"
#include "typesmsj.h"

class Estado{

    public:
        Estado();
        virtual bool iniciar();
        virtual int sendData(void* dato);
        virtual int receiveData();
        virtual void processData(void* dato);
        virtual void cerrar();

    protected:
        int puerto;
        int socket;
        pthread_mutex_t mutex;

};

#endif
