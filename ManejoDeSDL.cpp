#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "Nave.h"
#include "TextureW.h"
#include "ManejoDeSDL.h"
#include "BotonIniciar.h"

//ACA HABRIA QUE CAMBIARLE A 800x600
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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
    // Cargar la textura del fondo
	if( !gBGTexture.loadFromFile( "sprites/bg.png" ) ){
		printf( "Error al cargar la imagen de fondo!\n" );
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
	gBGTexture.free();


	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	setWindow(NULL);
	setRenderer(NULL);


	IMG_Quit();
	SDL_Quit();
}

void ManejoDeSDL::procesoMenu()
{
        bool quit = false;
        BotonIniciar bt;
        int scrollingOffset = 0;

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
			Nave nave;
			int scrollingOffset = 0;

			// Mientras que siga corriendo la app
			while( usuarioNoRequieraSalir(quit) ){
				while( hayEventos() ){
					if( eventoEsSalir() ) quit = true;
					nave.handleEvent( e );
				}

				nave.move();
				--scrollingOffset;
				if( scrollingOffset < -gBGTexture.getWidth() ) scrollingOffset = 0;

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				gBGTexture.render( scrollingOffset, 0 );
				gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0 );

				nave.render();

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
