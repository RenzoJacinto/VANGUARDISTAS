#ifndef MANEJODENIVELES_H_
#define MANEJODENIVELES_H_

#include <list>
#include "Mapa.h"

using namespace std;

class ManejoDeNiveles{

    public:

        ManejoDeNiveles();
        void procesar();

    private:
        std::list<Mapa*> niveles;
};

#endif

