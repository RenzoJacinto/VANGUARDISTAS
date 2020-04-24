#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

//#include "Nave.h"
#include "TextureW.h"
#include "ManejoDeSDL.h"
#include "BotonIniciar.h"
#include "NaveJugador.h"
#include "NaveEnemiga.h"
#include "colicionador.h"


//ACA HABRIA QUE CAMBIARLE A 800x600
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

ManejoDeSDL::ManejoDeSDL(){
    setScreenWidth(SCREEN_WIDTH);
    setScreenHeight(SCREEN_HEIGHT);

    //The window we'll be rendering to
    setWindow(NULL);

    //The window renderer
    setRenderer(NULL);
}

bool ManejoDeSDL::iniciarSDL(){
	//Initialization flag
	bool ok = true;

	//Initialize SDL
	if( huboErrorAlIniciarSDL() ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		ok = false;
	}
	else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) printf( "Warning: Linear texture filtering not enabled!" );

		//Create window
		setWindow( SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN ) );
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			ok = false;
		}
		else{
			//Create vsynced renderer for window
			setRenderer( SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) );
			if( gRenderer == NULL ){
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				ok = false;
			}
			else{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ){
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					ok = false;
				}
			}
		}
	}
	return ok;
}

bool ManejoDeSDL::cargarImagen(){
	//Loading success flag
	bool ok = true;

    // Cargar la textura de la nave
	if( !gNaveTexture.loadFromFile( "sprites/nave.png" ) ){
		printf( "Error al cargar la imagen de la nave!\n" );
		ok = false;
	}
	// Cargar la textura de la nave
	if( !gEnemigoTexture.loadFromFile( "sprites/enemigo.png" ) ){
		printf( "Error al cargar la imagen de la nave!\n" );
		ok = false;
	}
    // Cargar la textura del fondo
	if( !gBGTexture.loadFromFile( "sprites/bg.png" ) ){
		printf( "Error al cargar la imagen de fondo!\n" );
		ok = false;
	}

	// Cargar la textura de la ciudad
	if( !gCiudadTexture.loadFromFile( "sprites/ciudad.png" ) ){
        printf( "Error al cargar la imagen de la ciudad" );
        ok = false;
	}

	// Cargar la textura del planeta
	if( !gPlanetaTexture.loadFromFile( "sprites/planeta.png" ) ){
        printf( "Error al cargar la imagen del planeta" );
        ok = false;
	}
	return ok;
}

bool ManejoDeSDL::cargarImagenMenu(){
	//Loading success flag
	bool ok = true;

    // Cargar la textura de la nave
	if( !gMenuBGTexture.loadFromFile( "sprites/menuBG.png" ) ){
		printf( "Error al cargar la imagen del fondo del menú!\n" );
		ok = false;
	}

	return ok;

}

void ManejoDeSDL::cerrar(){
	gNaveTexture.free();
	gEnemigoTexture.free();
	gBGTexture.free();
	gCiudadTexture.free();
	gPlanetaTexture.free();


	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	setWindow(NULL);
	setRenderer(NULL);


	IMG_Quit();
	SDL_Quit();
}

void ManejoDeSDL::procesoMenu(){
        bool quit = false;
        BotonIniciar bt;
        // Mientras que siga corriendo la app
        while( usuarioNoRequieraSalir(quit) ){
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );
            gMenuBGTexture.render( 0, 0 );

            //gBotonIniciarTexture.render(0, 0);

            SDL_RenderPresent( gRenderer );
            while( hayEventos() ){
                if( eventoEsSalir() ) quit = true;
                bt.handleEvent( e );
            }
        }
}

void ManejoDeSDL::proceso() {

	    bool quit = false;

	    //Nave nave;

        NaveJugador* dot = new NaveJugador( NaveJugador::DOT_WIDTH / 2, NaveJugador::DOT_HEIGHT / 2 );

        NaveEnemiga* otherDot = new NaveEnemiga( SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 );

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
			      dot->handleEvent( e );
			 }

			 //dot.mover( *otherDot );
                	 dot->mover(otherDot);

			 //Scroll background

			 scrollingOffsetBG -= 0.1;
			 if( scrollingOffsetBG < -dataBG.w )
			 {
				 scrollingOffsetBG = 0;
			 }

			 scrollingOffsetCity -= 5;
		 	 if( scrollingOffsetCity < -dataCiudad.w )
			 {
				 scrollingOffsetCity = 0;
			 }

			 SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			 SDL_RenderClear( gRenderer );

			 gBGTexture.render( scrollingOffsetBG, 0, &dataBG );
			 gBGTexture.render( scrollingOffsetBG + dataBG.w, 0, &dataBG );

			 gPlanetaTexture.render(tierraInicial, -50);
             tierraInicial -= 0.2;

			 gCiudadTexture.render(scrollingOffsetCity, 150, &dataCiudad);
			 gCiudadTexture.render(scrollingOffsetCity + dataCiudad.w, 150, &dataCiudad);

			 //dot.renderizar();
			 dot->renderizar();

                	 otherDot->mover( dot );
                         otherDot->renderizar();

			 SDL_RenderPresent( gRenderer );
        }
}

bool ManejoDeSDL::huboErrorAlIniciarSDL(){
    return SDL_Init( SDL_INIT_VIDEO ) < 0;
}

void ManejoDeSDL::setWindow(SDL_Window* window){
    gWindow = window;
}

void ManejoDeSDL::setRenderer(SDL_Renderer* renderer){
    gRenderer = renderer;
}

SDL_Window* ManejoDeSDL::getWindow(){
    return gWindow;
}

SDL_Renderer* ManejoDeSDL::getRenderer(){
    return gRenderer;
}

void ManejoDeSDL::setScreenWidth(int width){
    screenWidth = width;
}

void ManejoDeSDL::setScreenHeight(int height){
    screenHeight = height;
}

int ManejoDeSDL::getScreenWidth(){
    return screenWidth;
}

int ManejoDeSDL::getScreenHeight(){
    return screenHeight;
}

bool ManejoDeSDL::hayEventos(){
    return SDL_PollEvent( &e ) != 0;
}

bool ManejoDeSDL::usuarioNoRequieraSalir(bool quit){
    return !quit;
}

SDL_Event ManejoDeSDL::getEvento(){
    return e;
}

bool ManejoDeSDL::eventoEsSalir(){
    return e.type == SDL_QUIT;
}

void ManejoDeSDL::renderNave(int x, int y){
    gNaveTexture.render(x,y);
}

void ManejoDeSDL::renderEnemigo(int x, int y){
    gEnemigoTexture.render(x,y);
}
