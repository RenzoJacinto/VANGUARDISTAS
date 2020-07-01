#include "Juego.h"
#include "global.h"
#include "TextureW.h"
#include "NaveJugador.h"

Juego::Juego(){

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

void Juego::cerrarMenu(){
    menu.cerrar();
    //for(int i = time(NULL) + 5; time(NULL) != i; time(NULL));
}

void Juego::cerrar(){
    sdl.cerrar();
}

void Juego::render_errorLoguin(int intentos){

    std::string intentos_rest = std::to_string(intentos);
    menu.renderErrorLoguin(intentos_rest.c_str());
}

void Juego::iniciar(position_t* pos){
    niveles.cargarNiveles();
    niveles.procesar(pos);
}

void Juego::pushDato(client_vw_t* client_view){
    niveles.pushDato(client_view);
}


