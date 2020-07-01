#ifndef COLAMULTIHILOSERVER_H_
#define COLAMULTIHILOSERVER_H_

#include <queue>
#include <pthread.h>
#include "typesmsj.h"

class ColaMultihiloServer{

    public:
        ColaMultihiloServer(){
            elements = 0;
            pthread_mutex_init(&mutex, NULL);
        }

        void push(void* dato){
            pthread_mutex_lock(&mutex);
            cola.push(dato);
            elements++;
            pthread_mutex_unlock(&mutex);
        }

        client_vw_t* pop(){
            pthread_mutex_lock(&mutex);
            client_vw_t* data = cola.front();
            cola.pop();
            elements--;
            pthread_mutex_unlock(&mutex);
            return data;
        }

        bool estaVacia(){
            return elements;
        }

    private:
        std::queue<void*> cola;
        pthread_mutex_t mutex;
        int elements;
};

#endif
