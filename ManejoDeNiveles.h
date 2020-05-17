#ifndef MANEJODENIVELES_H_
#define MANEJODENIVELES_H_

#include <list>
#include "Nivel.h"

using namespace std;

class ManejoDeNiveles{

    public:

        ManejoDeNiveles();
        void procesar();

    private:
        std::list<Nivel*> niveles;

        TextureW fin_nivel1;
};

#endif

