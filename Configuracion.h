#ifndef CONFIGURACION_H_
#define CONFIGURACION_H_

using namespace std;

#include <string>

class Configuracion{
    public:
        Configuracion();

        std::string menuBG;
        std::string mapaBG;
        std::string ciudad;
        std::string planeta;

        std::string sprite_enemigo1;
        int cant_enemigo1;

        std::string sprite_enemigo2;
        int cant_enemigo2;

        std::string jugador;

        int log;
};

#endif
