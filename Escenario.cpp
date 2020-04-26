#include "Escenario.h"
#include "global.h"

Escenario::Escenario(){}

void Escenario::cargarImagen(const char* imagen, TextureW* textura){
    // Cargar la textura de la nave
	if( textura->loadFromFile(imagen)){
        std::string frase = "Se cargó la imagen ";
        string direccion(imagen);
        frase = frase + direccion;
        const char* update = frase.c_str();
        logger.info(update);
    }
}

bool Escenario::usuarioNoRequieraSalir(bool quit){
    return !quit;
}

bool Escenario::hayEventos(){
    return SDL_PollEvent( &e ) != 0;
}

bool Escenario::eventoEsSalir(){
    return e.type == SDL_QUIT;
}

