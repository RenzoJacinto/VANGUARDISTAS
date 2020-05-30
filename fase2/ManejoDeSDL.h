#ifndef MANEJODESDL_H_
#define MANEJODESDL_H_

#include "TextureW.h"

class ManejoDeSDL{
    public:

        ManejoDeSDL();

        //Inicia SDL y crea una ventana
        bool iniciarSDL();

        //Libera espacio en memoria y cierra SDL
        void cerrar();

        bool huboErrorAlIniciarSDL();

        void setWindow(SDL_Window* window);

        void setRenderer(SDL_Renderer* renderer);

        SDL_Window* getWindow();

        SDL_Renderer* getRenderer();

        void setScreenWidth(int width);

        void setScreenHeight(int height);

        int getScreenWidth();

        int getScreenHeight();

        void renderNave(int x, int y);

        void renderEnemigo(int x, int y);


    private:

        int screenHeight, screenWidth;

        SDL_Window* gWindow;

        SDL_Renderer* gRenderer;

};

#endif
