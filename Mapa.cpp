#include "Mapa.h"
#include "NaveJugador.h"

Mapa::Mapa(){}

bool Mapa::cargarImagen(){

	//Loading success flag
	bool ok = true;

    // Cargar la textura del fondo
	if( !gBGTexture.loadFromFile( "sprites/bg.png" ) ){
		ok = false;
	}

	// Cargar la textura de la ciudad
	if( !gCiudadTexture.loadFromFile( "sprites/ciudad.png" ) ){
        ok = false;
	}

	// Cargar la textura del planeta
	if( !gPlanetaTexture.loadFromFile( "sprites/planeta.png" ) ){
        ok = false;
	}
	return ok;
}

void Mapa::procesar(){

	    bool quit = false;

        NaveJugador* jugador = new NaveJugador( NaveJugador::NAVE_WIDTH / 2, NaveJugador::NAVE_HEIGHT / 2 );

        NaveEnemiga* enemigo = new NaveEnemiga( sdl.getScreenWidth() / 2 , sdl.getScreenHeight() / 2 );

	    double scrollingOffsetBG = 0;
	    double scrollingOffsetCity = 0;
        double tierraInicial = 850;

	    SDL_Rect dataCiudad;
	    SDL_Rect dataPlaneta;
	    SDL_Rect dataBG;

	    dataBG.h = 600;
	    dataBG.w = 800;

	    dataCiudad.h = 450;
	    dataCiudad.w = 2048;

        dataPlaneta.h = 400;
	    dataPlaneta.w = 400;

	    //gDotTexture.setWidth(75);
	    //gDotTexture.setHeight(32);

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
}

void Mapa::cerrar(){
	gBGTexture.free();
	gCiudadTexture.free();
	gPlanetaTexture.free();
}


