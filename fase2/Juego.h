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
        void renderNave(client_vw_t* client_view);

        std::string obtenerSpriteNave(int tipo, int serial);

        static const int CARGADO = 1;
        static const int VACIO = 0;

    private:
        Menu menu;

        TextureW texturesJugadores[4];
        TextureW texturesEnemigos[4];


        int bit_jugadores[4];
        int bit_enemigos[4];

};

#endif
