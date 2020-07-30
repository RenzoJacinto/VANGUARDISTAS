#include "ManejoDeJson.h"
#include <stdio.h>
#include "global.h"

ManejoDeJson::ManejoDeJson(){}

bool ManejoDeJson::abrir_archivo(){
    bool abrio_config = true;
    if(!abrir_archivo_aux("config/configuracion.json")){
        abrio_config = false;
        logger.error("No se encuentra el archivo en la direccion config/configuracion.json");
    } else {
        try{archivo_json >> j;}
        catch(nlohmann::detail::parse_error ){
            logger.error("El archivo de configuracion.json esta creado incorrectamente");
            abrio_config = false;
        }
        archivo_json.close();
    }
    if(!abrir_archivo_aux("config/default.json")){
        logger.error("No se encuentra el archivo en la direccion config/default.json");
        return false;
    } else {
        try{archivo_json >> def;}
        catch(nlohmann::detail::parse_error){
            logger.error("El archivo default.json esta creado incorrectamente");
        }

        if(!abrio_config) j = def;
        archivo_json.close();
    }

    return true;
}

bool ManejoDeJson::abrir_archivo_aux(const char* filename){
    archivo_json.open(filename, ios::in);
    if(!archivo_json) return false;
    return true;
}


nlohmann::json& ManejoDeJson::searchValue(json& j_aux, const char* key){
    for(auto& el : j_aux.items()) {
        if(el.key() == key) return el.value();
    }
    string clave(key);
    std::string mensaje = "No se encontro la clave: " + clave;
    logger.error(mensaje.c_str());
    return def.at("error");
}


std::string ManejoDeJson::get_sprite_menu(const char* sp){
    json& j_aux = searchValue(j, "menu");
    if(j_aux == "errorKey") return get_sprite_menu_default(sp);

    string sSP(sp);

    try { return j_aux.at(sp); }
    catch (nlohmann::detail::out_of_range) {
        std::string msj = "No se encontro el sprite " + sSP + " del menu. Se obtiene de default.json";
        logger.error(msj.c_str());
        return get_sprite_menu_default(sp);
    }
    catch(nlohmann::detail::type_error){
        logger.error("El sprite buscado debe ser un string y es un numero");
        return get_sprite_menu_default(sp);
    }
}

int ManejoDeJson::get_nivel_de_log(){
    try { return j.at("log"); }
    catch (nlohmann::detail::out_of_range) {
        logger.error("No se encuentra el log. Se abre el nivel de log de default.json");
        return get_nivel_de_log_default();
    }
    catch(nlohmann::detail::type_error){
        logger.error("El nivel de log debe ser un numero y es un string. Se abre el nivel de log de default.json");
        return get_nivel_de_log_default();
    }
}

std::string ManejoDeJson::get_sprite_mapa(char const* key, const char* sp){

    json& j_aux = searchValue(j, "stages");
    if(j_aux == "errorKey") return get_sprite_mapa_default(key, sp);
    json& j_nivel = searchValue(j_aux, key);
    if(j_nivel == "errorKey") return get_sprite_mapa_default(key, sp);
    json& j_sprites = searchValue(j_nivel, "sprites");
    if(j_sprites == "errorKey") return get_sprite_mapa_default(key, sp);

    string sSP(sp);

    try{return  j_sprites.at(sp);}
    catch (nlohmann::detail::out_of_range) {
        std::string mensaje1 = "No se encontro el sprite: " + sSP + ". Se obtiene de default.json";
        logger.error(mensaje1.c_str());
        return get_sprite_mapa_default(key, sp);
    }
    catch(nlohmann::detail::type_error){
        std::string mensaje1 = "El sprite " + sSP + ", debe ser un string y es un numero. Se obtiene de default.json";
        logger.error(mensaje1.c_str());
        return get_sprite_mapa_default(key, sp);
    }
}

std::string ManejoDeJson::get_sprite_nave(const char* key, const char* tipo, const char* sp){

    json& j_aux = searchValue(j, key);
    if(j_aux == "errorKey") return get_sprite_nave_default(key, tipo, sp);
    json& j_nave = searchValue(j_aux, tipo);
    if(j_nave == "errorKey") return get_sprite_nave_default(key, tipo, sp);

    string sSP(sp);
    string stipo(tipo);

    try{return  j_nave.at(sp);}
    catch (nlohmann::detail::out_of_range) {
        std::string mensaje1 = "No se encontro el sprite: " + sSP + " de " + stipo + ". Se obtiene de default.json";
        logger.error(mensaje1.c_str());
        return get_sprite_nave_default(key, tipo, sp);
    }
    catch(nlohmann::detail::type_error){
        std::string mensaje1 = "El sprite " + sSP + "de " + stipo + ", debe ser un string y es un numero. Se obtiene de default.json";
        logger.error(mensaje1.c_str());
        return get_sprite_nave_default(key, tipo, sp);
    }
}

 std::string ManejoDeJson::get_sprite_puntajes(const char* sp){
    json& j_aux = searchValue(j, "puntajes");
    if(j_aux == "errorKey") return get_sprite_puntajes_default(sp);

    string sSP(sp);

    try { return j_aux.at(sp); }
    catch (nlohmann::detail::out_of_range) {
        std::string msj = "No se encontro el sprite " + sSP + " de puntajes. Se obtiene de default.json";
        logger.error(msj.c_str());
        return get_sprite_puntajes_default(sp);
    }
    catch(nlohmann::detail::type_error){
        logger.error("El sprite buscado debe ser un string y es un numero");
        return get_sprite_puntajes_default(sp);
    }

 }

// PARA LOS DE DEFAULT (SOLO LLAMADAS EN CASOS DE ERROR DE CLAVE)

std::string ManejoDeJson::get_sprite_mapa_default(char const* key, const char* sp){

    json& j_aux = searchValue(def, "stages");
    json& j_nivel = searchValue(j_aux, key);
    json& j_sprites = searchValue(j_nivel, "sprites");

    string sSP(sp);
    std::string mensaje = "Se obtuvo de default.json el sprite: " + sSP;

    logger.debug(mensaje.c_str());

    return j_sprites.at(sp);
}

std::string ManejoDeJson::get_sprite_nave_default(const char* key, const char* tipo, const char* sp){

    json& j_aux = searchValue(def, key);
    json& j_nave = searchValue(j_aux, tipo);

    string sSP(sp);
    string stipo(tipo);
    std::string mensaje = "Se obtuvo de default.json el sprite: " + sSP + " de " + stipo;

    logger.debug(mensaje.c_str());

    return j_nave.at(sp);
}

std::string ManejoDeJson::get_sprite_menu_default(const char* sp){

    json& j_aux = searchValue(def, "menu");

    string sSP(sp);
    std::string msj = "Se obtuvo el sprite " + sSP + " del menu de default.json";
    logger.debug(msj.c_str());

    return j_aux.at(sp);
}

int ManejoDeJson::get_nivel_de_log_default(){
    logger.debug("Se obtuvo el nivel de log de default.json");
    return def.at("log");
}

std::string ManejoDeJson::get_sprite_puntajes_default(const char* sp){

    json& j_aux = searchValue(def, "puntajes");

    string sSP(sp);
    std::string msj = "Se obtuvo el sprite " + sSP + " de puntajes de default.json";
    logger.debug(msj.c_str());

    return j_aux.at(sp);
}

std::string ManejoDeJson::get_sound_default(char const* tipo, const char* snd){

    json& j_aux = searchValue(def, "sounds");
    json& j_sound = searchValue(j_aux, tipo);


    string sSP(snd);
    std::string mensaje = "Se obtuvo el sprite: " + sSP + " de sonidos de default.json";
    logger.debug(mensaje.c_str());

    return j_sound.at(snd);
}


// PARA LLAMADAS DE ERROR DE ENCONTRAR SPRITE
std::string ManejoDeJson::get_imagen_default(const char* sp){
    json& j_aux = searchValue(def, "default");
    logger.debug("Se obtuvo la imagen de error, que muestra inexistencia de imagen");
    return j_aux.at(sp);
}


// PARA LOS SONIDOS

std::string ManejoDeJson::get_sound(char const* tipo, const char* snd){

    json& j_aux = searchValue(j, "sounds");
    if(j_aux == "errorKey") return get_sound_default(tipo, snd);
    json& j_sound = searchValue(j_aux, tipo);
    if(j_sound == "errorKey") return get_sound_default(tipo, snd);

    string sSP(snd);

    try{return  j_sound.at(snd);}
    catch (nlohmann::detail::out_of_range) {
        std::string mensaje1 = "No se encontro el sprite: " + sSP + ". Se obtiene de default.json";
        logger.error(mensaje1.c_str());
        return get_sound_default(tipo, snd);
    }
    catch(nlohmann::detail::type_error){
        std::string mensaje1 = "El sprite " + sSP + ", debe ser un string y es un numero. Se obtiene de default.json";
        logger.error(mensaje1.c_str());
        return get_sound_default(tipo, snd);
    }
}
