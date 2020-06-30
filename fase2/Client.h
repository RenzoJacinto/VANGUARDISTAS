#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"
#include "Juego.h"

class Client: public Estado{

    public:

        Client(char* IP, int port);
        bool iniciar();

        //hilos
        void* encolar();
        void* desencolar();
        void* enviar();

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
        pthread_t hiloEnviar;

        Juego juego;

        struct position* pos;
        int size_pos;

};

#endif
