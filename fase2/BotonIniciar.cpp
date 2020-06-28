#include "BotonIniciar.h"
#include "ManejoDeNiveles.h"

BotonIniciar::BotonIniciar(){
    box = NONE_SELECT;
    std::string file = json.get_sprite_menu("selectName");
    if(! gSelectName.loadFromFile(file.c_str()))
        logger.error("No se pudo cargar la textura del select name");
    file = json.get_sprite_menu("selectPass");
    if(! gSelectPass.loadFromFile(file.c_str()))
        logger.error("No se pudo cargar la textura del select password");
}

void BotonIniciar::handleEvent( SDL_Event& e ){

    int click_act = mouseEvent(e);
    if(click_act != box && click_act != NONE_SELECT){
        inputText = " ";
    }


    if(click_act == SELECT_NAME) box = SELECT_NAME;
    else if(click_act == SELECT_PASS) box = SELECT_PASS;

	if( e.type == SDL_KEYDOWN){
        // ACOMODO EL STRING EN EL CASO DE CAMBIO DE BOX
        if(inputText == " ") inputText = "";

        //Handle backspace
		if( e.key.keysym.sym == SDLK_BACKSPACE && inputText != "" ){
			//lop off character
            inputText.pop_back();
		} else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL ){
			SDL_SetClipboardText( inputText.c_str() );
		} else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL ){
			inputText = SDL_GetClipboardText();
		}
		 // Cuando se apreta Enter ARRANCA
        if( e.key.keysym.sym == SDLK_RETURN){
            ManejoDeNiveles niveles;
            // SE QUEDAN GUARDADO 'ID' Y 'PASS' PARA LAS CREDENCIALES
            niveles.procesar();
        }
    } else if( e.type == SDL_TEXTINPUT ){
        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) ){
			//Append character
			inputText += e.text.text;
		}
    }

    if(box == SELECT_NAME){
        gSelectName.render(0,0);
        if(inputText != " "){
            id = inputText;
            size_t length = id.length();
            if(length > 0 && length <= 49){
                if(! gId.loadFromRenderedText(id.c_str()))
                    logger.error("No se pudo cargar la textura del texto");
            }
        }

    } else if(box == SELECT_PASS){
        gSelectPass.render(0,0);
        if(inputText != " "){
            pass = inputText;
            size_t length = pass.length();
            if(length > 0 && length <= 49){
                if(! gPass.loadFromRenderedText(pass.c_str()))
                    logger.error("No se pudo cargar la textura del texto");
            }
        }
    } else inputText = "";

    if(id != "") gId.render(INIT_X_TEXT_NAME, Y_MEDIO_TEXT);
    if(pass != "") gPass.render(INIT_X_TEXT_PASS, Y_MEDIO_TEXT);

}

int BotonIniciar::mouseEvent(SDL_Event& e){
    if(e.type == SDL_MOUSEBUTTONDOWN){
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		if(clickOnBoxName(x, y)) return SELECT_NAME;
        else if(clickOnBoxPass(x, y)) return SELECT_PASS;

	}
	return NONE_SELECT;
}

bool BotonIniciar::clickOnBoxName(int x, int y){
    bool ok = false;
    if(x > INIT_X_TEXT_NAME && x < FIN_X_TEXT_NAME){
        if(y > INIT_Y_TEXT && y < FIN_Y_TEXT) ok = true;
    }
    return ok;
}

bool BotonIniciar::clickOnBoxPass(int x, int y){
    bool ok = false;
    if(x > INIT_X_TEXT_PASS && x < FIN_X_TEXT_PASS){
        if(y > INIT_Y_TEXT && y < FIN_Y_TEXT) ok = true;
    }
    return ok;
}
