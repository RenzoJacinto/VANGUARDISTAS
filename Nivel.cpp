#include "Nivel.h"
#include "Nave.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Temporizador.h"
#include "global.h"

Nivel::Nivel(){}

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
    return enemigos;
}

bool Nivel::procesar(){

	    bool quit = false;

	    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
	    Temporizador temporizador;
	    temporizador.iniciar();

        NaveJugador* jugador = new NaveJugador( sdl.getScreenWidth() / 4, sdl.getScreenWidth() / 4);
        logger.debug("Se creó a la nave del jugador");

        vector<NaveEnemiga*> enemigos = crear_enemigos();
        logger.debug("Se crearon los enemigos");
        float tiempo_por_enemigos = TIEMPO_NIVEL_SEGS/cantidad_enemigos;
        double tiempo_nivel = 0;
        int renderizados = 1;

	    // Mientras que siga corriendo la app
	    while( usuarioNoRequieraSalir(quit) && tiempo_nivel < TIEMPO_NIVEL_SEGS ) {
		    while( hayEventos() ){
		         if( eventoEsSalir() ) quit = true;
			      jugador->handleEvent( e );
			 }

			jugador->mover(enemigos);

            SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( sdl.getRenderer() );
			renderBackground();

            //render jugador
            jugador->renderizar();

            //Todo este bloque deberiamos declararlo en otro lado
            for(int i = 0; i < renderizados && i < cantidad_enemigos; i++){
                enemigos[i]->mover(jugador);
                enemigos[i]->renderizar();
            }

            tiempo_nivel = temporizador.transcurridoEnSegundos();
            if(tiempo_nivel/renderizados > tiempo_por_enemigos) renderizados++;

            //Hasta aca

			SDL_RenderPresent( sdl.getRenderer() );
        }
        vector<NaveEnemiga*>::iterator pos;
        // CIERRA LAS NAVES
        jugador->cerrarNave();
        for(pos = enemigos.begin(); pos != enemigos.end(); pos++){
            (*pos)->cerrarNave();
        }
        return quit;
}

void Nivel::finalizar() {
    logger.info("Finalizó el nivel");
    gFinNivel.render(0,0);
    SDL_RenderPresent( sdl.getRenderer() );
    logger.info("Se renderizo el final del nivel");
}


void Nivel::cerrar(){}

void Nivel::cargarNivel(){}

void Nivel::renderBackground(){}
