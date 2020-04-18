#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

#include "ManejoDeSDL.h"

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !iniciarSDL() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !cargarImagen() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( hayEventos() )
				{
					//User requests quit
					if( usuarioRequirioSalir() )
					{
						quit = true;
					}
				}

				//Apply the image
				aplicarImagen();

				//Update the surface
				actualizarSuperficie();
			}
		}
	}

	//Free resources and close SDL
	cerrar();

	return 0;
}
