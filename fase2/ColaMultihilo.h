#ifndef COLAMULTIHILO_H_
#define COLAMULTIHILO_H_

#include <queue>
#include <pthread.h>
#include "typesmsj.h"


class ColaMultihilo{

    public:
        ColaMultihilo();
        void push(client_vw_t* dato);
        client_vw_t* pop();
        bool estaVacia();

    private:
        std::queue<client_vw_t*> cola;
        pthread_mutex_t mutex;
        int elements;
};

#endif
