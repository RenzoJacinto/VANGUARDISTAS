#include "Escenario.h"
#include "global.h"

Escenario::Escenario(){}

void Escenario::cargarImagen(const char* nivel, const char* sprite, TextureW* textura){

    std::string imagen = json.get_sprite_mapa(nivel, sprite);
    // Cargar la textura de la nave
	if( textura->loadFromFile(imagen.c_str())){
        std::string frase = "Se cargó la imagen ";
        string direccion(imagen);
        frase = frase + direccion;
        const char* update = frase.c_str();
        logger.info(update);
    } else{
        imagen = json.get_sprite_mapa_default(nivel, sprite);
        std::string mensaje = "No se encontro el sprite: " + imagen + ", se cargo una por defecto";
        logger.error(mensaje.c_str());
        textura->loadFromFile(imagen.c_str());
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

