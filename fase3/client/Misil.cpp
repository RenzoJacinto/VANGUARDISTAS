#include "Misil.h"

Misil::Misil( int x_parm, int y_parm, int id_parm){
    int div = id_parm / 10;
    int rem = id_parm % 10;

    id_lanzada = div;
    id_recibida = rem;

    std::string jug = "jugador"+std::to_string(id_lanzada);

    std::string sp = json.get_sprite_nave("jugador", jug.c_str(), "shot");
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
    bool ok = false;
    if(id_recibida == 0){
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
    return id_lanzada*10 + id_recibida;
}

int Misil::getVel(){
    return vel;
}

