#ifndef SERVER_H_
#define SERVER_H_

#include "Estado.h"

class Server: public Estado{

    public:

        Server();
        int inicializar(int* args);
        void initializeData(struct View* client_view);
        void processData(int action, struct View* view);
        int receiveData(int* client_socket, struct Command* client_command);
        int sendData(int* client_socket, struct View* client_view);
};

#endif
