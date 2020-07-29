#include "Misil.h"

Misil::Misil( int x_parm, int y_parm, int id_parm){

    id = id_parm;

    std::string tipo = "jugador";
    std::string jug = "jugador"+std::to_string(id_parm);
    if(id>3){
        tipo = "enemigas";
        jug = "enemigo1";
    }

    std::string sp = json.get_sprite_nave(tipo.c_str(), jug.c_str(), "shot");
    if(! misilTexture.loadFromFile(sp.c_str())){
        jug = "No se pudo cargar la imagen del disparo para " + jug;
        logger.error(jug.c_str());
        //sp = json.get_imagen_default("tiro");
        sp = json.get_imagen_default("tiro");
        misilTexture.loadFromFile(sp.c_str());
    }

    alto = MISIL_HEIGHT;
    ancho = MISIL_WIDTH;

    x = x_parm;
    y = y_parm;
    vel = MISIL_VEL;

}

void Misil::mover(){
    x += vel;
}

bool Misil::renderizar(){


    misilTexture.render(x, y);

    return true;
}

int Misil::getAlto(){
    return alto;
}

int Misil::getAncho(){
    return ancho;
}

int Misil::get_id(){
    return id;
}

int Misil::getVel(){
    return vel;
}

