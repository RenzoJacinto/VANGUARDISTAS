#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "Nave.h"
#include "Fondo.h"
#include "ManejoDeSDL.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

ManejoDeSDL::ManejoDeSDL()
{
    setScreenWidth(SCREEN_WIDTH);
    setScreenHeight(SCREEN_HEIGHT);

    //The window we'll be rendering to
    setWindow(NULL);

    //The window renderer
    setRenderer(NULL);
}

bool ManejoDeSDL::iniciarSDL()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( huboErrorAlIniciarSDL() )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		setWindow( SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN ) );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			setRenderer( SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ) );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool ManejoDeSDL::cargarImagen()
{
	//Loading success flag
	bool success = true;

	//Load dot texture
	if( !gDotTexture.loadFromFile( "dot.png" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	//Load background texture
	if( !gBGTexture.loadFromFile( "bg.png" ) )
	{
		printf( "Failed to load background texture!\n" );
		success = false;
	}

	return success;
}

void ManejoDeSDL::cerrar()
{
	//Free loaded images
	gDotTexture.free();
	gBGTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	setWindow(NULL);
	setRenderer(NULL);

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void ManejoDeSDL::proceso() {
            //Main loop flag
			bool quit = false;

			//The dot that will be moving around on the screen
			Nave dot;

			//The background scrolling offset
			int scrollingOffset = 0;

			//While application is running
			while( usuarioNoRequieraSalir(quit) )
			{
				//Handle events on queue
				while( hayEventos() )
				{
					//User requests quit
					if( eventoEsSalir() )
					{
						quit = true;
					}

					//Handle input for the dot
					dot.handleEvent( e );
				}

				//Move the dot
				dot.move();

				//Scroll background
				--scrollingOffset;
				if( scrollingOffset < -gBGTexture.getWidth() )
				{
					scrollingOffset = 0;
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render background
				gBGTexture.render( scrollingOffset, 0 );
				gBGTexture.render( scrollingOffset + gBGTexture.getWidth(), 0 );

				//Render objects
				dot.render();

				//Update screen
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

Fondo ManejoDeSDL::getDotTexture(){
    return gDotTexture;
}

Fondo ManejoDeSDL::getBGTexture(){
    return gBGTexture;
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
