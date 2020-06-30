#ifndef COLAMULTIHILO_H_
#define COLAMULTIHILO_H_

#include <queue>
#include <pthread.h>

class ColaMultihilo{

    public:
        ColaMultihilo(){
            elements = 0;
            pthread_mutex_init(&mutex, NULL);
        }

        void push(void* dato){
            pthread_mutex_lock(&mutex);
            cola.push(dato);
            elements++;
            pthread_mutex_unlock(&mutex);
        }

        void* pop(){
            pthread_mutex_lock(&mutex);
            void* data = cola.front();
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
