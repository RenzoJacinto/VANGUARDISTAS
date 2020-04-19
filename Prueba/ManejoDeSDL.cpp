#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "Nave.h"
#include "Fondo.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
Fondo gDotTexture;
Fondo gBGTexture;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool iniciarSDL()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
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
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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

bool cargarImagen()
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

void cerrar()
{
	//Free loaded images
	gDotTexture.free();
	gBGTexture.free();

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void proceso() {
            //Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Nave dot;

			//The background scrolling offset
			int scrollingOffset = 0;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
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

SDL_Renderer* getRenderer(){
    return gRenderer;
}

int getScreenWidth(){
    return SCREEN_WIDTH;
}

int getScreenHeight(){
    return SCREEN_HEIGHT;
}

Fondo getDotTexture(){
    return gDotTexture;
}

