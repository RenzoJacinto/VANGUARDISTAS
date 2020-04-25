#include "Escenario.h"
#include "global.h"
#include "ManejoDeSDL.h"

Escenario::Escenario(){}

bool Escenario::cargarImagen(){return true;}

void Escenario::procesar(){}

bool Escenario::usuarioNoRequieraSalir(bool quit){
    return !quit;
}

bool Escenario::hayEventos(){
    return SDL_PollEvent( &e ) != 0;
}

bool Escenario::eventoEsSalir(){
    return e.type == SDL_QUIT;
}
