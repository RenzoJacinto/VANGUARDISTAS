#include "Nivel.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Temporizador.h"

Nivel::Nivel(){}

int Nivel::get_width(){
    return rand() % sdl.getScreenWidth();
}

int Nivel::get_height(){
    return rand() % sdl.getScreenHeight();
}

list<NaveEnemiga*> Nivel::crear_enemigos(){
        list<NaveEnemiga*> enemigos;
        int cantidad_enemigos_1 = json.get_cantidad_enemigo_1();
        std::string enemigo1 = json.get_sprite_nave("enemigas", "enemigo1");
        const char* sEnemigo1 = enemigo1.c_str();
        for(int i = 1; i <= cantidad_enemigos_1; i++){
            NaveEnemiga* enemigo = new NaveEnemiga(get_width(), get_height(), sEnemigo1);
            logger.debug("Se creó una nave enemiga 1");
            enemigos.push_back(enemigo);
        }
        int cantidad_enemigos_2 = json.get_cantidad_enemigo_2();
        std::string enemigo2 = json.get_sprite_nave("enemigas", "enemigo2");
        const char* sEnemigo2 = enemigo2.c_str();
        for(int i = 1; i <= cantidad_enemigos_2; i++){
            NaveEnemiga* enemigo = new NaveEnemiga(get_width(), get_height(), sEnemigo2);
            logger.debug("Se creó una nave enemiga 2");
            enemigos.push_back(enemigo);
        }
        int cantidad_enemigos_3 = json.get_cantidad_enemigo_3();
        std::string enemigo3 = json.get_sprite_nave("enemigas", "enemigo3");
        const char* sEnemigo3 = enemigo3.c_str();
        for(int i = 1; i <= cantidad_enemigos_3; i++){
            NaveEnemiga* enemigo = new NaveEnemiga(get_width(), get_height(), sEnemigo3);
            logger.debug("Se creó una nave enemiga 3");
            enemigos.push_back(enemigo);
        }
        int cantidad_enemigos_4 = json.get_cantidad_enemigo_4();
        std::string enemigo4 = json.get_sprite_nave("enemigas", "enemigo4");
        const char* sEnemigo4 = enemigo4.c_str();
        for(int i = 1; i <= cantidad_enemigos_4; i++){
            NaveEnemiga* enemigo = new NaveEnemiga(get_width(), get_height(), sEnemigo4);
            logger.debug("Se creó una nave enemiga 4");
            enemigos.push_back(enemigo);
        }
        return enemigos;
}

void Nivel::procesar(){

	    bool quit = false;
	    bool fin_mapa = false;

	    //Inicializo el temporizador. La duracion de cada nivel podriamos tomarla del archivo Json
	    Temporizador temporizador;
	    temporizador.iniciar();

        NaveJugador* jugador = new NaveJugador( sdl.getScreenWidth() / 4, sdl.getScreenWidth() / 4);
        logger.debug("Se creó a la nave del jugador");

        list<NaveEnemiga*> enemigos = crear_enemigos();

	    logger.info("Se creó el arreglo de enemigos");

	    // Mientras que siga corriendo la app               //Puse solo 5 segundos para probar
	    while( usuarioNoRequieraSalir(quit) && !fin_mapa && (temporizador.transcurridoEnSegundos() < 5) ) {
		    while( hayEventos() ){
		         if( eventoEsSalir() ) quit = true;
			      jugador->handleEvent( e );
			 }

			jugador->mover(enemigos);

            SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( sdl.getRenderer() );
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
        logger.info("Finalizó el nivel");
}

void Nivel::cerrar(){}

void Nivel::cargarNivel(){}

bool Nivel::renderBackground(){return true;}
