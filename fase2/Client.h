#ifndef CLIENT_H_
#define CLIENT_H_

#include "Estado.h"

class Client: public Estado{

    public:

        Client();
        int inicializar(int* args);
        int receiveData(int* client_socket, struct View* client_view);
        int sendData(int* client_socket, struct Command* client_command);

};

#endif
