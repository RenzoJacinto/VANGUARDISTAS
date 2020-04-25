#include "Mapa.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "Nave.h"

Mapa::Mapa(){}

void Mapa::procesar(){

        cargarImagen("sprites/bg.png", &gBGTexture);
        cargarImagen("sprites/ciudad.png", &gCiudadTexture);
        cargarImagen("sprites/planeta.png", &gPlanetaTexture);

	    bool quit = false;

        NaveJugador* jugador = new NaveJugador( NaveJugador::NAVE_WIDTH / 2, NaveJugador::NAVE_HEIGHT / 2 , "sprites/nave.png");
        if (logger.seDebeInformarDebug()) logger.informar("Se creó a la nave del jugador");

        NaveEnemiga* enemigo = new NaveEnemiga( sdl.getScreenWidth() / 2 , sdl.getScreenHeight() / 2 , "sprites/enemigo.png");
        if (logger.seDebeInformarDebug()) logger.informar("Se creó nave enemiga");

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

	    //gDotTexture.setWidth(75);
	    //gDotTexture.setHeight(32);

	    if (logger.seDebeInformarInfo()) logger.informar("Se mostró el mapa");

	    // Mientras que siga corriendo la app
	    while( usuarioNoRequieraSalir(quit) ) {
		    while( hayEventos() ){
		         if( eventoEsSalir() ) quit = true;
			      jugador->handleEvent( e );
			 }

			jugador->mover(enemigo);

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

             enemigo->mover( jugador );
             enemigo->renderizar();

			 SDL_RenderPresent( sdl.getRenderer() );

        }
        if (logger.seDebeInformarInfo()) logger.informar("Se volvió al menu");
}

void Mapa::cerrar(){
	gBGTexture.free();
	gCiudadTexture.free();
	gPlanetaTexture.free();
}


