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
        void jugar();

        int sendData(position_t* dato, int size_data);
        int receiveData(client_vw_t* dato, int size_data);
        void processData(client_vw_t* dato);

        bool iniciarSesion();

        void cerrar();

    private:
        std::string ip;
        ColaMultihilo* cola;
        pthread_t hiloPop;
        pthread_t hiloPush;
        pthread_t hiloEnviar;
        pthread_t hiloJuego;

        Juego juego;

        position_t* pos;
        position_t pos_ant;
        int size_pos;

};

#endif
