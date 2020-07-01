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

        void pushDato(client_vw_t* client_view);

        static const int CANT_NIVELES = 3;
    private:
        std::list<Nivel*> niveles;
        int actual_nivel;
};

#endif
