#ifndef MENU_H
#define MENU_H

#include "Escenario.h"
#include "TextureW.h"

class Menu: public Escenario
{
    public:
        Menu();
        bool cargarImagen();
        bool procesar();
        void cerrar();

    private:
        SDL_Rect dataMenu;

        TextureW gMenuBGTexture;
        TextureW gTxt;
};


#endif
