#include "ManejoDeJson.h"
#include <stdio.h>


ManejoDeJson::ManejoDeJson(){}

bool ManejoDeJson::abrir_archivo(){
    if(!abrir_archivo_aux("config/configuracion.json")) {
        printf("No se encuentra el archivo en la direccion config/configuracion.json\n");
    }
    else {
        archivo_json >> j;
        archivo_json.close();
    }
    if(!abrir_archivo_aux("config/default.json")){
        printf("No se encuentra el archivo en la direccion config/default.json\n");
        return false;
    }
    else {
        archivo_json >> def;
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
        return def.at("menu");
    }
}

int ManejoDeJson::get_nivel_de_log(){
    try { return j.at("log"); }
    catch (nlohmann::detail::out_of_range) {
        return def.at("log");
    }
}

nlohmann::json& ManejoDeJson::searchValue(json& j_aux, const char* key){
    for(auto& el : j_aux.items()) {
        if(el.key() == key) return el.value();
    }
    return j_aux;
}

std::string ManejoDeJson::get_sprite_mapa_default(char const* key, const char* sp){

        json& j_aux = searchValue(def, "stages");
        json& j_nivel = searchValue(j_aux, key);
        json& j_sprites = searchValue(j_nivel, "sprites");

        for(auto& el : j_sprites.items()) {
            std::string sActualKey = el.key();
            if(strstr(sActualKey.c_str(),sp)){
                return el.value();
            }
        }
        return NULL;
}
std::string ManejoDeJson::get_sprite_mapa(char const* key, const char* sp){

    try {
        json& j_aux = searchValue(j, "stages");
        json& j_nivel = searchValue(j_aux, key);
        json& j_sprites = searchValue(j_nivel, "sprites");

        for(auto& el : j_sprites.items()) {
            std::string sActualKey = el.key();
            if(strstr(sActualKey.c_str(),sp)){
                return el.value();
            }
        }
        return NULL;
    }

    catch (nlohmann::detail::out_of_range)  {
        return get_sprite_mapa_default(key, sp);
    }

    catch (std::logic_error) {
        return get_sprite_mapa_default(key, sp);
    }
}

int ManejoDeJson::get_cantidad_enemigo_default(char const* key) {
        json& j_aux = searchValue(def, "stages");
        json& j_nivel = searchValue(j_aux, key);

        return j_nivel.at("enemigos");
}

int ManejoDeJson::get_cantidad_enemigo(char const* key){

    try {
        json& j_aux = searchValue(j, "stages");
        json& j_nivel = searchValue(j_aux, key);

        return j_nivel.at("enemigos");
    }

    catch (nlohmann::detail::out_of_range)  {
        return get_cantidad_enemigo_default(key);
    }

    catch (std::logic_error) {
        return get_cantidad_enemigo_default(key);
    }
}

std::string ManejoDeJson::get_sprite_nave_default(char const* key, char const* sp) {
    json& j_aux = searchValue(def, "naves");
    json& j_nivel = searchValue(j_aux, key);

    for(auto& el : j_nivel.items()) {
        std::string sActualKey = el.key();
        std::string sActualValue = el.value();
        if(strstr(sActualKey.c_str(),sp)){
            return sActualValue;
        }
    }
    return NULL;
}

std::string ManejoDeJson::get_sprite_nave(char const* key, char const* sp){

    try {
        json& j_aux = searchValue(j, "naves");
        json& j_nivel = searchValue(j_aux, key);

        for(auto& el : j_nivel.items()) {
            std::string sActualKey = el.key();
            std::string sActualValue = el.value();
            if(strstr(sActualKey.c_str(),sp)){
                return sActualValue;
            }
        }
        return NULL;
    }

    catch (nlohmann::detail::out_of_range)  {
        return get_sprite_nave_default(key, sp);
    }

    catch (std::logic_error) {
        return get_sprite_nave_default(key, sp);
    }
}
