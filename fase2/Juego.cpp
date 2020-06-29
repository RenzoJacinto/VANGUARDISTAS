#include "Juego.h"
#include "global.h"
#include "TextureW.h"

Juego::Juego(){
    //gErrorLoguin.loadFromFile("s");
    if(! sdl.iniciarSDL()) logger.error("Fallo la inicializacion de SDL");
}

void Juego::init_menu(){
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

void Juego::render_errorLoguin(int intentos){

    std::string intentos_rest = std::to_string(intentos);
    menu.renderErrorLoguin(intentos_rest.c_str());
}
