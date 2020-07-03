#ifndef COLAMULTIHILO_H_
#define COLAMULTIHILO_H_
#include "Estado.h"
#include <queue>
#include <pthread.h>

class ColaMultihilo{

    public:

        void* push(void* dato){
            pthread_mutex_lock(&mutex);
            cola.push(dato);
            pthread_mutex_unlock(&mutex);
            return NULL;
        }

        void* pop(){
            pthread_mutex_lock(&mutex);
            void* data = cola.front();
            cola.pop();
            pthread_mutex_unlock(&mutex);
            return data;
        }

        bool estaVacia(){
            return cola.empty();
        }

    private:
        std::queue<void*> cola;
        pthread_mutex_t mutex;
};

#endif
