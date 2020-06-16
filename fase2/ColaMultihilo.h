#ifndef COLAMULTIHILO_H_
#define COLAMULTIHILO_H_

#include <queue>

class ColaMultihilo{

    public:

        void push(T dato){
            mutex.lock();
            cola.push(dato);
            mutex.unlock();
        }

        T pop(){
            mutex.lock();
            T data = cola.pop();
            mutex.unlock();
            return data;
        }

        bool estaVacia(){
            return cola.empty();
        }

    private:
        queue<T> cola;
};

#endif
