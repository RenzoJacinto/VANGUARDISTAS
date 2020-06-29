#ifndef JUEGO_H_
#define JUEGO_H_

#include "Menu.h"


class Juego{

    public:
        Juego();
        void init_menu();
        std::string get_id();
        std::string get_password();
        void render_errorLoguin(int intentos);

        void cerrar();

    private:
        Menu menu;


};

#endif
