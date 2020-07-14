#include "Misil.h"

Misil::Misil( int x_parm, int y_parm, int id_parm){
    std::string jug = "jugador"+std::to_string(id_parm);

    std::string sp = json.get_sprite_tiro("jugador", jug.c_str());
    if(! misilTexture.loadFromFile(sp.c_str())){
        jug = "No se pudo cargar la imagen del disparo para " + jug;
        logger.error(jug.c_str());
        //sp = json.get_imagen_default("tiro");
        sp = json.get_imagen_default("nave");
        misilTexture.loadFromFile(sp.c_str());
    }

    alto = MISIL_HEIGHT;
    ancho = MISIL_WIDTH;
    id = id_parm;

    x = x_parm;
    y = y_parm;
    vel = MISIL_VEL;

}

void Misil::mover(){
    x += vel;
}

bool Misil::renderizar(){
    bool ok = false;
    if(x < SCREEN_WIDTH && x > 0){
        misilTexture.render(x, y);
        ok = true;
    }
    return ok;
}

int Misil::getAlto(){
    return alto;
}

int Misil::getAncho(){
    return ancho;
}

int Misil::get_id(){
    return id_nave;
}

int Misil::getVel(){
    return vel;
}

