#ifndef MANEJODENIVELES_H_
#define MANEJODENIVELES_H_

#include <list>
#include "Nivel.h"
#include "typesmsj.h"
#include "ColaMultihilo.h"

using namespace std;

class ManejoDeNiveles{

    public:

        ManejoDeNiveles();
        void cerrar_niveles(list<Nivel*> niveles);
        void procesar(position_t* pos);
        void delay(int sec);
        void cargarNiveles();

        void procesarServer(ColaMultihilo* cola);

        static const int CANT_NIVELES = 3;
    private:
        std::list<Nivel*> niveles;
};

#endif

