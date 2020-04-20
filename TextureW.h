#ifndef TEXTUREW_H_
#define TEXTUREW_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>

//Texture wrapper class
class TextureW
{
	public:
		// Constructor
		TextureW();

		//Deallocates memory
		~TextureW();

		// Carga las imagenees con una ruta especifica
		bool loadFromFile( std::string path );

		#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );

		// Renderiza la textura y obtiene un punto
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		// Obtiene las dimensiones de la imagen
		int getWidth();
		int getHeight();

	private:
		// La textura actual
		SDL_Texture* mTexture;

		// Las dimensiones de la imagen
		int mWidth;
		int mHeight;
};

#endif
