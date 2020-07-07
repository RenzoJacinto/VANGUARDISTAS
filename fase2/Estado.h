#ifndef ESTADO_H_
#define ESTADO_H_

#include <string>
#include <time.h>
#include <ctime>

#include "ColaMultihilo.h"

typedef struct posiciones {
    int id;
    char descrip[15];
    int posX;
    int posY;
} posiciones_t;

typedef struct velocidades {
    int id;
    char descrip[5];
    int VelX;
    int VelY;
} velocidades_t;

typedef struct Credenciales {
    char id[28];
    char pass[28];
} credenciales_t;

static const int LOGIN_CORRECTO = 0;
static const int ERROR_LOGIN = 1;
static const int ID_YA_LOGUEADA = -1;
static const int JUEGO_LLENO = 2;
static const int ID_NO_LOGUEADA_RECON = -2;

class Estado{

    public:
        Estado();
        virtual bool iniciar();
        virtual bool sendData(void* dato);
        virtual void* receiveData();
        virtual void* recibir_encolar();
        virtual void* processData(void* dato);
        virtual void cerrar();
        const char* est;

    protected:
        int puerto;
        int socket;
        pthread_mutex_t mutex;

};

#endif
