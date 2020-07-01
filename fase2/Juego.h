#ifndef JUEGO_H_
#define JUEGO_H_

#include "Menu.h"
#include "ManejoDeNiveles.h"
#include "typesmsj.h"

class Juego{

    public:
        Juego();
        void init_menu();
        std::string get_id();
        std::string get_password();

        void cerrarMenu();
        void cerrar();

        void render_errorLoguin(int intentos);
        void iniciar(position_t* pos);
        void pushDato(client_vw_t* client_view);

        static const int CARGADO = 1;
        static const int VACIO = 0;

    private:
        Menu menu;
        ManejoDeNiveles niveles;

};

#endif
