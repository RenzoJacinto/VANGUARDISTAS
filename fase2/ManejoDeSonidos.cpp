#include "ManejoDeSonidos.h"
#include "global.h"

ManejoDeSonidos::ManejoDeSonidos(){
    effectsOff = false;
}

// MUSIC
Mix_Music* ManejoDeSonidos::loadMusic(const char* file){
    Mix_Music* music = Mix_LoadMUS(file);
    if( music == NULL ){
        std::string dir(file);
        std::string msj = "No se pudo cargar la musica con directorio: " + dir;
		logger.error(msj.c_str());
	}
	return music;
}

void ManejoDeSonidos::playMusic(Mix_Music* music){
    if(music != NULL) Mix_PlayMusic( music, -1 );
}

void ManejoDeSonidos::pauseMusic(Mix_Music* music){
    if( Mix_PlayingMusic() == PLAYING ) Mix_PlayMusic( music, -1 );
    else{
        // Si esta pausada, que arranque
        if( Mix_PausedMusic() == PAUSED ) Mix_ResumeMusic();
        else Mix_PauseMusic();
    }
}

void ManejoDeSonidos::haltMusic(){
    Mix_HaltMusic();
}

void ManejoDeSonidos::freeMusic(Mix_Music* music){
    Mix_FreeMusic( music );
	music = NULL;
}

void ManejoDeSonidos::stopMusic(){
    Mix_HaltMusic();
}

// EFFECTS SOUNDS
Mix_Chunk* ManejoDeSonidos::loadEffect(const char* file){

    Mix_Chunk* effect = Mix_LoadWAV(file);
	if( effect == NULL ){
        std::string dir(file);
        std::string msj = "No se pudo cargar el efecto con directorio: " + dir;
		logger.error(msj.c_str());
	}
	return effect;
}

void ManejoDeSonidos::playEffect(Mix_Chunk* effect){
    if(effect != NULL && !effectsOff) Mix_PlayChannel( -1, effect, 0 );
}

// si estan pausados los vuelvo activar, sino al revez
void ManejoDeSonidos::pauseEffects(){
    if(effectsOff) effectsOff = false;
    else effectsOff = true;
}

void ManejoDeSonidos::freeEffect(Mix_Chunk* effect){
    Mix_FreeChunk(effect);
	effect = NULL;
}
