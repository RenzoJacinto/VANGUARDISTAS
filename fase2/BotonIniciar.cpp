#include "BotonIniciar.h"
#include "ManejoDeNiveles.h"

BotonIniciar::BotonIniciar(){}

void BotonIniciar::handleEvent( SDL_Event& e ){

    int i = mouseEvent(e);

	if( e.type == SDL_KEYDOWN){
        // Cuando se apreta Enter
        //Handle backspace
		if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ){
			//lop off character
            inputText.pop_back();
		} else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL ){
			SDL_SetClipboardText( inputText.c_str() );
		} else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL ){
			inputText = SDL_GetClipboardText();
		}
        if( e.key.keysym.sym == SDLK_RETURN){
            ManejoDeNiveles niveles;
            niveles.procesar();
        }
    } else if( e.type == SDL_TEXTINPUT ){
        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) ){
			//Append character
			inputText += e.text.text;
		}
    }
    if(i == 0) box = 0;
    if(i == 1) box = 1;

    /*std::cout<<inputStringId<<"\n";
    size_t length1 = inputStringId.length();
    size_t length2 = inputStringPass.length();*/
    size_t length = inputText.length();

    /*if(length1 > 0 && length1 < 49){
        gInputTextIdTexture.loadFromRenderedText( inputStringId.c_str());
        gInputTextIdTexture.render(128, gInputTextIdTexture.getHeight());

    }
    if(length2 > 0 && length2 < 49){
        gInputTextPassTexture.loadFromRenderedText( inputStringId.c_str());
        gInputTextPassTexture.render(453, gInputTextPassTexture.getHeight());
    }*/
    //if(box != -1) std::cout<<inputText<<"\n";
    //else inputText = "";
    if(box == -1) inputText = "";
}

int BotonIniciar::mouseEvent(SDL_Event& e){
    if(e.type == SDL_MOUSEBUTTONDOWN){
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		if(clickOnBoxName(x, y)) return 0;
        else if(clickOnBoxPass(x, y)) return 1;
	}
	return -1;
}

bool BotonIniciar::clickOnBoxName(int x, int y){
    bool ok = false;
    if(x > 128 && x < 385){
        if(y > 342 && y < 367) ok = true;
    }
    return ok;
}

bool BotonIniciar::clickOnBoxPass(int x, int y){
    bool ok = false;
    if(x > 453 && x < 677){
        if(y > 342 && y < 367) ok = true;
    }
    return ok;
}

std::string BotonIniciar::get_inputTxt(){
    return inputText;
}
