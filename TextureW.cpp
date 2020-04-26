#include "TextureW.h"
#include "ManejoDeSDL.h"
#include "global.h"

TextureW::TextureW(){
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

TextureW::~TextureW(){
	free();
}

bool TextureW::loadFromFile( std::string path ){

    //free();
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

	if( loadedSurface == NULL){
        logger.error(IMG_GetError());
	} else{
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( sdl.getRenderer(), loadedSurface );
		if( newTexture == NULL){
            logger.error(SDL_GetError());
		} else{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

void TextureW::free(){
	if( mTexture != NULL ){
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void TextureW::setColor( Uint8 red, Uint8 green, Uint8 blue ){
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void TextureW::setBlendMode( SDL_BlendMode blending ){
	SDL_SetTextureBlendMode( mTexture, blending );
}

void TextureW::setAlpha( Uint8 alpha ){
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void TextureW::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip ){

	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	if( clip != NULL ){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx( sdl.getRenderer(), mTexture, clip, &renderQuad, angle, center, flip );
}

int TextureW::getWidth(){
	return mWidth;
}

int TextureW::getHeight(){
	return mHeight;
}
