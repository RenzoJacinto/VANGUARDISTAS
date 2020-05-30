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
            logger.error("El archivo de configuracion.json esta incompleto");
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
            logger.error("El archivo default.json");
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

std::string ManejoDeJson::get_sprite_menu(){
    try { return j.at("menu"); }
    catch (nlohmann::detail::out_of_range) {
        logger.error("No se encuentra el menu");
        return def.at("menu");
    }
     catch(nlohmann::detail::type_error){
        logger.error("El sprite del menu debe ser un string y es un numero");
        return get_sprite_menu_default();
    }
}

int ManejoDeJson::get_nivel_de_log(){
    try { return j.at("log"); }
    catch (nlohmann::detail::out_of_range) {
        logger.error("No se encuentra el log");
        return def.at("log");
    }
    catch(nlohmann::detail::type_error){
        logger.error("El nivel de log debe ser un numero y es un string");
        return get_nivel_de_log_default();
    }
}

nlohmann::json& ManejoDeJson::searchValue(json& j_aux, const char* key){
    for(auto& el : j_aux.items()) {
        if(el.key() == key) return el.value();
    }
    string clave(key);
    std::string mensaje = "No se encontro la clave: " + clave + ", se abre el archivo default.json";
    logger.error(mensaje.c_str());
    return def;
}

std::string ManejoDeJson::get_sprite_mapa(char const* key, const char* sp){

    json& j_aux = searchValue(j, "stages");
    if(j_aux == def) return get_sprite_mapa_default(key, sp);
    json& j_nivel = searchValue(j_aux, key);
    if(j_nivel == def) return get_sprite_mapa_default(key, sp);
    json& j_sprites = searchValue(j_nivel, "sprites");
    if(j_sprites == def) return get_sprite_mapa_default(key, sp);

    try{return  j_sprites.at(sp);}
    catch(nlohmann::detail::type_error){
        string sKey(key);
        string sSP(sp);
        std::string mensaje = "En stages y en el nivel " + sKey + " el sprite " + sSP + "debe ser un string y es un numero";
        logger.error(mensaje.c_str());
        return get_sprite_mapa_default(key, sp);
    }
}

int ManejoDeJson::get_cantidad_enemigo(const char* key){

    json& j_aux = searchValue(j, "stages");
    if(j_aux == def) return get_cantidad_enemigo_default(key);
    json& j_nivel = searchValue(j_aux, key);
    if(j_nivel == def) return get_cantidad_enemigo_default(key);

    try{return j_nivel.at("enemigos");}
    catch(nlohmann::detail::type_error){
        string sKey(key);
        std::string mensaje = "En stages y en el nivel " + sKey + " la cantidad de enemigos debe ser un numero y es un string";
        logger.error(mensaje.c_str());
        return get_cantidad_enemigo_default(key);
    }
}

std::string ManejoDeJson::get_sprite_nave(const char* key, const char* sp){

    json& j_aux = searchValue(j, "naves");
    if(j_aux == def) return get_sprite_nave_default(key, sp);
    json& j_nave = searchValue(j_aux, key);
    if(j_nave == def) return get_sprite_nave_default(key, sp);

    try{return  j_nave.at(sp);}
    catch(nlohmann::detail::type_error){
        string sKey(key);
        string sSP(sp);
        std::string mensaje = "En naves y en la nave " + sKey + " el sprite " + sSP + "debe ser un string y es un numero";
        logger.error(mensaje.c_str());
        return get_sprite_nave_default(key, sp);
    }
}

// PARA LOS DE DEFAULT (SOLO LLAMADAS EN CASOS DE ERROR DE CLAVE)

std::string ManejoDeJson::get_sprite_mapa_default(char const* key, const char* sp){

    json& j_aux = searchValue(def, "stages");
    json& j_nivel = searchValue(j_aux, key);
    json& j_sprites = searchValue(j_nivel, "sprites");

    return j_sprites.at(sp);
}

int ManejoDeJson::get_cantidad_enemigo_default(const char* key){

    json& j_aux = searchValue(def, "stages");
    json& j_nivel = searchValue(j_aux, key);

    return j_nivel.at("enemigos");
}

std::string ManejoDeJson::get_sprite_nave_default(const char* key, const char* sp){

    json& j_aux = searchValue(def, "naves");
    json& j_nivel = searchValue(j_aux, key);

    return j_nivel.at(sp);
}

std::string ManejoDeJson::get_sprite_menu_default(){
    return def.at("menu");
}

int ManejoDeJson::get_nivel_de_log_default(){
    return def.at("log");
}

// PARA LLAMADAS DE ERROR DE ENCONTRAR SPRITE
std::string ManejoDeJson::get_imagen_default(const char* sp){
    json& j_aux = searchValue(def, "default");
    return j_aux.at(sp);
}

std::string ManejoDeJson::get_estado_aplicacion(){
    try { return j.at("estado"); }
    catch (nlohmann::detail::out_of_range) {
        logger.error("No se encuentra el estado de la aplicación");
        return def.at("estado");
    }
    catch(nlohmann::detail::type_error){
        logger.error("El estado debe ser un string y es un número");
        return def.at("estado");
    }
}

