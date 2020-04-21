#ifndef MANEJODESDL_H_
#define MANEJODESDL_H_

#include "TextureW.h"

class ManejoDeSDL{
    public:

        ManejoDeSDL();

        //Inicia SDL y crea una ventana
        bool iniciarSDL();

        //Carga una imagen
        bool cargarImagen();

        bool cargarImagenMenu();

        //Libera espacio en memoria y cierra SDL
        void cerrar();

        void procesoMenu();

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


        //void aplicarImagen();

        //void actualizarSuperficie();

        bool hayEventos();

        bool usuarioNoRequieraSalir(bool quit);

        SDL_Event getEvento();

        bool eventoEsSalir();

        //nuevo para la aparcicion de la nave en pantalla con movimiento
        void renderNave(int x, int y);

    private:

        int screenHeight, screenWidth;

        //Scene textures
        TextureW gMenuBGTexture;
        TextureW gNaveTexture;
        TextureW gBGTexture;

        //Event handler
        SDL_Event e;

        SDL_Window* gWindow;

        SDL_Renderer* gRenderer;

};

#endif
