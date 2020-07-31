#include "TextureW.h"
#include "ManejoDeSDL.h"
#include "global.h"
#include <SDL2/SDL_ttf.h>

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

	if( loadedSurface != NULL){
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        newTexture = SDL_CreateTextureFromSurface( sdl.getRenderer(), loadedSurface );
		if( newTexture != NULL){
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		SDL_FreeSurface( loadedSurface );
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
bool TextureW::loadFromRenderedText( std::string textureText, std::string action){
	//Get rid of preexisting texture
	//free();
	SDL_Color textColor = { 34, 177, 76, 255 };
	TTF_Font* font = sdl.getFontBox();
	if(action == "log") font = sdl.getFontLog();
	else if(action == "game"){
        textColor = {128,128,0,255};
        font = sdl.getFontBox();
	} else if(action == "fin"){
        textColor = {64,128,128,255};
        font = sdl.getFontLog();
	}


	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
	if( textSurface != NULL ){
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( sdl.getRenderer(), textSurface );
		if( mTexture == NULL ) printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		else{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	} else{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return mTexture != NULL;
}
#endif

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
