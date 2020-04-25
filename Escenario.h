#ifndef ESCENARIO_H_
#define ESCENARIO_H_

#include <SDL2/SDL.h>
#include "TextureW.h"

class Escenario{

    public:

        Escenario();
        void cargarImagen(const char* imagen, TextureW* textura);
        void procesar();
        bool usuarioNoRequieraSalir(bool quit);
        bool hayEventos();
        bool eventoEsSalir();
        void cerrar();

    protected:

        SDL_Event e;

};

#endif
