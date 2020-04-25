#ifndef MENU_H
#define MENU_H

#include "Escenario.h"
#include "TextureW.h"

class Menu: public Escenario
{
    public:
        Menu();
        void procesar();
        void cerrar();

    private:

        TextureW gMenuBGTexture;
};


#endif
