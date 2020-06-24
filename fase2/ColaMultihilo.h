#ifndef COLAMULTIHILO_H_
#define COLAMULTIHILO_H_

#include <queue>

class ColaMultihilo{

    public:

        void push(int dato){
            //mutex.lock();
            cola.push(dato);
            //mutex.unlock();
        }

        int pop(){
            //mutex.lock();
            int data = cola.front();
            cola.pop();
            //mutex.unlock();
            return data;
        }

        bool estaVacia(){
            return cola.empty();
        }

    private:
        std::queue<int> cola;
};

#endif
