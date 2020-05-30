#ifndef ESTADO_H_
#define ESTADO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>

class Estado{

    public:

        Estado();
        virtual int inicializar(int* args);

        struct Command {
            int action;
        };

        struct View {
            int positionX;
            int positionY;
        };

};

#endif
