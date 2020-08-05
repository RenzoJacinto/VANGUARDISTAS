#include "BotonIniciar.h"
#include "ManejoDeNiveles.h"

BotonIniciar::BotonIniciar(){
    box = NONE_CLICK;
}

bool BotonIniciar::handleEvent( SDL_Event& e , Mix_Music* gMusic){
    std::string inputTxt = "";
    bool backspace = false;
    int click_act = mouseEvent(e);
    if(click_act == SELECT_NAME) box = SELECT_NAME;
    if(click_act == SELECT_PASS) box = SELECT_PASS;
    if(click_act == NONE_SELECT) box = NONE_SELECT;

    if( e.type == SDL_KEYDOWN){
        //Handle backspace
		if( e.key.keysym.sym == SDLK_BACKSPACE){
            backspace = true;
		} else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL ){
			SDL_SetClipboardText( inputTxt.c_str() );
		} else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL ){
			inputTxt = SDL_GetClipboardText();
		}

        // CUANDO SE APRETA ENTER (QUEDAN LOS ESTADOS FINALES DE ID Y PASS)
        if( e.key.keysym.sym == SDLK_RETURN){
            gNoneSelect.free();
            gSelectName.free();
            gSelectPass.free();
            return false;
        }

    } else if( e.type == SDL_TEXTINPUT ){
        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) ){
			//Append character
			inputTxt += e.text.text;
		}
    }

    if(box == SELECT_NAME){
        gSelectName.render(0,0);
        id += inputTxt;
        if(backspace && id != "") id.pop_back();
        size_t length = id.length();
        if(length > 0 && length <= 27){
            if(! gId.loadFromRenderedText(id.c_str(), ""))
                logger.error("No se pudo cargar la textura del texto");
        }

    } else if(box == SELECT_PASS){
        gSelectPass.render(0,0);
        pass += inputTxt;
        if(backspace && pass != "") pass.pop_back();
        size_t length = pass.length();
        if(length > 0 && length <= 27){
            if(! gPass.loadFromRenderedText(pass.c_str(), ""))
                logger.error("No se pudo cargar la textura del texto");
        }
    } else if(box == NONE_SELECT){
        gNoneSelect.render(0,0);
        if(inputTxt == "m") sounds.pauseMusic(gMusic);
    }

    if(id != "") gId.render(INIT_X_TEXT_NAME, Y_MEDIO_TEXT);
    if(pass != "") gPass.render(INIT_X_TEXT_PASS, Y_MEDIO_TEXT);

    gId.free();
    gPass.free();

    return true;
}

int BotonIniciar::mouseEvent(SDL_Event& e){
    if(e.type == SDL_MOUSEBUTTONDOWN){
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		if(clickOnBoxName(x, y)) return SELECT_NAME;
        else if(clickOnBoxPass(x, y)) return SELECT_PASS;
        else return NONE_SELECT;

	}
	return NONE_CLICK;
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

std::string BotonIniciar::get_ID(){
    return id;
}

std::string BotonIniciar::get_Pass(){
    return pass;
}

void BotonIniciar::inicializar_credenciales(){
    id = "";
    pass = "";
}

void BotonIniciar::cargarImagenes(){
    std::string file = json.get_sprite_menu("selectName");
    if(! gSelectName.loadFromFile(file.c_str()))
        logger.error("No se pudo cargar la textura del select name");
    else logger.debug("Se cargo las imagen de seleccion de box name");

    file = json.get_sprite_menu("selectPass");
    if(! gSelectPass.loadFromFile(file.c_str()))
        logger.error("No se pudo cargar la textura del select password");
    else logger.debug("Se cargo las imagen de seleccion de box password");

    file = json.get_sprite_menu("noneSelect");
    if(! gNoneSelect.loadFromFile(file.c_str()))
        logger.error("No se pudo cargar la textura del none select");
    else logger.debug("Se cargo las imagen de seleccion de none select box");
}
