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

        virtual void cerrar();

    protected:
        int puerto;
        int socket;
        pthread_mutex_t mutex;

};

#endif
