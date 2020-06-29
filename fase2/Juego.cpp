#include "Juego.h"
#include "global.h"
#include "TextureW.h"

Juego::Juego(){
    //gErrorLoguin.loadFromFile("s");
}

void Juego::init_menu(){
    if(! sdl.iniciarSDL()) logger.error("Fallo la inicializacion de SDL");
    if (menu.cargarImagen()) menu.procesar();
}

std::string Juego::get_id(){
    return menu.get_id();
}

std::string Juego::get_password(){
    return menu.get_pass();
}

void Juego::cerrar(){
    menu.cerrar();
    sdl.cerrar();
}

void Juego::render_errorLoguin(){
    //gErrorLoguin.render(0,0);
}
