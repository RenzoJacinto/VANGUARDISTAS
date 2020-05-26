#include "ManejoDeJson.h"
#include <stdio.h>
#include "global.h"


ManejoDeJson::ManejoDeJson(){}

bool ManejoDeJson::abrir_archivo(){
    if(!abrir_archivo_aux("config/configuracion.json")) logger.error("No se encuentra el archivo en la direccion config/configuracion.json");
    else {
        archivo_json >> j;
        archivo_json.close();
        logger.debug("Se cargo el archivo de configuracion de json");
    }
    if(!abrir_archivo_aux("config/default.json")){
        logger.error("No se encuentra el archivo en la direccion config/default.json");
        return false;
    } else {
        archivo_json >> def;
        archivo_json.close();
        logger.debug("Se cargo el archivo de configuracion de json");
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
}

int ManejoDeJson::get_nivel_de_log(){
    try { return j.at("log"); }
    catch (nlohmann::detail::out_of_range) {
        logger.error("No se encuentra el log");
        return def.at("log");
    }
}

nlohmann::json& ManejoDeJson::searchValue(json& j_aux, const char* key){
    for(auto& el : j_aux.items()) {
        if(el.key() == key) return el.value();
    }
    string clave(key);
    std::string mensaje = "No se encontro la clave: " + key;
    logger.error(mensaje.c_str());
    return searchValue(def, key);
}

std::string ManejoDeJson::get_sprite_mapa(char const* key, const char* sp){

    json& j_aux = searchValue(j, "stages");
    json& j_nivel = searchValue(j_aux, key);
    json& j_sprites = searchValue(j_nivel, "sprites");

    return j_sprites.at(sp);
}

int ManejoDeJson::get_cantidad_enemigo(const char* key){

    json& j_aux = searchValue(j, "stages");
    json& j_nivel = searchValue(j_aux, key);

    return j_nivel.at("enemigos");
}

std::string ManejoDeJson::get_sprite_nave(const char* key, const char* sp){

    json& j_aux = searchValue(j, "naves");
    json& j_nivel = searchValue(j_aux, key);

    return j_nivel.at(sp);
}
