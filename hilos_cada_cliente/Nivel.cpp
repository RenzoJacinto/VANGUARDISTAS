#include "Nivel.h"

Nivel::Nivel(){
    dataFinNivel.h = 600;
    dataFinNivel.w = 800;
    dataFinNivel.x = 0;
    dataFinNivel.y = 0;
}

//manejoSDL
//Menu
//TextureW
//BotonIniciar

int get_nave(vector<NaveJugador*> jugadores, int id) {
    int k = 0;
    vector<NaveJugador*>::iterator pos;
    for(pos = jugadores.begin(); pos != jugadores.end(); pos++){
//        printf("id buscada: %d, id de iteracion: %d\n", id, (*pos)->get_id());
        if(id == (*pos)->get_id()) break;
        k++;
    }
    return k;
}

vector<NaveEnemiga*> Nivel::crear_enemigos(){
    vector<NaveEnemiga*> enemigos(cantidad_enemigos);
    for(int i=0; i<cantidad_enemigos; i++){

        // Elige un enemigo random
        std::string sprite = "enemigo";
        int enemigo_random = 1 + rand() % 4;
        sprite += std::to_string(enemigo_random);

        // Obtencion de la posicion pos = inf + rand()%(sup+1-inf)
        int y = 50 + rand() % (sdl.getScreenHeight() + 1);
        // SUPONGO EL BORDE DE RESPAWN COMO +/-100
        // CASO ENEMIGOS 1 y 2: sup = 800 + 100
        int x = sdl.getScreenWidth() + rand() % 51;

        // CASO ENEMIGOS 3 y 4: inf = -100
        if(enemigo_random == 4 || enemigo_random == 3) x = -50 + rand() % 51;

        NaveEnemiga* enemigo = new NaveEnemiga(x, y, sprite.c_str());

        enemigos[i]=enemigo;
    }
    std::string mensaje = "Se crearon los " + std::to_string(cantidad_enemigos) + " enemigos";
    logger.debug(mensaje.c_str());
    return enemigos;
}

bool Nivel::procesar(){return true;}

void Nivel::finalizar() {
    logger.info("Finalizó el nivel");
    gFinNivel.render(0,0, &dataFinNivel);
    SDL_RenderPresent( sdl.getRenderer() );
    logger.info("Se renderizo el final del nivel");
}


void Nivel::cerrar(){}

void Nivel::cargarNivel(){}

void Nivel::renderBackground(){}
