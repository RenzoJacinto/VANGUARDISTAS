#ifndef MENU_H
#define MENU_H

#include "Escenario.h"
#include "TextureW.h"
#include "Mapa.h"

class Menu: public Escenario
{
    public:
        Menu();
        bool cargarImagen();
        void procesar();
        void cerrar();

    private:

        TextureW gMenuBGTexture;
};


#endif
