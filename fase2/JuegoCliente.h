#ifndef JUEGOCLIENTE_H_
#define JUEGOCLIENTE_H_

#include "Client.h"
#include "Escenario.h"
#include "Nave.h"
#include "NaveEnemiga.h"
#include "NaveJugador.h"
#include "Menu.h"

class Client;

class JuegoCliente : public Escenario
{

    public:

        JuegoCliente();
        bool iniciarSDL();
        void init_menu();
        void iniciarNivel(Client* client);
        void procesar(posiciones_t* pos);
        void cerrar();
        void cerrar_ventana();
        void reconectar(Client* client);

        void cargarNivel(Client* client);
        void renderBackground();
        void parallax();
        void aumentar_renderizados(int i);

        std::string get_id();
        std::string get_password();
        void render_errorLogin(int intentos, int accion_recibida);
        void cerrarMenu();
        void renderWaitUsers();

    private:
        vector<NaveEnemiga*> enemigos;
        vector<NaveJugador*> jugadores;
        Menu menu;

        int renderizados;

        TextureW gFinNivel;
        SDL_Rect dataFinNivel;

        TextureW gBGTexture;
        TextureW gCiudadTexture;
        TextureW gPlanetaTexture;
        TextureW gNube1;
        TextureW gNube2;

        SDL_Rect dataCiudad;
        SDL_Rect dataBG;
        SDL_Rect dataNube1;
        SDL_Rect dataNube2;

        double scrollingOffsetBG;

        double scrollingOffsetCity;
        double tierraInicial;
        double scrollingOffsetNube1;
        double scrollingOffsetNube2;
};

#endif
