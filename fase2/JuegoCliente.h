#ifndef JUEGOCLIENTE_H_
#define JUEGOCLIENTE_H_

#include "Client.h"
#include "Escenario.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Menu.h"
#include "Nivel.h"

class Client;
class Nivel;

class JuegoCliente : public Escenario
{

    public:

        JuegoCliente();
        bool iniciarSDL();
        void init_menu();
        void iniciarJuego(Client* client, int nivel);
        //void procesar(posiciones_t* pos);
        void cerrar();
        void cerrar_ventana();
        void reconectar(Client* client, int nivel);
        void reconectarSiguiente(Client* client, int nivel);

        //void renderBackground();
        //void parallax();
        //void aumentar_renderizados(int i);

        std::string get_id();
        std::string get_password();
        void render_errorLogin(int intentos, int accion_recibida);
        void cerrarMenu();
        void renderWaitUsers();
        void renderServerLleno();

        void renderServerCaido();

    private:

        Menu menu;

        vector<Nivel*> niveles;
};

#endif
