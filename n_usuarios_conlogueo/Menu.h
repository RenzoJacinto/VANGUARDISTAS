#ifndef MENU_H
#define MENU_H

#include "Escenario.h"
#include "TextureW.h"
#include "BotonIniciar.h"

class Menu: public Escenario
{
    public:
        Menu();
        bool cargarImagen();
        void procesar();
        void cerrar();
        void renderErrorLoguin(const char* intentos);

        std::string get_id();
        std::string get_pass();

    private:
        BotonIniciar bt;

        SDL_Rect dataMenu;

        TextureW gMenuBGTexture;
        TextureW gTxt;
};


#endif
