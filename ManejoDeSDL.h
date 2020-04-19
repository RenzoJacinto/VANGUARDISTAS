#ifndef MANEJODESDL_H_
#define MANEJODESDL_H_

#include "Fondo.h"

class ManejoDeSDL
{
    public:

        ManejoDeSDL();

        //Inicia SDL y crea una ventana
        bool iniciarSDL();

        //Carga una imagen
        bool cargarImagen();

        //Libera espacio en memoria y cierra SDL
        void cerrar();

        void proceso();

        bool huboErrorAlIniciarSDL();

        void setWindow(SDL_Window* window);

        void setRenderer(SDL_Renderer* renderer);

        SDL_Window* getWindow();

        SDL_Renderer* getRenderer();

        void setScreenWidth(int width);

        void setScreenHeight(int height);

        int getScreenWidth();

        int getScreenHeight();

        Fondo getDotTexture();

        Fondo getBGTexture();

        //void aplicarImagen();

        //void actualizarSuperficie();

        bool hayEventos();

        bool usuarioNoRequieraSalir(bool quit);

        SDL_Event getEvento();

        bool eventoEsSalir();

    private:

        int screenHeight, screenWidth;

        //Scene textures
        Fondo gDotTexture;
        Fondo gBGTexture;

        //Event handler
        SDL_Event e;

        SDL_Window* gWindow;

        SDL_Renderer* gRenderer;

};

#endif
