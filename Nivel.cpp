#include "Nivel.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"

Nivel::Nivel(){}

void Nivel::procesar(){

	    bool quit = false;
	    bool fin_mapa = false;

        NaveJugador* jugador = new NaveJugador( sdl.getScreenWidth() / 4, sdl.getScreenWidth() / 4);
        logger.debug("Se creó a la nave del jugador");

        //Todo este bloque deberiamos declararlo en otro lado
        list<NaveEnemiga*> enemigos;
        //int cant_enemigos = json.get_cantidad_enemigos_enemigo1() + json.get_cantidad_enemigos_enemigo2();
        for(int i = 1; i <= 6; i++){
            NaveEnemiga* enemigo = new NaveEnemiga( rand() % sdl.getScreenWidth() , rand() % sdl.getScreenHeight() );
            logger.debug("Se creó una nave enemiga");
            enemigos.push_back(enemigo);
        }
        //Hasta aca

	    logger.info("Se mostró el mapa");

	    // Mientras que siga corriendo la app
	    while( usuarioNoRequieraSalir(quit) && ! fin_mapa) {
		    while( hayEventos() ){
		         if( eventoEsSalir() ) quit = true;
			      jugador->handleEvent( e );
			 }

			jugador->mover(enemigos);

			fin_mapa = renderBackground();

			if(!fin_mapa){
                 //Scroll background
                jugador->renderizar();

                //Todo este bloque deberiamos declararlo en otro lado
                list<NaveEnemiga*>::iterator pos;
                for(pos = enemigos.begin(); pos != enemigos.end(); pos++){
                    (*pos)->mover( jugador );
                    (*pos)->renderizar();
                }
                //Hasta aca
			}

            SDL_RenderPresent( sdl.getRenderer() );

        }

        // CIERRA LAS NAVES
        jugador->cerrarNave();
        list<NaveEnemiga*>::iterator pos;
        for(pos = enemigos.begin(); pos != enemigos.end(); pos++){
            (*pos)->cerrarNave();
        }
        logger.info("Se volvió al menu");
}

void Nivel::cerrar(){}

void Nivel::cargarNivel(){}

bool Nivel::renderBackground(){}

