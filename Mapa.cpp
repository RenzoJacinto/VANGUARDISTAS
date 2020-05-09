#include "Mapa.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Nave.h"
#include <list>

Mapa::Mapa(){}

void Mapa::procesar(){

        const char* sMapaBG = json.get_sprite_MapaBG();
        const char* sCiudad = json.get_sprite_Ciudad();
        const char* sPlaneta = json.get_sprite_Planeta();

        cargarImagen(sMapaBG, &gBGTexture);
        cargarImagen(sCiudad, &gCiudadTexture);
        cargarImagen(sPlaneta, &gPlanetaTexture);

	    bool quit = false;

        NaveJugador* jugador = new NaveJugador( sdl.getScreenWidth() / 4, sdl.getScreenWidth() / 4);
        logger.debug("Se creó a la nave del jugador");

        //Todo este bloque deberiamos declararlo en otro lado
        list<NaveEnemiga*> enemigos;

        for(int i = 1; i <= 6; i++)
        {
            NaveEnemiga* enemigo = new NaveEnemiga( rand() % sdl.getScreenWidth() , rand() % sdl.getScreenHeight() );
            logger.debug("Se creó una nave enemiga");
            enemigos.push_back(enemigo);
        }
        //Hasta aca

	    double scrollingOffsetBG = 0;
	    double scrollingOffsetCity = 0;
        double tierraInicial = 850;

	    SDL_Rect dataCiudad;
	    SDL_Rect dataBG;

	    dataBG.h = 600;
	    dataBG.w = 800;
	    dataBG.x = 0;
	    dataBG.y = 0;

	    dataCiudad.h = 450;
	    dataCiudad.w = 2048;
	    dataCiudad.x = 0;
	    dataCiudad.y = 0;

	    logger.info("Se mostró el mapa");

	    // Mientras que siga corriendo la app
	    while( usuarioNoRequieraSalir(quit) ) {
		    while( hayEventos() ){
		         if( eventoEsSalir() ) quit = true;
			      jugador->handleEvent( e );
			 }

			jugador->mover(enemigos);

			 //Scroll background
			 scrollingOffsetBG -= 0.1;
			 if( scrollingOffsetBG < -dataBG.w ) scrollingOffsetBG = 0;


			 scrollingOffsetCity -= 5;
		 	 if( scrollingOffsetCity < -dataCiudad.w ) scrollingOffsetCity = 0;

			 SDL_SetRenderDrawColor( sdl.getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF );
			 SDL_RenderClear( sdl.getRenderer() );

			 gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
			 gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

			 gPlanetaTexture.render(tierraInicial, -50);
             tierraInicial -= 0.2;

			 gCiudadTexture.render(scrollingOffsetCity, 150, &dataCiudad);
			 gCiudadTexture.render(scrollingOffsetCity + dataCiudad.w, 150, &dataCiudad);

			 jugador->renderizar();

			 //Todo este bloque deberiamos declararlo en otro lado
             list<NaveEnemiga*>::iterator pos;
             for(pos = enemigos.begin(); pos != enemigos.end(); pos++)
             {
                (*pos)->mover( jugador );
                (*pos)->renderizar();
             }
             //Hasta aca

			 SDL_RenderPresent( sdl.getRenderer() );

        }
        logger.info("Se volvió al menu");
}

void Mapa::cerrar(){
	gBGTexture.free();
	gCiudadTexture.free();
	gPlanetaTexture.free();
}



