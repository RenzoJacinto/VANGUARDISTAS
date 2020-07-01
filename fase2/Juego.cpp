#include "Juego.h"
#include "global.h"
#include "TextureW.h"

Juego::Juego(){

    if(! sdl.iniciarSDL()) logger.error("Fallo la inicializacion de SDL");
    for(int i=0; i<4;i++){
        bit_enemigos[i] = VACIO;
        bit_jugadores[i] = VACIO;
    }
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
}

void Juego::cerrar(){
    sdl.cerrar();
}

void Juego::render_errorLoguin(int intentos){

    std::string intentos_rest = std::to_string(intentos);
    menu.renderErrorLoguin(intentos_rest.c_str());
}

void Juego::iniciar(position_t* pos){
    ManejoDeNiveles niveles;
    niveles.cargarNiveles();
    niveles.procesar(pos);
}
void Juego::renderNave(client_vw_t* client_view){
    TextureW texture;
    std::string sprite;
    std::string nave;
    if(client_view->tipo_nave == TIPO_JUGADOR){
        nave = "nave" + std::to_string(client_view->serial);
        sprite = json.get_sprite_nave("jugador", nave.c_str());
        if(! texture.loadFromFile(sprite.c_str())){
            sprite = json.get_imagen_default("nave");
            texture.loadFromFile(sprite.c_str());
        }
    } else if(client_view->tipo_nave == TIPO_ENEMIGO){
        nave = "enemigo" + std::to_string(client_view->serial);
        sprite = json.get_sprite_nave("enemigas", nave.c_str());
        if(! texture.loadFromFile(sprite.c_str())){
            sprite = json.get_imagen_default("nave");
            texture.loadFromFile(sprite.c_str());
        }
    }

    texture.render(client_view->x, client_view->y);
    SDL_RenderPresent(sdl.getRenderer());
}
