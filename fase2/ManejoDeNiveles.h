#ifndef MANEJODENIVELES_H_
#define MANEJODENIVELES_H_

#include <list>
#include "Nivel.h"
#include "typesmsj.h"

using namespace std;

class ManejoDeNiveles{

    public:

        ManejoDeNiveles();
        void cerrar_niveles(list<Nivel*> niveles);
        void procesar(position_t* pos);
        void delay(int sec);
        void cargarNiveles();

    private:
        std::list<Nivel*> niveles;
};

#endif

